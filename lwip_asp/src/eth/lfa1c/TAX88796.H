//________________________________________________________________________________________
//
//	AX88796 EtherNET LAN サンプルヘッダ
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

//#include	"TAX88796.H"					/* AX88796 定義 */

//________________________________________________________________________________________
//
//	定数定義
//________________________________________________________________________________________
//

#ifndef		__ETHER_NET_AX88796__
#define		__ETHER_NET_AX88796__

//----------------------------------------------------------------------------------------
//	ページ共通 レジスタ名称
//----------------------------------------------------------------------------------------
#define		Px_CR				0x00
#define		Px_DATA				0x10
#define		Px_RESET			0x1f

//----------------------------------------------------------------------------------------
//	ページ0 レジスタ名称
//----------------------------------------------------------------------------------------
#define		P0_CLDA0			0x01
#define		P0_PSTART			0x01
#define		P0_CLDA1			0x02
#define		P0_PSTOP			0x02
#define		P0_BNRY				0x03
#define		P0_TSR				0x04
#define		P0_TPSR				0x04
#define		P0_NCR				0x05
#define		P0_TBCR0			0x05
#define		P0_FIFO				0x06
#define		P0_TBCR1			0x06
#define		P0_ISR				0x07
#define		P0_CRDA0			0x08
#define		P0_RSAR0			0x08
#define		P0_CRDA1			0x09
#define		P0_RSAR1			0x09
#define		P0_RBCR0			0x0A
#define		P0_RBCR1			0x0B
#define		P0_RSR				0x0C
#define		P0_RCR				0x0C
#define		P0_CNTR0			0x0D
#define		P0_TCR				0x0D
#define		P0_CNTR1			0x0E
#define		P0_DCR				0x0E
#define		P0_CNTR2			0x0F
#define		P0_IMR				0x0F
#define		P0_MII				0x14
#define		P0_EEPROM			0x14
#define		P0_BTCR				0x15
#define		P0_GPI				0x17

//----------------------------------------------------------------------------------------
//	ページ1 レジスタ名称
//----------------------------------------------------------------------------------------
#define		P1_PAR0				0x01
#define		P1_PAR1				0x02
#define		P1_PAR2				0x03
#define		P1_PAR3				0x04
#define		P1_PAR4				0x05
#define		P1_PAR5				0x06
#define		P1_CURR				0x07
#define		P1_MAR0				0x08
#define		P1_MAR1				0x09
#define		P1_MAR2				0x0A
#define		P1_MAR3				0x0B
#define		P1_MAR4				0x0C
#define		P1_MAR5				0x0D
#define		P1_MAR6				0x0E
#define		P1_MAR7				0x0F

//----------------------------------------------------------------------------------------
//	ページ2 レジスタ名称
//----------------------------------------------------------------------------------------
#define		P2_PSTART			0x01
#define		P2_CLDA0			0x01
#define		P2_PSTOP			0x02
#define		P2_CLDA1			0x02
#define		P2_RNPP				0x03	/* レジスタ名要確認 */
#define		P2_TPSR				0x04
#define		P2_LNPP				0x05	/* レジスタ名要確認 */
#define		P2_AC0				0x06	/* レジスタ名要確認 */
#define		P2_AC1				0x07	/* レジスタ名要確認 */
#define		P2_RCR				0x0C
#define		P2_TCR				0x0D
#define		P2_DCR				0x0E
#define		P2_IMR				0x0F

//----------------------------------------------------------------------------------------
//	ページ3 レジスタ名称
//----------------------------------------------------------------------------------------
#define		P3_WFBM0			0x01
#define		P3_WFBM1			0x02
#define		P3_WFBM2			0x03
#define		P3_WFBM3			0x04
#define		P3_WF10CRC			0x05
#define		P3_WF32CRC			0x06
#define		P3_WFOFST			0x07
#define		P3_WFLB				0x08
#define		P3_WFCMD			0x09
#define		P3_WUCSR			0x0A
#define		P3_PMR				0x0B
#define		P3_REER				0x0C	/* 0x01 を書くとEEPROMから再読込を行う */
#define		P3_MISC				0x0D
#define		P3_GPT0				0x0E
#define		P3_GPT1				0x0F

//----------------------------------------------------------------------------------------
//	コマンドレジスタ(CR)制御ビット
//----------------------------------------------------------------------------------------
#define		CR_STP				0x01
#define		CR_STA				0x02
#define		CR_TXP				0x04
#define		CR_RD0				0x08
#define		CR_RD1				0x10
#define		CR_RD2				0x20
#define		CR_PS0				0x40
#define		CR_PS1				0x80
#define		CR_stop				0x21	/* 動作停止 */
#define		CR_sendstart		0x26	/* 送信開始 */
#define		CR_dataread			0x0A	/* リモートDMAリード開始 */
#define		CR_datawrite		0x12	/* リモートDMAライト開始 */
#define		CR_page0			0x00	/* ページ0選択時 */
#define		CR_page1			0x40	/* ページ1選択時 */
#define		CR_page2			0x80	/* ページ2選択時 */
#define		CR_page3			0xC0	/* ページ3選択時 */


//----------------------------------------------------------------------------------------
//	割り込みマスクレジスタ(IMR)制御ビット
//----------------------------------------------------------------------------------------
#define		IMR_NONE			0x00
#define		IMR_PRXE			0x01
#define		IMR_PTXE			0x02
#define		IMR_RXEE			0x04
#define		IMR_TXEE			0x08
#define		IMR_OVWE			0x10
#define		IMR_CNTE			0x20
#define		IMR_RDCE			0x40
#define		IMR_ALLE			0x7F

//----------------------------------------------------------------------------------------
//	割り込みステータスレジスタ(ISR)制御ビット
//----------------------------------------------------------------------------------------
#define		ISR_PRX				0x01
#define		ISR_PTX				0x02
#define		ISR_RXE				0x04
#define		ISR_TXE				0x08
#define		ISR_OVW				0x10
#define		ISR_CNT				0x20
#define		ISR_RDC				0x40
#define		ISR_RST				0x80
#define		ISR_AllClr			0xFF	/* ステータス全クリア */

//----------------------------------------------------------------------------------------
//	データコンフィギュレーションレジスタ(DCR)制御ビット
//----------------------------------------------------------------------------------------
#define		DCR_WTS				0x01
#define		DCR_BOS				0x02
#define		DCR_LAS				0x04
#define		DCR_LS				0x08
#define		DCR_AR				0x10
#define		DCR_FT0				0x20
#define		DCR_FT1				0x40
#define		DCR_normal			0x08	/* 通常動作時 */
#define		DCR_loop			0x00	/* ループバック時 */
#define		DCR_fifo2			0x00	/* FIFOスレッショルド設定 2バイト */
#define		DCR_fifo4			0x20	/* FIFOスレッショルド設定 4バイト */
#define		DCR_fifo8			0x40	/* FIFOスレッショルド設定 8バイト */
#define		DCR_fifo12			0x60	/* FIFOスレッショルド設定12バイト */

//----------------------------------------------------------------------------------------
//	送信コンフィギュレーションレジスタ(TCR)制御ビット
//----------------------------------------------------------------------------------------
#define		TCR_CRC				0x01
#define		TCR_LB0				0x02
#define		TCR_LB1				0x04
#define		TCR_ATD				0x08
#define		TCR_OFST			0x10
#define		TCR_CRCon			0x00	/* CRCを付加する */
#define		TCR_CRCoff			0x01	/* CRCを付加しない */
#define		TCR_normal			0x00	/* 通常動作時 */
#define		TCR_loop1			0x02	/* NIC内部ループバック時 */
#define		TCR_loop2			0x04	/* ENDECループバック時 */
#define		TCR_loop3			0x06	/* 外部ループバック時 */
#define		TCR_TXFullenable	0x80	/* 送信部イネーブル(通常動作時 Full Duplex) */
#define		TCR_TXenable		0x00	/* 送信部イネーブル(通常動作時 Half Duplex) */
#define		TCR_TXdisable		0x02	/* 送信部ディセーブル(NIC内部ループバック時) */

//----------------------------------------------------------------------------------------
//	送信ステータスレジスタ(TSR)制御ビット
//----------------------------------------------------------------------------------------
#define		TSR_PTX				0x01
#define		TSR_COL				0x04
#define		TSR_ABT				0x08
#define		TSR_CRS				0x10
#define		TSR_FU				0x20
#define		TSR_CHD				0x40
#define		TSR_OWC				0x80

//----------------------------------------------------------------------------------------
//	受信コンフィギュレーションレジスタ(RCR)制御ビット
//----------------------------------------------------------------------------------------
#define		RCR_SEP				0x01
#define		RCR_AR				0x02
#define		RCR_AB				0x04
#define		RCR_AM				0x08
#define		RCR_PRO				0x10
#define		RCR_MON				0x20
#define		AX_RCR_RXenable		0x4c	/* 受信部イネーブル(自分宛&ブロードキャストパケット受信 */
#define		RCR_RXenable		0x04	/* 受信部イネーブル(自分宛&ブロードキャストパケット受信 */
#define		AX_RCR_RXdisable	0x60	/* 受信部ディセーブル(モニタモード) */
#define		RCR_RXdisable		0x20	/* 受信部ディセーブル(モニタモード) */

//----------------------------------------------------------------------------------------
//	受信ステータスレジスタ(RSR)制御ビット
//----------------------------------------------------------------------------------------
#define		RSR_PRX				0x01
#define		RSR_CRC				0x02
#define		RSR_FAE				0x04
#define		RSR_FO				0x08
#define		RSR_MPA				0x10
#define		RSR_PHY				0x20
#define		RSR_DIS				0x40
#define		RSR_DFR				0x80

#define		MII_MDO				0x08
#define		MII_MDI				0x04
#define		MII_MDIR			0x02
#define		MII_MDCH			0x01
#define		MII_0				0x00
#define		MII_READ			0x02
#define		MII_WRITE			0x01

#endif		/*__ETHER_NET_AX88796__*/
