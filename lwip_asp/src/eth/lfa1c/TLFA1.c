//________________________________________________________________________________________
//
//	LFA1 AX88796 EtherNET LAN デバイスドライバ　サンプル
//	16bitバス ビッグエンディアンのみ対応
//
//----------------------------------------------------------------------------------------
//	開発履歴
//
//	2009/11/25
//
//----------------------------------------------------------------------------------------
//	㈱L&F
//________________________________________________________________________________________
//

#include	"TAX88796.H"					/* AX88796 定義 */

//________________________________________________________________________________________
//
//	定数定義
//________________________________________________________________________________________
//
#define NULL 0

//----------------------------------------------------------------------------------------
//	一般定義
//----------------------------------------------------------------------------------------
#define		A1_SendBuff0_Base		0x4000					/* 送信バッファ0ベースアドレス */
#define		A1_SendBuff1_Base		0x4600					/* 送信バッファ1ベースアドレス */
#define		A1_RecvBuff_Start		0x4C00					/* 受信バッファ先頭アドレス */
#define		A1_RecvBuff_End			0x8000					/* 受信バッファ終了+1アドレス */
	
#define		A1_LAN_BASE    			0x20000000				/* CS0(キャッシュ無効) */
//#define		A1_LAN_BASE    			0x28000000				/* CS2(キャッシュ無効) */

#ifdef BUS_8_BIT
#define		A1_LAN_ADDR(r)			(A1_LAN_BASE+(r))		/* 各レジスタのアドレス */
#else
#define		A1_LAN_ADDR(r)			(A1_LAN_BASE+(r * 2))	/* 各レジスタのアドレス */
#endif

typedef		void	(*PROC_CALL)(void);

#ifndef BUS_16_BIT_LITTLE
#define swapb(a) a
#else
#define swapb(a) (((a >> 8) & 0x00ff) | ((a << 8) & 0xff00))
#endif
#define TThread_loop()
#define TThread_sleep(a) {dly_tsk(a);}

//________________________________________________________________________________________
//
//	8/16ビット接続共通
//________________________________________________________________________________________
//
//	データレジスタの読み書き
static unsigned char	TLFA1X_ind(int r){return (*(volatile unsigned char *)A1_LAN_ADDR(r));}
static unsigned char	TLFA1X_outd(int r, unsigned char d){(*(volatile unsigned char *)A1_LAN_ADDR(r)) = (d); return d;}

//	ワードの読み書き
static unsigned short	TLFA1X_inw(int r){return (*(volatile unsigned short *)A1_LAN_ADDR(r));}
static unsigned short	TLFA1X_outw(int r, unsigned short d){(*(volatile unsigned short *)A1_LAN_ADDR(r)) = (d); return d;}

//________________________________________________________________________________________
//
//	プロトタイプ宣言
//________________________________________________________________________________________
//

// ローカル
void			TLFA1_Dummy_Wait(void);													// 時間待ちウェイト
void			TLFA1_remoteDMA_write(int adr, int len, void *src);						// リモートDMAデータ書き込み
void			TLFA1_remoteDMA_read(int adr, int len, void *dst);						// リモートDMAデータ読み出し
void			TLFA1_SetMultiCastAaddress(unsigned char *src);							// マルチキャストアドレス設定(NICレジスタへの設定)
void			TLFA1_GetMultiCastAaddress(unsigned char *dst);							// マルチキャストアドレス取得(NICレジスタからの取得)
void			TLFA1_SendStart(int BuffBase, int len);									// パケット送信開始

void			TLFA1_miiout(unsigned char data);										// data( 0 or 1)をPHYへ書き出す
unsigned char	TLFA1_miiin(void);														// data( 0 or 1)をPHYから読み込む
unsigned int	TLFA1_miirw(unsigned char opcode, unsigned char reg_addr, unsigned int wdata);// MIIへの読み書き

// グローバル
void			TLFA1_SetMACaddress(unsigned char *src);								// MACアドレス設定(NICレジスタへの設定)
void			TLFA1_GetMACaddress(unsigned char *dst);								// MACアドレス取得(NICレジスタからの取得)
void			TLFA1_SendEnableCtrl(int flg);											// 送信部イネーブル/ディセーブル制御
void			TLFA1_RecvEnableCtrl(int flg);											// 受信部イネーブル/ディセーブル制御
int				TLFA1_Init(unsigned char *mac);											// NIC初期化

void			TLFA1_Media_Set(int media);												// 動作方法の設定

void			TLFA1_Start(void);														// NIC使用開始
void			TLFA1_Stop(void);														// NIC使用終了

int				TLFA1_GetStatus(void);													// NICステータス取得

int				TLFA1_SetSendPacket(unsigned char *buff, int len);						// 送信パケット書き込み
int				TLFA1_CheckSendBuffer(void);											// 送信バッファ空き状態チェック
void			TLFA1_SendPacketRetry(void);											// 送信バッファ再送信
void			TLFA1_Interrupt_Init(void *rx, void *tx, void *re, void *te);			// NIC割り込み初期化
int				TLFA1_Interrupt(void);													// NIC割り込み処理
void			TLFA1_CheckSentPacket(void);											// 送信パケットチェック
int				TLFA1_CheckRecvPacket(void);											// 受信パケット有無チェック

int				TLFA1_GetRecvPacket(unsigned char *buff, int length);					// 受信パケット取り出し


//________________________________________________________________________________________
//
//	ローカル変数
//________________________________________________________________________________________
//

volatile int			a1_SendBuff_Select;						/* 送信バッファ選択 */
volatile int			a1_SendBuff0_Send;						/* 送信バッファ0動作フラグ */
volatile unsigned short	a1_SendBuff0_len;						/* 送信バッファ0送信バイト数 */
volatile int			a1_SendBuff1_Send;						/* 送信バッファ1動作フラグ */
volatile unsigned short	a1_SendBuff1_len;						/* 送信バッファ1送信バイト数 */
volatile int			a1_RecvPacket_Ready;					/* 内部に受信パケットあり */

void					*a1_net_sned_event_proc = NULL;			// 送信完了イベント処理先
void					*a1_net_sned_error_proc = NULL;			// 送信エラーイベント処理先
void					*a1_net_recv_event_proc = NULL;			// 受信完了イベント処理先
void					*a1_net_recv_error_proc = NULL;			// 受信エラーイベント処理先

unsigned char			a1_net_mac_addres[6] = {0,0,0,0,0,0};	// MAC-address

//________________________________________________________________________________________
//
//	処理時間待ち
//----------------------------------------------------------------------------------------
//	引数
//		無し
//	戻り
//		無し
//________________________________________________________________________________________
//
void TLFA1_Dummy_Wait(void)
{
	unsigned char c;
	int i;

	for(i = 0; i < 0x1000; i++)
	{
		c = TLFA1X_ind(Px_CR);
	}
}

//________________________________________________________________________________________
//
//	リモートDMAデータ読み出し
//----------------------------------------------------------------------------------------
//	引数
//		adr		読み出し開始アドレス
//		len		読み出すバイト数
//		*dst	データ保存先
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_remoteDMA_read(int adr, int len, void *dst)
{
	int				i, end;
	unsigned short	data, *buff;
	unsigned char	wkc;

	if(len == 0) return;

	buff = (unsigned short *)dst;
	end = (len >> 1) + (len & 1);		//奇数バイトの場合は +1 ワード読み出しが必要

	wkc = adr & 0xff;
	TLFA1X_outd(P0_RSAR0, wkc);			/* 読み出し開始アドレス(下位) */
	wkc = adr >> 8;
	TLFA1X_outd(P0_RSAR1, wkc);			/* 読み出し開始アドレス(上位) */

	wkc = len & 0xff;
	TLFA1X_outd(P0_RBCR0, wkc);			/* 読み出しバイト数(下位) */
	wkc = len >> 8;
	TLFA1X_outd(P0_RBCR1, wkc);			/* 読み出しバイト数(上位) */

	TLFA1X_outd(Px_CR, CR_dataread);	/* リモートDMAリード開始 */

	for(i = 0; i < end; i++, buff++)
	{
		data = TLFA1X_inw(Px_DATA);		// データポート読み出し
		*buff = swapb(data);			// 上下バイトの入れ替え
	}
}

//________________________________________________________________________________________
//
//	リモートDMAデータ書き込み
//----------------------------------------------------------------------------------------
//	引数
//		adr		書き込み開始アドレス
//		len		書き込むバイト数
//		*src	データ読み出し先
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_remoteDMA_write(int adr, int len, void *src)
{

	int				i, end;
	unsigned short	data, *buff;
	unsigned char	wkc;

	if(len == 0) return;
	buff = (unsigned short *)src;
	end = (len >> 1) + (len & 1);		//奇数バイトの場合は +1 ワード読み出しが必要

	wkc = adr & 0xff;
	TLFA1X_outd(P0_RSAR0, wkc);			/* 読み出し開始アドレス(下位) */
	wkc = adr >> 8;
	TLFA1X_outd(P0_RSAR1, wkc);			/* 読み出し開始アドレス(上位) */

	wkc = len & 0xff;
	TLFA1X_outd(P0_RBCR0, wkc);			/* 読み出しバイト数(下位) */
	wkc = len >> 8;
	TLFA1X_outd(P0_RBCR1, wkc);			/* 読み出しバイト数(上位) */

	TLFA1X_outd(Px_CR,CR_datawrite);	// リモートDMAライト開始

	for(i = 0; i < end; i++, buff++)
	{
		data = swapb(*buff);			// 上下バイト入れ替え取得
		TLFA1X_outw(Px_DATA, data);		// データポート書き込み
	}
}

//________________________________________________________________________________________
//
//	MACアドレス設定(NICレジスタへの設定)
//----------------------------------------------------------------------------------------
//	引数
//		*src	MACアドレスを格納したバッファポインタ
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_SetMACaddress(unsigned char *src)
{
	int i;

	TLFA1X_outd(Px_CR, CR_page1 | (TLFA1X_ind(Px_CR) & 0x3F));			// Regページ1選択
	
	for(i = 0; i < 6; i++, src++)
	{
		TLFA1X_outd(P1_PAR0 + i, *src);		// MACアドレス設定	src[XX:XX:XX:XX:XX:XX]
	}
	TLFA1X_outd(Px_CR, CR_page0 | (TLFA1X_ind(Px_CR) & 0x3F));			// Regページ0選択
}


//________________________________________________________________________________________
//
//	MACアドレス取得(NICレジスタからの取得)
//----------------------------------------------------------------------------------------
//	引数
//		*dst	MACアドレスを格納するバッファポインタ
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_GetMACaddress(unsigned char *dst)
{
	int i;

	TLFA1X_outd(Px_CR, CR_page1 | (TLFA1X_ind(Px_CR) & 0x3F));			// Regページ1選択

	for(i=0;i<6;i++, dst++)
	{
		*dst = TLFA1X_ind(P1_PAR0 + i);		// MACアドレス取得	dst[XX:XX:XX:XX:XX:XX]
	}
	TLFA1X_outd(Px_CR, CR_page0 | (TLFA1X_ind(Px_CR) & 0x3F));			// Regページ0選択
}

//________________________________________________________________________________________
//
//	マルチキャストアドレス設定(NICレジスタへの設定)
//----------------------------------------------------------------------------------------
//	引数
//		*src	マルチキャストアドレス読み出し先バッファ
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_SetMultiCastAaddress(unsigned char *src)
{
	int i;

	TLFA1X_outd(Px_CR, CR_page1 | (TLFA1X_ind(Px_CR) & 0x3F));			// Regページ1選択

	for(i = 0; i < 8; i++, src++)
	{
		TLFA1X_outd(P1_MAR0 + i, *src);		// マルチキャストアドレス設定
	}
	TLFA1X_outd(Px_CR, CR_page0 | (TLFA1X_ind(Px_CR) & 0x3F));			// Regページ0選択
}

//________________________________________________________________________________________
//
//	マルチキャストアドレス取得(NICレジスタからの取得)
//----------------------------------------------------------------------------------------
//	引数
//		*dst	マルチキャストアドレス取得先バッファ
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_GetMultiCastAaddress(unsigned char *dst)
{
	int i;

	TLFA1X_outd(Px_CR, CR_page1 | (TLFA1X_ind(Px_CR) & 0x3F));			// Regページ1選択

	for(i = 0; i < 8; i++, dst++)
	{
		*dst = TLFA1X_ind(P1_MAR0 + i);		// マルチキャストアドレス取得
	}
	TLFA1X_outd(Px_CR, CR_page0 | (TLFA1X_ind(Px_CR) & 0x3F));			// Regページ0選択
}


//________________________________________________________________________________________
//
//	送信部イネーブル/ディセーブル制御
//----------------------------------------------------------------------------------------
//	引数
//		flg		許可フラグ	2:FULL送信許可 / 1:HALF送信許可 / 0:送信禁止
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_SendEnableCtrl(int flg)
{
	if (flg == 2)
	{
		TLFA1X_outd(P0_TCR, TCR_TXFullenable);	/* 送信部イネーブル Full duplex */
	}
	else
	if (flg == 1)
	{
		TLFA1X_outd(P0_TCR, TCR_TXenable);		/* 送信部イネーブル  Half duplex*/
	}
	else
	{
		TLFA1X_outd(P0_TCR, TCR_TXdisable);		/* 送信部ディセーブル */
	}
}


//________________________________________________________________________________________
//
//	受信部イネーブル/ディセーブル制御
//----------------------------------------------------------------------------------------
//	引数
//		flg		許可フラグ	1:受信許可 / 0:受信禁止
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_RecvEnableCtrl(int flg)
{
	if(flg)
	{
		TLFA1X_outd(P0_RCR, AX_RCR_RXenable);		/* 受信部イネーブル */
	}
	else
	{
		TLFA1X_outd(P0_RCR, AX_RCR_RXdisable);		/* 受信部ディセーブル */
	}
}


//________________________________________________________________________________________
//
//	NIC初期化
//----------------------------------------------------------------------------------------
//	引数
//		*mac	マックアドレス
//	戻り
//		int		0：初期化終了 / 他：エラー終了
//________________________________________________________________________________________
//
int		TLFA1_Init(unsigned char *mac)
{
	int				i;
	unsigned char	buff[16];
	unsigned short	wks;
	unsigned char	wkc;
	unsigned char	mmc[2];

	TLFA1X_outd(Px_RESET, 0xff);					/* リセット */
	TThread_sleep(100);								/* リカバリタイム */
	TLFA1X_outd(Px_RESET, 0);						/* リセット */
	TThread_sleep(100);								/* リカバリタイム */
	TLFA1X_outd(P0_IMR, 0);							/* 割り込みマスククリア */
	TLFA1X_outd(P0_IMR, 0);							/* 割り込みマスククリア */

	TThread_sleep(100);								/* リカバリタイム */

	/* EEPROM読み出し */
#if 0
	TLFA1X_outd(Px_CR, CR_page3 | CR_stop);			/* Regページ3選択/動作停止 */
	TLFA1X_outd(P3_REER, 0x01);						/* リロード開始 */
#else
	TLFA1X_outd(P0_RBCR0, 12);
	TLFA1X_outd(P0_RBCR1, 0);
	TLFA1X_outd(P0_RSAR0, 0);
	TLFA1X_outd(P0_RSAR1, 0);
#endif
	TThread_sleep(100);								/* リカバリタイム */

	/* 送受信ディセーブル状態で初期化 */
	TLFA1X_outd(Px_CR, CR_page0 | CR_stop);			/* Regページ0選択/動作停止 */


	TLFA1X_outd(P0_IMR, 0);							/* 割り込みマスククリア */
	TLFA1X_outd(P0_ISR, ISR_AllClr);				/* 割り込み要求クリア */

	TLFA1X_outd(P0_RBCR0, 0);						/* リモートバイトカウントレジスタ0クリア */
	TLFA1X_outd(P0_RBCR1, 0);						/* リモートバイトカウントレジスタ1クリア */

#ifndef BUS_8_BIT
	TLFA1X_outd(P0_DCR, 0x01);						/* 16ビット転送 */
#else
	TLFA1X_outd(P0_DCR, 0x00);						/* 8ビット転送 */
#endif

	TLFA1_SendEnableCtrl(0);						/* 送信部ディセーブル */
	TLFA1_RecvEnableCtrl(0);						/* 受信部ディセーブル */

	wkc = A1_RecvBuff_Start >> 8;					// debug
	TLFA1X_outd(P0_PSTART, wkc);					/* 受信リンクバッファ用先頭アドレス */

	wkc = A1_RecvBuff_End >> 8;						//debug
	TLFA1X_outd(P0_PSTOP,wkc);						/* 受信リンクバッファ用終了アドレス */

	wkc = (A1_RecvBuff_End >> 8) - 1;				// debug
	TLFA1X_outd(P0_BNRY, wkc);						/* 境界ポインタ設定 */

	TLFA1X_outd(Px_CR, CR_page1 + CR_STP + CR_RD2);
	wkc = A1_RecvBuff_Start >> 8;					//debug
	TLFA1X_outd(P1_CURR, wkc);						/* カレントページレジスタ設定 */

	TLFA1X_outd(Px_CR, CR_page0);					/* Regページ0選択 */

#if 0
	TLFA1_GetMACaddress(a1_net_mac_addres);
#else
	TLFA1X_outd(Px_CR, CR_page0 | CR_RD0 | CR_STA);
	for(i = 0; i < 6; i++)
	{
#ifdef BUS_8_BIT
		a1_net_mac_addres[i] = TLFA1X_ind(Px_DATA);
		TLFA1X_ind(Px_DATA);
#else
		a1_net_mac_addres[i] = (unsigned char)TLFA1X_inw(Px_DATA);
#endif
	}

	TLFA1X_outd(Px_CR, CR_page1 | CR_RD2 | CR_STP);
	TLFA1X_outd(P1_PAR0, a1_net_mac_addres[0]);
	TLFA1X_outd(P1_PAR1, a1_net_mac_addres[1]);
	TLFA1X_outd(P1_PAR2, a1_net_mac_addres[2]);
	TLFA1X_outd(P1_PAR3, a1_net_mac_addres[3]);
	TLFA1X_outd(P1_PAR4, a1_net_mac_addres[4]);
	TLFA1X_outd(P1_PAR5, a1_net_mac_addres[5]);
#endif

	mac[0] = a1_net_mac_addres[0];
	mac[1] = a1_net_mac_addres[1];
	mac[2] = a1_net_mac_addres[2];
	mac[3] = a1_net_mac_addres[3];
	mac[4] = a1_net_mac_addres[4];
	mac[5] = a1_net_mac_addres[5];

	//マルチキャストレジスタ設定
	for(i = 0; i < 8; i++)
	{
		buff[i] = 0;	/* マルチキャストアドレス オールクリア */
	}
	TLFA1_SetMultiCastAaddress(buff);

	/* 各種変数初期化 */
	a1_SendBuff_Select = 0;
	a1_SendBuff0_Send = 0;
	a1_SendBuff1_Send = 0;
	a1_RecvPacket_Ready = 0;

	/* データコンフィギュレーションレジスタイネーブル */
#ifndef BUS_8_BIT
	TLFA1X_outd(P0_DCR, 0x01);						/* 16ビット転送 */
#else
	TLFA1X_outd(P0_DCR, 0x00);						/* 8ビット転送 */
#endif

	return 0;
}


//________________________________________________________________________________________
//
//	NIC開始
//----------------------------------------------------------------------------------------
//	引数
//		無し
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_Start(void)
{
	TLFA1X_outd(Px_CR, CR_page0 + CR_STA + CR_RD2);	/* Regページ0選択 */
}	

//________________________________________________________________________________________
//
//	NIC停止
//----------------------------------------------------------------------------------------
//	引数
//		無し
//	戻り
//		無し
//________________________________________________________________________________________
//
void TLFA1_Stop(void)
{
	TLFA1_SendEnableCtrl(0);				/* 送信部ディセーブル */
	TLFA1_RecvEnableCtrl(0);				/* 受信部ディセーブル */
	TLFA1X_outd(Px_CR, CR_stop);			/* 停止 */
}

//________________________________________________________________________________________
//
//	MII 処理		data( 0 or 1)をPHYへ書き出す
//----------------------------------------------------------------------------------------
//	引数
//		data	MII に書き出すデータ(MII_MDO)
//	戻り
//		無し
//________________________________________________________________________________________
//
void TLFA1_miiout(unsigned char data)
{
	TLFA1X_outd(P0_MII, data);				/* MDC clock = low */
	TLFA1X_outd(P0_MII, data | MII_MDCH);		/* MDC clock = high */
	TLFA1X_outd(P0_MII, data);				/* MDC clock = low */
}

//________________________________________________________________________________________
//
//	MII 処理		data( 0 or 1)をPHYから読み込む
//----------------------------------------------------------------------------------------
//	引数
//		無し
//	戻り
//		unsigned char	MII から読み出したデータ(MII_MDI)
//________________________________________________________________________________________
//
unsigned char TLFA1_miiin(void)
{
	unsigned char data, i;
	
	TLFA1X_outd(P0_MII, MII_0);
	TLFA1X_outd(P0_MII, MII_MDCH);
	
	for(i = 0; i < 10; i++){}	//待ち
	
	data = TLFA1X_ind(P0_MII);
	TLFA1X_outd(P0_MII, MII_0);
	
	if(data & MII_MDI) return 1;
	return 0;
}

//________________________________________________________________________________________
//
//	MII 処理		送受信処理
//----------------------------------------------------------------------------------------
//	引数
//		opcode			処理コード
//		reg_addr		レジスタアドレス(0x10:PHY / 他:MR reg.adr.)
//		wdata			書き込みデータ
//	戻り
//		unsigned int	読み出し結果
//________________________________________________________________________________________
//
unsigned int TLFA1_miirw(unsigned char opcode, unsigned char reg_addr, unsigned int wdata)
{
	unsigned char i, phy_add = 0x10;
	unsigned int miidata = 0;
	
	for(i = 0; i < 32; i++)
	{
		TLFA1_miiout(MII_MDO);		/* preamble bit x 32 */
	}

	TLFA1_miiout(MII_0);				/* SFD */
	TLFA1_miiout(MII_MDO);
	
	for(i = 0; i < 2; i++)		/* op code */
	{
		if((opcode >> (1 - i)) & 0x01)
			TLFA1_miiout(MII_MDO);
		else
			TLFA1_miiout(MII_0);
	}
	
	for(i = 0; i < 5; i++)		/* phy add */
	{
		if((phy_add >> (4 - i)) & 0x01)
			TLFA1_miiout(MII_MDO);
		else
			TLFA1_miiout(MII_0);
	}
	
	for(i = 0; i < 5; i++)		/* reg add */
	{
		if((reg_addr >> (4 - i)) & 0x01)
			TLFA1_miiout(MII_MDO);
		else
			TLFA1_miiout(MII_0);
	}
	
	if(opcode == MII_READ)		/* readアクセス */
	{
		i = TLFA1_miiin();			//dummy
		for(i = 0; i < 16; i++)
		{
			miidata = (miidata << 1) | TLFA1_miiin();
		}
	}
	else					/* writeライトアクセス */
	{
		TLFA1_miiout(MII_MDO);
		TLFA1_miiout(MII_0);
		for(i = 0; i < 16; i++)
		{
			if((wdata >> (15 - i)) & 0x01)
				TLFA1_miiout(MII_MDO);
			else
				TLFA1_miiout(MII_0);
		}
	}
	
	return miidata;
}

//________________________________________________________________________________________
//
//	MII 処理		運転モード切替
//----------------------------------------------------------------------------------------
//	引数
//		media		MII のモード設定(0:自動接続再始動 / 1:10M-HALF)
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_Media_Set(int media)
{
	int i;
	
	if(media == 0)		/* Autonego Restart */
	{
		i = TLFA1_miirw(MII_WRITE, 0, 0x1200);
	}
	if(media == 1)		/* 10M-Half */
	{
		i = TLFA1_miirw(MII_WRITE, 0, 0x0000);
	}
}


//________________________________________________________________________________________
//
//	NICステータス取得
//----------------------------------------------------------------------------------------
//	引数
//		無し
//	戻り
//		int		Bit	15	:	'1'=リンク状態  '0'~リンクが切れている
//					...未使用
//					3	:	'1'=100Base-TX Full
//					2	:	'1'=100Base-TX Half
//					1	:	'1'=10Base-T   Full
//				Bit	0	:	'1'=10Base-T   Half
//________________________________________________________________________________________
//
int		TLFA1_GetStatus(void)
{
	return (int)TLFA1X_inw(P0_GPI);
}

//________________________________________________________________________________________
//
//	パケット送信開始命令発行
//----------------------------------------------------------------------------------------
//	引数
//		BuffBase	送信ループバッファページ位置
//		len			送信バイト数
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_SendStart(int BuffBase, int len)
{

	unsigned char	wkc;

	wkc = BuffBase >> 8;
	TLFA1X_outd(P0_TPSR, wkc);					// 送信開始バッファxページアドレス

	wkc = len & 0xff;
	TLFA1X_outd(P0_TBCR0, wkc);					// 送信開始バイト数(下位)
	wkc = len >> 8;
	TLFA1X_outd(P0_TBCR1, wkc);					// 送信開始バイト数(上位)

	TLFA1X_outd(Px_CR, CR_page0 | CR_sendstart);	// Regページ0選択/送信開始
}

//________________________________________________________________________________________
//
//	送信パケット書き込み
//----------------------------------------------------------------------------------------
//	引数
//		*pack	送信パケット
//		len		パケットサイズ
//	戻り
//		int		0:成功 / -1:失敗(バッファ空き無し)
//________________________________________________________________________________________
//
int		TLFA1_SetSendPacket(unsigned char *pack, int len)
{
	if (len < 46)   len = 46;		/* 最小46バイト(ヘッダ&FCS付加で最小64バイト) */
	if (len > 1500) len = 1500;		/* 最大1500バイト(ヘッダ&FCS付加で最大1518バイト) */

	if (a1_SendBuff_Select == 0)
	{
		//------------------------------------
		// 次の送信に送信バッファ0を使う
		//------------------------------------
		if (a1_SendBuff0_Send == 0)
		{
			//------------------------------------
			// 送信バッファ0が空いている
			//------------------------------------
			TLFA1_remoteDMA_write(A1_SendBuff0_Base, len, pack);		/* リモートDMAで送信バッファ0に書き込み */
			a1_SendBuff0_len = len;							/* 送信バッファ0送信バイト数保存 */
			if (a1_SendBuff1_Send == -1)
			{
				//------------------------------------
				// 送信バッファ1が送信中
				//------------------------------------
				a1_SendBuff0_Send = 1;							/* 送信バッファ0送信開始保留 */
			}
			else
			{
				//------------------------------------
				// 送信バッファ0送信
				//------------------------------------
				a1_SendBuff0_Send = -1;						/* 送信バッファ0送信開始 */
				TLFA1_SendStart(A1_SendBuff0_Base, a1_SendBuff0_len);	/* 送信開始制御 */
			}
			a1_SendBuff_Select = 1;			/* 次は送信バッファ1を使う */
			return 0;						/* 送信バッファ格納完了 */
		}
		else
		{
			return -1;						/* 送信バッファが空いていない */
		}

	}
	else
	{
		if (a1_SendBuff1_Send==0)
		{
			//------------------------------------
			// 送信バッファ1が空いている
			//------------------------------------
			TLFA1_remoteDMA_write(A1_SendBuff1_Base, len, pack);	/* リモートDMAで送信バッファ1に書き込み */
			a1_SendBuff1_len = len;			/* 送信バッファ1送信バイト数保存 */
			if (a1_SendBuff0_Send == -1)
			{
				//------------------------------------
				// 送信バッファ0が送信中
				//------------------------------------
				a1_SendBuff1_Send = 1;			/* 送信バッファ1送信開始保留 */
			}
			else
			{
				//------------------------------------
				// 送信バッファ1送信
				//------------------------------------
				a1_SendBuff1_Send = -1;		/* 送信バッファ1送信開始 */
				TLFA1_SendStart(A1_SendBuff1_Base, a1_SendBuff1_len);	/* 送信開始制御 */
			}
			a1_SendBuff_Select = 0;			/* 次は送信バッファ0を使う */
			return 0;						/* 送信バッファ格納完了 */
		}
		else
		{
			return -1;						/* 送信バッファが空いていない */
		}

	}
}

//________________________________________________________________________________________
//
//	送信バッファ空き状態チェック
//----------------------------------------------------------------------------------------
//	引数
//		無し
//	戻り
//		int		0:有り / -1:無し
//________________________________________________________________________________________
//
int		TLFA1_CheckSendBuffer(void)
{
	if (a1_SendBuff_Select == 0)
	{
		/* 次の送信に送信バッファ0を使う */
		if (a1_SendBuff0_Send==0)
		{
			/* 送信バッファ0が空いている */
			return 0;	/* 送信バッファが空いている */
		}
		else
		{
			return -1;	/* 送信バッファが空いていない */
		}
	}
	else
	{	
		/* 次の送信に送信バッファ1を使う */
		if (a1_SendBuff1_Send==0)
		{
			/* 送信バッファ1が空いている */
			return 0;	/* 送信バッファが空いている */
		}
		else
		{
			return -1;	/* 送信バッファが空いていない */
		}
	}
}


//________________________________________________________________________________________
//
//	送信バッファ再送信
//----------------------------------------------------------------------------------------
//	引数
//		無し
//	戻り
//		無し
//	説明
//		パケットの衝突が激しくてハードウェアでの再送処理が16回を超えた場合、送信エラーで終了した
//________________________________________________________________________________________
//
void	TLFA1_SendPacketRetry(void)
{
	TLFA1X_outd(Px_CR, CR_page0 | CR_sendstart);	/* Regページ0選択/送信開始 */
}


//________________________________________________________________________________________
//
//	NIC割り込み初期化
//----------------------------------------------------------------------------------------
//	引数
//		*rx		受信完了通知関数へのポインタ
//		*tx		送信完了通知関数へのポインタ
//		*re		受信エラー通知関数へのポインタ
//		*te		送信エラー通知関数へのポインタ
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_Interrupt_Init(void *rx, void *tx, void *re, void *te)
{
	// パケット受信完了 / パケット送信完了 : 割り込みマスク解除
	a1_net_sned_event_proc = tx;
	a1_net_sned_error_proc = te;
	a1_net_recv_event_proc = rx;
	a1_net_recv_error_proc = re;

	TLFA1X_outd(P0_ISR, TLFA1X_ind(P0_ISR));	// 全ステータスクリア

	// パケット受信完了 / パケット送信完了 / 受信エラー / 送信エラー : 割り込みマスク解除
	TLFA1X_outd(P0_IMR, IMR_ALLE);
}

//________________________________________________________________________________________
//
//	NIC割り込み処理
//----------------------------------------------------------------------------------------
//	引数
//		無し
//	戻り
//		無し
//________________________________________________________________________________________
//
int	TLFA1_Interrupt(void)
{
	int res = 0;
	unsigned char	Status;
	PROC_CALL		proc_call = NULL;
	
	Status = TLFA1X_ind(P0_ISR);	/* 割り込みステータス取得 */
	TLFA1X_outd(P0_ISR, Status);	/* 割り込みステータスクリア */

	//-----------------------------
	//	送信関連割り込み確認
	//-----------------------------
	if (Status&(ISR_PTX | ISR_TXE))
	{
		if (Status & ISR_PTX)
		{
			//-----------------------------
			//	送信完了り込み
			//-----------------------------
			if (a1_SendBuff0_Send == -1)
			{
				/* 送信バッファ0送信中 */
				a1_SendBuff0_Send = 0;								/* 送信バッファ0送信完了 */
				if (a1_SendBuff1_Send == 1)
				{
					/* 送信バッファ1送信保留中 */
					a1_SendBuff1_Send = -1;						/* 送信バッファ1送信開始 */
					TLFA1_SendStart(A1_SendBuff1_Base, a1_SendBuff1_len);	/* 送信開始制御 */
				}
				if(a1_net_sned_event_proc) proc_call = a1_net_sned_event_proc;
			}
			else
			if (a1_SendBuff1_Send == -1)
			{
				a1_SendBuff1_Send = 0;										/* 送信バッファ1送信完了 */
				if (a1_SendBuff0_Send == 1)
				{	
					a1_SendBuff0_Send = -1;									/* 送信バッファ0送信開始 */
					TLFA1_SendStart(A1_SendBuff0_Base, a1_SendBuff0_len);	/* 送信開始制御 */
				}
				if(a1_net_sned_event_proc) proc_call = a1_net_sned_event_proc;
			}
		}
		else
		{
			//-----------------------------
			//	送信エラー割り込み
			//-----------------------------
			if(a1_net_sned_error_proc) proc_call = a1_net_sned_error_proc;
		}
	}
	//-----------------------------
	//	受信関連割り込み確認
	//-----------------------------
	if (Status & (ISR_PRX | ISR_RXE))
	{
		if (Status & ISR_PRX)
		{
			//-----------------------------
			//	受信完了り込み
			//-----------------------------
			a1_RecvPacket_Ready++;
			if(a1_net_recv_event_proc) proc_call = a1_net_recv_event_proc;
			res = 1;
		}
		else
		{
			//-----------------------------
			//	受信エラー割り込み
			//-----------------------------
			if(a1_net_recv_error_proc) proc_call = a1_net_recv_error_proc;
		}
	}
	if(proc_call) proc_call();

	return res;
}


//________________________________________________________________________________________
//
//	送信パケットチェック
//----------------------------------------------------------------------------------------
//	引数
//		無し
//	戻り
//		無し
//________________________________________________________________________________________
//
void	TLFA1_CheckSentPacket(void)
{
	unsigned char wkc;
	
	wkc = TLFA1X_ind(P0_ISR);

	if(wkc & ISR_PTX)
	{
		TLFA1X_outd(P0_ISR, ISR_PTX);						// 送信完了フラグ解除
		if(a1_SendBuff0_Send == -1) a1_SendBuff0_Send = 0;	// Buffer0:送信完了
		if(a1_SendBuff1_Send == -1) a1_SendBuff1_Send = 0;	// Buffer1:送信完了
	}
}

//________________________________________________________________________________________
//
//	受信パケット有無チェック
//----------------------------------------------------------------------------------------
//	引数
//		無し
//	戻り
//		int		0:無し / 他:有り
//________________________________________________________________________________________
//
int		TLFA1_CheckRecvPacket(void)
{
	return a1_RecvPacket_Ready;
}


//________________________________________________________________________________________
//
//	受信パケット読み出し
//----------------------------------------------------------------------------------------
//	引数
//		*pack	パケット転送先バッファ
//	戻り
//		int		パケットサイズ
//________________________________________________________________________________________
//
int		TLFA1_GetRecvPacket(unsigned char *pack, int length)
{
	unsigned char	CURR,BNRY, work[4];
	unsigned short	StartAdr, len;

	if(a1_RecvPacket_Ready == 0) return 0;		// 受信データなし

	TLFA1X_outd(Px_CR, CR_page1);				/* Regページ1選択 */
	CURR = TLFA1X_ind(P1_CURR);					/* カレントページレジスタ */
	TLFA1X_outd(Px_CR, CR_page0);				/* Regページ0選択 */

	BNRY = TLFA1X_ind(P0_BNRY);					/* 境界ポインタページレジスタ */

	if(((CURR == (A1_RecvBuff_Start >> 8)) && (BNRY == (A1_RecvBuff_End >> 8) - 1)) || ((BNRY + 1) == CURR))
	{
		a1_RecvPacket_Ready = 0;				/* 受信パケット取り出し完了 */
		return 0;								/* 受信パケットデータなし */
	}
	else
	if((BNRY < 0x4C) || (BNRY > 0x80))
	{
		TLFA1_Init(a1_net_mac_addres);
		a1_RecvPacket_Ready = 0;				/* 受信パケット取り出し完了 */
		return 0;								/* 受信パケットデータなし */
	}

	/* 受信バッファRAMからパケットデータ取り出し */
	/* 取り出しバッファ先頭アドレス計算 */
	if (BNRY == (A1_RecvBuff_End >> 8) - 1)
	{
		StartAdr = A1_RecvBuff_Start;
	}
	else
	{
		StartAdr = (BNRY + 1) << 8;
	}

	TLFA1_remoteDMA_read(StartAdr, 4, work);	/* 先頭4バイト読み出し */
	len = (work[2] | (work[3] << 8)) - 4;		/* 残りのバイト数 */

	if(0 == pack)
	{
		return len;
	}

	TLFA1_remoteDMA_read(StartAdr + 4, len, pack);	/* 実パケットデータ部読み出し */

	BNRY = work[1] - 1;								/* 次の受信バッファ(カレントページ)の手前に設定 */

	if (BNRY == (A1_RecvBuff_Start >> 8) - 1)
	{
		BNRY = (A1_RecvBuff_End >> 8) - 1;
	}
	TLFA1X_outd(P0_BNRY, BNRY);						/* 新境界ポインタ位置に書き換え */

	if (((CURR == (A1_RecvBuff_Start >> 8)) && (BNRY == (A1_RecvBuff_End >> 8) - 1)) || ((BNRY + 1) == CURR))
	{
		a1_RecvPacket_Ready = 0;	/* 受信パケット取り出し完了 */
	}
	/* ↑この条件が成立しないときは、複数パケットが受信バッファRAMに格納されている場合 */
	/* よってa1_RecvPacket_Readyをクリアせずに1パケット分の処理をするためにこの関数を抜ける */

	return a1_RecvPacket_Ready;
}

//________________________________________________________________________________________
//
//	シリアルEEPROM オペコードと使用法
//________________________________________________________________________________________
//

#define		AT93C46_READ		0x180	/* 読み出し 		or ADDRESS						*/
#define		AT93C46_EWEN		0x130	/* 書き込み許可										*/
#define		AT93C46_WRITE		0x140	/* 書き込み			or ADDRESS	+ Data bit 15..0	*/
#define		AT93C46_ERAL		0x120	/* 全消去											*/
#define		AT93C46_WRAL		0x110	/* 全書き込み		+ Data bit 15..0				*/
#define		AT93C46_EWDS		0x100	/* 全書き込み禁止									*/

#define		AT93C46_CMDBITS		8		/* コマンドはStartBit(1) + 8bit長					*/

#define		AT93C46_EECK		0x80	/* Sift Clock (P0_MII) out */
#define		AT93C46_EEDO		0x40	/* Sift Clock (P0_MII) in  */
#define		AT93C46_EEDI		0x20	/* Sift Clock (P0_MII) out */
#define		AT93C46_EECS		0x10	/* Sift Clock (P0_MII) out */
#define		AT93C46_EEDIR		0x02	/* Sift Clock (P0_MII) in = 1 / out = 0 */


/*

読み出し手順
		        _   _   _   _   _   _   _   _   _____   _   _   _   _   _   _   _       _   _   _   _       
EECK	_______| |_| |_| |_| |_| |_| |_| |_| |_|     |_| |_| |_| |_| |_| |_| |_| |_|||_| |_| |_| |_| |______

		                 |        Address        |
		      _______     ___ ___ ___ ___ ___ ___ ______________________________________________________    
EEDI	_____| 1   1 |_0_|_5_X_4_X_3_X_2_X_1_X_0_/                                                      |___

		                                              |                Data Bit 15 .. 0               |
		                                               ___ ___ ___ ___ ___ ___ ___     ___ ___ ___ ___      
EEDO	----------------------------------------x_____/_5_X_4_X_3_X_2_X_1_X_0_X_9_X---X_3_X_2_X_1_X_0_X-----
		      __________________________________________________________________________________________    
EECS	_____|                                                                                          |___



書き込み手順
		        _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _         _   _   _   _     
EECK	_______| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_______| |_| |_| |_| |____

		                 |        Address        |         Data Bit 15 .. 0      |
		      ___     ___ ___ ___ ___ ___ ___ ___ ___ ___ ___     ___ ___ ___ ___       ________________    
EEDI	_____| 1 |_0_| 1 |_5_X_4_X_3_X_2_X_1_X_0_X_5_X_4_X_3_X---X_3_X_2_X_1_X_0_|_____|                |___
		                                                                                      __________
EEDO	-------------------------------------------------------------------------------|_____/  Ready   |---
		      _____________________________________________________________________     ________________    
EECS	_____|                                                                     |___|                |___

*/
//________________________________________________________________________________________
//							  __
//	シリアルEEPROM クロック__↑↓__出力
//----------------------------------------------------------------------------------------
//	引数
//		port	直前のポートイメージ(MII/EEPROM adr = 0x14)
//	戻り
//		int		読み出したビット状態(0 / 1)
//________________________________________________________________________________________
//
int	TLFA1_pulse_EEP(unsigned char port)
{
	unsigned char	dat;

	TLFA1X_outd(P0_EEPROM, port);	// port setting
	TThread_loop();	//遅延

	port |= AT93C46_EECK;
	TLFA1X_outd(P0_EEPROM, port);	// ↑
	TThread_loop();	//遅延

	port &= ~AT93C46_EECK;
	dat = TLFA1X_ind(P0_EEPROM);
	TLFA1X_outd(P0_EEPROM, port);	// ↓
	if(dat & AT93C46_EEDO) return 1;
	return 0;
}

//________________________________________________________________________________________
//
//	シリアルEEPROM 1ワード読み出し
//----------------------------------------------------------------------------------------
//	引数
//		addr	読み出しアドレス
//		*data	格納先
//	戻り
//		int		0：成功　/　他：失敗
//________________________________________________________________________________________
//

int TLFA1_get_eep(unsigned short addr,		/* EEPROM WORD ADDRESS */
				  unsigned short *data)		/* 読み出したデータ */
{
	int i;
	unsigned char port;

	// EEPROM word address defined by the POINTER REGISTER 
	TLFA1X_outd(Px_CR, CR_page0);							// Regページ0選択
	port = (TLFA1X_ind(P0_EEPROM) & 0x0D) | (AT93C46_EEDI | AT93C46_EECS);	// EECS = H / EEDI = H / EEDIR = L(out)
	addr = AT93C46_READ | (addr & 0x3F);
	for(i = 0; i < 9; i++){
		if(addr & 0x100){
			port |= AT93C46_EEDI;
		}else{
			port &= ~AT93C46_EEDI;
		}
		addr <<= 1;
		TLFA1_pulse_EEP(port);
	}
	port |= AT93C46_EEDIR;			//リードモード
	TLFA1X_outd(P0_EEPROM, port);	// port setting
	for(i = 0; i < 16; i++){
		*data <<= 1;
		if(TLFA1_pulse_EEP(port)){
			*data |= 1;
		}
	}
	port = TLFA1X_ind(P0_EEPROM) & 0x0F;
	TLFA1X_outd(P0_EEPROM, port);
	TThread_loop();
	port = TLFA1X_ind(P0_EEPROM) & 0x0D;
	TLFA1X_outd(P0_EEPROM, port);
	return 0;
}

//________________________________________________________________________________________
//
//	シリアルEEPROM 1ワード書き込み
//----------------------------------------------------------------------------------------
//	引数
//		addr	読み出しアドレス
//		data	書き込むデータ
//	戻り
//		int		0：成功　/　他：失敗
//________________________________________________________________________________________
//

int TLFA1_set_eep(unsigned short addr,	/* EEPROM WORD ADDRESS */
				  unsigned short data)	/* 書き込むデータ */
{
	int i;
	unsigned char port;
	unsigned short flg;

	// EEPROM word address defined by the POINTER REGISTER 
	TLFA1X_outd(Px_CR, CR_page0);							// Regページ0選択

	//プロテクトOFF
	port = (TLFA1X_ind(P0_EEPROM) & 0x0D) | (AT93C46_EEDI | AT93C46_EECS);	// EECS = H / EEDI = H / EEDIR = L(out)
	flg = AT93C46_EWEN;
	for(i = 0; i < 9; i++){
		if(flg & 0x100){
			port |= AT93C46_EEDI;
		}else{
			port &= ~AT93C46_EEDI;
		}
		flg <<= 1;
		TLFA1_pulse_EEP(port);
	}
	port = TLFA1X_ind(P0_EEPROM) & 0x0F;
	TLFA1X_outd(P0_EEPROM, port);
	TThread_loop();
	port = TLFA1X_ind(P0_EEPROM) & 0x0D;
	TLFA1X_outd(P0_EEPROM, port);

	//データ書き込み
	TThread_loop();
	port = (TLFA1X_ind(P0_EEPROM) & 0x0D) | (AT93C46_EEDI | AT93C46_EECS);	// EECS = H / EEDI = H / EEDIR = L(out)
	addr = AT93C46_WRITE | (addr & 0x3F);
	for(i = 0; i < 9; i++){
		if(addr & 0x100){
			port |= AT93C46_EEDI;
		}else{
			port &= ~AT93C46_EEDI;
		}
		addr <<= 1;
		TLFA1_pulse_EEP(port);
	}
	for(i = 0; i < 16; i++){
		if(data & 0x8000){
			port |= AT93C46_EEDI;
		}else{
			port &= ~AT93C46_EEDI;
		}
		TLFA1_pulse_EEP(port);
		data <<= 1;
	}
	port = TLFA1X_ind(P0_EEPROM) & 0x0D;
	TLFA1X_outd(P0_EEPROM, port);
	TThread_loop();

	//書き込み完了待ち
	port = (TLFA1X_ind(P0_EEPROM) & 0x0D) | (AT93C46_EECS | AT93C46_EEDIR);			//リードモード
	TLFA1X_outd(P0_EEPROM, port);	// port setting
	for(i = 0; i < 100; i++){
		if(TLFA1_pulse_EEP(port) == 0) break;
	}
	for(i = 0; i < 10000; i++){
		if(TLFA1_pulse_EEP(port) != 0) break;
	}
	port = TLFA1X_ind(P0_EEPROM) & 0x0F;
	TLFA1X_outd(P0_EEPROM, port);
	TThread_loop();
	port = TLFA1X_ind(P0_EEPROM) & 0x0D;
	TLFA1X_outd(P0_EEPROM, port);

	//プロテクトON
	TThread_loop();
	port = (TLFA1X_ind(P0_EEPROM) & 0x0D) | (AT93C46_EEDI | AT93C46_EECS);	// EECS = H / EEDI = H / EEDIR = L(out)
	flg = AT93C46_EWDS;
	for(i = 0; i < 9; i++){
		if(flg & 0x100){
			port |= AT93C46_EEDI;
		}else{
			port &= ~AT93C46_EEDI;
		}
		flg <<= 1;
		TLFA1_pulse_EEP(port);
	}
	port = TLFA1X_ind(P0_EEPROM) & 0x0F;
	TLFA1X_outd(P0_EEPROM, port);
	TThread_loop();
	port = TLFA1X_ind(P0_EEPROM) & 0x0D;
	TLFA1X_outd(P0_EEPROM, port);

	return 0;
}

//________________________________________________________________________________________
//
//	MACアドレス読み出し
//----------------------------------------------------------------------------------------
//	引数
//		*macaddr	格納先
//	戻り
//		int			0：成功　/　他：失敗
//________________________________________________________________________________________
//

int TLFA1_get_mac(unsigned char *macaddr)
{
	unsigned short d[3];

	if ((TLFA1_get_eep(0x20, &d[0]) != 0)
	 || (TLFA1_get_eep(0x21, &d[1]) != 0)
	 || (TLFA1_get_eep(0x22, &d[2]) != 0))
		return 1;

	macaddr[0] = (unsigned char)d[0];
	macaddr[1] = (unsigned char)(d[0] >> 8);
	macaddr[2] = (unsigned char)d[1];
	macaddr[3] = (unsigned char)(d[1] >> 8);
	macaddr[4] = (unsigned char)d[2];
	macaddr[5] = (unsigned char)(d[2] >> 8);
	return 0;
}

//________________________________________________________________________________________
//
//	MACアドレス書き込み
//----------------------------------------------------------------------------------------
//	引数
//		*macaddr	MACアドレス
//	戻り
//		int			0：成功　/　他：失敗
//________________________________________________________________________________________
//

int TLFA1_set_mac(unsigned char *macaddr)
{
	unsigned short d[3];

	d[0] = macaddr[0] | ((unsigned short)macaddr[1] << 8);
	d[1] = macaddr[2] | ((unsigned short)macaddr[3] << 8);
	d[2] = macaddr[4] | ((unsigned short)macaddr[5] << 8);

	if ((TLFA1_set_eep(0x20, d[0]) != 0)
	 || (TLFA1_set_eep(0x21, d[1]) != 0)
	 || (TLFA1_set_eep(0x22, d[2]) != 0))
		return 1;
	return 0;
}


void TLFA1_error_reset(void)
{
	TLFA1_Stop();
	TLFA1X_outd(Px_RESET, 0xff);
	TLFA1X_outd(Px_RESET, 0);
	TLFA1_SendEnableCtrl(1);
	TLFA1_RecvEnableCtrl(1);
	TLFA1_Start();
	return;
}
