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
 *	$Id: target_serial.h 1824 2010-07-02 06:50:03Z mit-kimai $
 */

/*
 *	シリアルI/Oデバイス（SIO）ドライバ（CQ_FRK_SH2A用）
 */

#ifndef TOPPERS_TARGET_SERIAL_H
#define TOPPERS_TARGET_SERIAL_H

#include "cq_frk_sh2a.h"
#include "pdic/sh/sh_scif.h"

/*
 *	SIOの割込みハンドラのベクタ番号
 *	　注意：target_syssvc.h内にも同様の定義が必要。
 */
#define INHNO_SIO_TX	 SCIF2_TXI_VECTOR /* 割込みハンドラ番号 */
#define INTNO_SIO_TX	 SCIF2_TXI_VECTOR /* 割込み番号 */
#define INHNO_SIO_RX	 SCIF2_RXI_VECTOR /* 割込みハンドラ番号 */
#define INTNO_SIO_RX	 SCIF2_RXI_VECTOR /* 割込み番号 */
#define INHNO_SIO_ER	 SCIF2_ERI_VECTOR /* 割込みハンドラ番号 */
#define INTNO_SIO_ER	 SCIF2_ERI_VECTOR /* 割込み番号 */
#define INHNO_SIO_BR	 SCIF2_BRI_VECTOR /* 割込みハンドラ番号 */
#define INTNO_SIO_BR	 SCIF2_BRI_VECTOR /* 割込み番号 */
#define INTPRI_SIO		 (-4)			  /* 割込み優先度 */
#define INTATR_SIO		 0U 			  /* 割込み属性 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *	SIOドライバの初期化
 */
extern void sio_initialize(intptr_t exinf);

/*
 *	シリアルI/Oポートのオープン
 */
extern SIOPCB *sio_opn_por(ID siopid, intptr_t exinf);

/*
 *	シリアルI/Oポートのクローズ
 */
extern void sio_cls_por(SIOPCB *p_siopcb);

/*
 *	SIOの割込みハンドラ
 */
extern void sio_tx_isr(intptr_t exinf);
extern void sio_rx_isr(intptr_t exinf);
extern void sio_er_isr(intptr_t exinf);
extern void sio_br_isr(intptr_t exinf);

/*
 *	シリアルI/Oポートへの文字送信
 */
#ifndef TOPPERS_HEW_SIMULATOR
extern bool_t sio_snd_chr(SIOPCB *siopcb, char_t c);

#else /* TOPPERS_HEW_SIMULATOR */

extern bool_t hew_io_sim_snd_chr(char_t c);
#define sio_snd_chr(siopcb, c) hew_io_sim_snd_chr(c)

#endif /* TOPPERS_HEW_SIMULATOR */

/*
 *	シリアルI/Oポートからの文字受信
 */
#ifndef TOPPERS_HEW_SIMULATOR
extern int_t sio_rcv_chr(SIOPCB *siopcb);

#else /* TOPPERS_HEW_SIMULATOR */

extern int_t hew_io_sim_rcv_chr(void);
#define sio_rcv_chr(siopcb) hew_io_sim_rcv_chr( )

#endif /* TOPPERS_HEW_SIMULATOR */

/*
 *	シリアルI/Oポートからのコールバックの許可
 */
extern void sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *	シリアルI/Oポートからのコールバックの禁止
 */
extern void sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn);

/*
 *	シリアルI/Oポートからの送信可能コールバック
 */
extern void sio_irdy_snd(intptr_t exinf);

/*
 *	シリアルI/Oポートからの受信通知コールバック
 */
extern void sio_irdy_rcv(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TARGET_SERIAL_H */
