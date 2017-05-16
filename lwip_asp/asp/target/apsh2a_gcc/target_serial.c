/*
 *	TOPPERS/ASP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2007-2010 by Industrial Technology Institute,
 *								Miyagi Prefectural Government, JAPAN
 *	
 *	上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *	ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *	変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *	(1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *		権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *		スコード中に含まれていること．
 *	(2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *		用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *		者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *		の無保証規定を掲載すること．
 *	(3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *		用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *		と．
 *	  (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *		  作権表示，この利用条件および下記の無保証規定を掲載すること．
 *	  (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *		  報告すること．
 *	(4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *		害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *		また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *		由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *		免責すること．
 *	
 *	本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *	よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *	に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *	アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *	の責任を負わない．
 *	
 *	$Id: target_serial.c 2125 2011-06-24 07:42:00Z mit-kimai $
 */

/*
 *	シリアルI/Oデバイス（SIO）ドライバ（APSH2A用）
 *	SCIF1 を使用
 */

#include <kernel.h>
#include <t_syslog.h>
#include "target_serial.h"
#include "target_syssvc.h"	/*  TNUM_SIOP  */


static const INTNO intno_sio_tx_table[TNUM_SIOP] = { 
    SCIF0_RXI_VECTOR
#if TNUM_SIOP > 1
    ,SCIF1_RXI_VECTOR
#endif /* TNUM_SIOP > 1 */   
#if TNUM_SIOP > 2
    ,SCIF2_RXI_VECTOR
#endif /* TNUM_SIOP > 2 */
}; 

static const INTNO intno_sio_rx_table[TNUM_SIOP] = { 
    SCIF0_TXI_VECTOR
#if TNUM_SIOP > 1
    ,SCIF1_TXI_VECTOR
#endif /* TNUM_SIOP > 1 */   
#if TNUM_SIOP > 2
    ,SCIF2_TXI_VECTOR
#endif /* TNUM_SIOP > 2 */
}; 

/* 
 *  シリアルI/OポートIDから割込み番号を取り出すためのマクロ 
 */ 
#define INDEX_SIOP(siopid)           ((uint_t)((siopid) - 1)) 
#define get_intno_sio_tx(siopid)     (intno_sio_tx_table[INDEX_SIOP(siopid)]) 
#define get_intno_sio_rx(siopid)     (intno_sio_rx_table[INDEX_SIOP(siopid)]) 

/*
 *	SIOドライバの初期化
 */
void
sio_initialize(intptr_t exinf)
{
	sh_scif_initialize();
}

/*
 *	シリアルI/Oポートのオープン
 */
SIOPCB *
sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB	*p_siopcb;
	ER		ercd;
	INTNO	intno_sio_tx, intno_sio_rx;

	/*
	 *	シリアルI/O割込みをマスクする．
	 */
	intno_sio_tx = get_intno_sio_tx(siopid);
	intno_sio_rx = get_intno_sio_rx(siopid);
	ercd = dis_int(intno_sio_tx);
	assert(ercd == E_OK);
	ercd = dis_int(intno_sio_rx);
	assert(ercd == E_OK);

	/*
	 *	デバイス依存のオープン処理．
	 */
	p_siopcb = sh_scif_opn_por(siopid, exinf);

	/*
	 *	シリアルI/O割込みのマスクを解除する．
	 */
	ercd = ena_int(intno_sio_tx);
	assert(ercd == E_OK);
	ercd = ena_int(intno_sio_rx);
	assert(ercd == E_OK);

	return(p_siopcb);
}

/*
 *	シリアルI/Oポートのクローズ
 */
void
sio_cls_por(SIOPCB *p_siopcb)
{
	ER		ercd;
    ID		siopid; 
	INTNO	intno_sio_tx, intno_sio_rx;

    /* 
     *  シリアルI/OポートIDの取得 
     */ 
    siopid = sh_scif_get_siopid(p_siopcb);

	/*
	 *	デバイス依存のクローズ処理．
	 */
	sh_scif_cls_por(p_siopcb);

	/*
	 *	シリアルI/O割込みをマスクする．
	 */
	intno_sio_tx = get_intno_sio_tx(siopid);
	intno_sio_rx = get_intno_sio_rx(siopid);
	ercd = dis_int(intno_sio_tx);
	assert(ercd == E_OK);
	ercd = dis_int(intno_sio_rx);
	assert(ercd == E_OK);
}

/*
 *	SIOの割込みハンドラ
 */
void
sio_tx_isr(intptr_t exinf)
{
	sh_scif_tx_isr(exinf);
}


/*
 *	SIOの割込みハンドラ
 */
void
sio_rx_isr(intptr_t exinf)
{
	sh_scif_rx_isr(exinf);
}

#ifndef TOPPERS_HEW_SIMULATOR
/*
 *	シリアルI/Oポートへの文字送信
 */
bool_t
sio_snd_chr(SIOPCB *siopcb, char_t c)
{
	return(sh_scif_snd_chr(siopcb, c));
}

/*
 *	シリアルI/Oポートからの文字受信
 */
int_t
sio_rcv_chr(SIOPCB *siopcb)
{
	return(sh_scif_rcv_chr(siopcb));
}
#endif /* TOPPERS_HEW_SIMULATOR */

/*
 *	シリアルI/Oポートからのコールバックの許可
 */
void
sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	sh_scif_ena_cbr(siopcb, cbrtn);
}

/*
 *	シリアルI/Oポートからのコールバックの禁止
 */
void
sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	sh_scif_dis_cbr(siopcb, cbrtn);
}

/*
 *	シリアルI/Oポートからの送信可能コールバック
 */
void
sh_scif_irdy_snd(intptr_t exinf)
{
	sio_irdy_snd(exinf);
}

/*
 *	シリアルI/Oポートからの受信通知コールバック
 */
void
sh_scif_irdy_rcv(intptr_t exinf)
{
	sio_irdy_rcv(exinf);
}
