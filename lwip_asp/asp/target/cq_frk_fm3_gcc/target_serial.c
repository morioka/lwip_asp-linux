/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 */

/*
 *  シリアルI/Oデバイス（SIO）ドライバ（cq_frk_fm3用）
 */

#include <kernel.h>
#include <t_syslog.h>
#include "target_serial.h"
#include "target_syssvc.h"

/*
 * レジスタ設定値
 */
#define PORT2SIOPID(x)	((x) + 1)
#define INDEX_PORT(x)	((x) - 1)
#define GET_SIOPCB(x)	(&siopcb_table[INDEX_PORT(x)])

/*
 * UARTレジスタ定義
 */
#define UART_SMR(x)		(x + 0x00)
#define UART_SCR(x)		(x + 0x01)
#define UART_ESCR(x)	(x + 0x04)
#define UART_SSR(x)		(x + 0x05)
#define UART_RDR(x)		(x + 0x08)
#define UART_TDR(x)		(x + 0x08)
#define UART_BGR(x)		(x + 0x0c)
#define UART_BGR0(x)	(x + 0x0c)
#define UART_BGR1(x)	(x + 0x0d)
#define UART_ISBA(x)	(x + 0x10)
#define UART_ISMK(x)	(x + 0x11)
#define UART_FCR0(x)	(x + 0x14)
#define UART_FCR1(x)	(x + 0x15)
#define UART_FBYTE1(x)	(x + 0x18)
#define UART_FBYTE2(x)	(x + 0x19)

/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block {
	uint32_t base;
	INTNO    intno_rx;
	INTNO    intno_tx;
} SIOPINIB;

/*
 *  兼用GPIOポート初期化ブロックの定義
 */
typedef struct gpio_port_initialization_block {
	uint32_t pfr;		/* PFRxレジスタアドレス */
	uint32_t pfr_set;	/* PFRxレジスタをセットするビット */
	uint32_t pcr;		/* PCRxレジスタアドレス */
	uint32_t pcr_set;	/* PCRxレジスタをセットするビット */
	uint32_t epfr;		/* EPFRxxレジスタアドレス */
	uint32_t epfr_clr;	/* EPFRxxレジスタをクリアするビット */
	uint32_t epfr_set;	/* EPFRxxレジスタをセットするビット */
	uint32_t ade;		/* ADEレジスタアドレス */
	uint32_t ade_clr;	/* ADEレジスタをクリアするビット */
} GPIOINIB;

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
struct sio_port_control_block {
	const SIOPINIB  *p_siopinib;  /* シリアルI/Oポート初期化ブロック */
	const GPIOINIB  *p_gpioinib;  /* 兼用GPIOポート初期化ブロック */
	intptr_t        exinf;        /* 拡張情報 */
	bool_t          opnflg;       /* オープン済みフラグ */
};

/*
 * シリアルI/Oポート初期化ブロック
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
	{(uint32_t)FM3_MFS4_UART_BASE, (INTNO)IRQ_VECTOR_MFS4RX, (INTNO)IRQ_VECTOR_MFS4TX},
	{(uint32_t)FM3_MFS0_UART_BASE, (INTNO)IRQ_VECTOR_MFS0RX, (INTNO)IRQ_VECTOR_MFS0TX},
	{(uint32_t)FM3_MFS3_UART_BASE, (INTNO)IRQ_VECTOR_MFS3RX, (INTNO)IRQ_VECTOR_MFS3TX},
};

/*
 * 兼用GPIOポート初期化ブロック
 */
const GPIOINIB gpioinib_table[TNUM_SIOP] = {
	{(uint32_t)FM3_GPIO_PFR0, (uint32_t)((1 << 5) | (1 << 6)), (uint32_t)FM3_GPIO_PCR0, (uint32_t)(1<<5), (uint32_t)FM3_GPIO_EPFR08, (uint32_t)~0x000000f0, (uint32_t)0x000000f0, (uint32_t)FM3_GPIO_ADE, (uint32_t)0},
	{(uint32_t)FM3_GPIO_PFR2, (uint32_t)((1 << 1) | (1 << 2)), (uint32_t)FM3_GPIO_PCR2, (uint32_t)(1<<1), (uint32_t)FM3_GPIO_EPFR07, (uint32_t)~0x000000f0, (uint32_t)0x00000050, (uint32_t)FM3_GPIO_ADE, (uint32_t)(1 << 31)},
	{(uint32_t)FM3_GPIO_PFR4, (uint32_t)((1 << 8) | (1 << 9)), (uint32_t)FM3_GPIO_PCR4, (uint32_t)(1<<8), (uint32_t)FM3_GPIO_EPFR07, (uint32_t)~0x03c00000, (uint32_t)0x03c00000, (uint32_t)FM3_GPIO_ADE, (uint32_t)0},
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB	siopcb_table[TNUM_SIOP];

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)	((uint_t)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

/*
 *  SIOドライバの初期化
 */
void
sio_initialize(intptr_t exinf)
{
	SIOPCB	*p_siopcb;
	uint_t	i;

	/*
	 *  シリアルI/Oポート管理ブロックの初期化
	 */
	for (p_siopcb = siopcb_table, i = 0; i < TNUM_SIOP; p_siopcb++, i++) {
		p_siopcb->p_siopinib = &(siopinib_table[i]);
		p_siopcb->p_gpioinib = &(gpioinib_table[i]);
		p_siopcb->opnflg = false;
	}
}


/*
 *  シリアルI/Oポートのオープン
 */
SIOPCB *
sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB          *p_siopcb;
	const SIOPINIB  *p_siopinib;
	const GPIOINIB  *p_gpioinib;
	bool_t  opnflg;
	ER      ercd;

	p_siopcb = get_siopcb(siopid);
	p_siopinib = p_siopcb->p_siopinib;
	p_gpioinib = p_siopcb->p_gpioinib;

	/*
	 *  オープンしたポートがあるかをopnflgに読んでおく．
	 */
	opnflg = p_siopcb->opnflg;

	p_siopcb->exinf = exinf;

	/*
	 *  ハードウェアの初期化
	 */
	sil_wrw_mem((uint32_t *)p_gpioinib->pfr, sil_rew_mem((uint32_t *)p_gpioinib->pfr) | p_gpioinib->pfr_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->pcr, sil_rew_mem((uint32_t *)p_gpioinib->pcr) | p_gpioinib->pcr_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->epfr, (sil_rew_mem((uint32_t *)p_gpioinib->epfr) & p_gpioinib->epfr_clr) | p_gpioinib->epfr_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->ade, sil_rew_mem((uint32_t *)p_gpioinib->ade) & ~p_gpioinib->ade_clr);

	uint32_t base = p_siopinib->base;

	sil_wrb_mem((uint8_t *)UART_SCR(base), 0);
	sil_wrb_mem((uint8_t *)UART_SMR(base), SMR_MD_UART | SMR_SOE);
	sil_wrh_mem((uint16_t *)UART_BGR(base), (uint16_t)(((SysFrePCLK2 + ((uint32_t)BPS_SETTING / 2)) / (uint32_t)BPS_SETTING) - 1));
	sil_wrb_mem((uint8_t *)UART_ESCR(base), ESCR_DATABITS_8);
	sil_wrb_mem((uint8_t *)UART_SCR(base), SCR_RXE | SCR_TXE);

	/*
	 *  シリアルI/O割込みのマスクを解除する．
	 */
	if (!opnflg) {
		ercd = ena_int(p_siopinib->intno_rx);
		assert(ercd == E_OK);
		ercd = ena_int(p_siopinib->intno_tx);
		assert(ercd == E_OK);
	}

	return(p_siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
sio_cls_por(SIOPCB *p_siopcb)
{
	/*
	 *  シリアルI/O割込みをマスクする．
	 */
	if (!(p_siopcb->opnflg)) {
		dis_int(p_siopcb->p_siopinib->intno_rx);
		dis_int(p_siopcb->p_siopinib->intno_tx);
	}
}

/*
 *  SIOの割込みサービスルーチン
 */

Inline bool_t
sio_putready(SIOPCB* p_siopcb)
{
	return (sil_reb_mem((uint8_t *)UART_SSR(p_siopcb->p_siopinib->base)) & SSR_TDRE) != 0;
}

Inline bool_t
sio_getready(SIOPCB* p_siopcb)
{
	char_t ssr = sil_reb_mem((uint8_t *)UART_SSR(p_siopcb->p_siopinib->base));

	if ((ssr & (SSR_ORE | SSR_FRE | SSR_PE)) != 0)
	{
		sil_wrb_mem((uint8_t *)UART_SSR(p_siopcb->p_siopinib->base), ssr | SSR_REC);
		return 0;
	}
	if ((ssr & SSR_RDRF) != 0)
	{
		return 1;
	}
	return 0;
}

void
sio_tx_isr(intptr_t exinf)
{
	SIOPCB          *p_siopcb;

	p_siopcb = get_siopcb(exinf);

	if (sio_putready(p_siopcb)) {
		sio_irdy_snd(p_siopcb->exinf);
	}
}

void
sio_rx_isr(intptr_t exinf)
{
	SIOPCB          *p_siopcb;

	p_siopcb = get_siopcb(exinf);

	if (sio_getready(p_siopcb)) {
		sio_irdy_rcv(p_siopcb->exinf);
	}
}

/*
 *  シリアルI/Oポートへの文字送信
 */
bool_t
sio_snd_chr(SIOPCB *p_siopcb, char c)
{
	if (sio_putready(p_siopcb)) {
		sil_wrh_mem((uint16_t *)UART_TDR(p_siopcb->p_siopinib->base), (uint16_t)c);

		return true;
	}

	return false;
}

/*
 *  シリアルI/Oポートからの文字受信
 */
int_t
sio_rcv_chr(SIOPCB *p_siopcb)
{
	int_t c = -1;

	if (sio_getready(p_siopcb)) {
		c = sil_reh_mem((uint16_t *)UART_RDR(p_siopcb->p_siopinib->base)) & 0xFF;
	}

	return c;
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
sio_ena_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	  case SIO_RDY_SND:
		sil_wrb_mem((uint8_t *)UART_SCR(p_siopcb->p_siopinib->base), sil_reb_mem((uint8_t *)UART_SCR(p_siopcb->p_siopinib->base)) | SCR_TIE);
		break;
	  case SIO_RDY_RCV:
		sil_wrb_mem((uint8_t *)UART_SCR(p_siopcb->p_siopinib->base), sil_reb_mem((uint8_t *)UART_SCR(p_siopcb->p_siopinib->base)) | SCR_RIE);
		break;
	}
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
sio_dis_cbr(SIOPCB *p_siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	  case SIO_RDY_SND:
		sil_wrb_mem((uint8_t *)UART_SCR(p_siopcb->p_siopinib->base), sil_reb_mem((uint8_t *)UART_SCR(p_siopcb->p_siopinib->base)) & ~SCR_TIE);
		break;
	  case SIO_RDY_RCV:
		sil_wrb_mem((uint8_t *)UART_SCR(p_siopcb->p_siopinib->base), sil_reb_mem((uint8_t *)UART_SCR(p_siopcb->p_siopinib->base)) & ~SCR_RIE);
		break;
	}
}

/*
 *  1文字出力（ポーリングでの出力）
 */
void sio_pol_snd_chr(char_t c, ID siopid)
{
	uint32_t base = siopinib_table[INDEX_PORT(siopid)].base;

	sil_wrh_mem((uint16_t *)UART_TDR(base), (uint16_t)c);
	while(0 == (sil_reb_mem((uint8_t *)UART_SSR(base)) & (1 << 1)));

	/*
	 *  出力が完全に終わるまで待つ
	 */
	volatile int n = 300000000/BPS_SETTING;
	while(n--);
}

/*
 *  ターゲットのシリアル初期化
 */
void target_uart_init(ID siopid)
{
	const GPIOINIB  *p_gpioinib = &gpioinib_table[INDEX_PORT(siopid)];

	sil_wrw_mem((uint32_t *)p_gpioinib->pfr, sil_rew_mem((uint32_t *)p_gpioinib->pfr) | p_gpioinib->pfr_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->epfr, (sil_rew_mem((uint32_t *)p_gpioinib->epfr) & p_gpioinib->epfr_clr) | p_gpioinib->epfr_set);
	sil_wrw_mem((uint32_t *)p_gpioinib->ade, sil_rew_mem((uint32_t *)p_gpioinib->ade) & ~p_gpioinib->ade_clr);

	uint32_t base = siopinib_table[INDEX_PORT(siopid)].base;

	sil_wrb_mem((uint8_t *)UART_SCR(base), 0);
	sil_wrb_mem((uint8_t *)UART_SMR(base), SMR_MD_UART | SMR_SOE);
	sil_wrh_mem((uint16_t *)UART_BGR(base), (uint16_t)(((SysFrePCLK2 + ((uint32_t)BPS_SETTING / 2)) / (uint32_t)BPS_SETTING) - 1));
	sil_wrb_mem((uint8_t *)UART_ESCR(base), ESCR_DATABITS_8);
	sil_wrb_mem((uint8_t *)UART_SCR(base), SCR_TXE);
}
