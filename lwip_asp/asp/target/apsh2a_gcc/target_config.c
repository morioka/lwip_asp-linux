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
 *	$Id: target_config.c 2142 2011-06-27 07:24:40Z mit-kimai $
 */

/*
 *	ターゲット依存モジュール（APSH2A用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "pdic/sh/sh_scif.h"
#include "apsh2a.h"

/*
 *	ターゲット依存の初期化
 */
void
target_initialize(void)
{
	/*
	 *	プロセッサ依存の初期化
	 */
	prc_initialize();

	/*
	 *	LED接続ポートの初期化
	 */
	sil_wrh_mem((uint16_t *)PAIORH_h, 0x0280U);
	sil_wrh_mem((uint16_t *)PAIORL_h, 0x0000U);
	
	/*
	 *	SCIF1のI/Oポートの設定
	 */
	sil_wrh_mem((uint16_t *)PACRH3_h, 0x55U);
	
	/*
	 * バーナー出力用のシリアルコントローラの初期化
	 */
	sh_scif_init(TARGET_PUTC_PORTID);
}

/*
 *	ターゲット依存の終了処理
 */
void
target_exit(void)
{
	/*
	 *	プロセッサ依存の終了処理
	 */
	prc_exit();

	/*
	 *	開発環境依存の終了処理
	 */
	apsh2a_exit();

	/*
	 * ここには来ない
	 */
	while(1);
}


/*
 *   システム文字出力先の指定
 */
#ifndef TOPPERS_HEW_SIMULATOR

#define TARGET_PUT_CHAR(c)	sh_scif_pol_putc(c, TARGET_PUTC_PORTID)

#else	/* TOPPERS_HEW_SIMULATOR */

extern void hew_io_sim_putc(char_t c);
#define TARGET_PUT_CHAR(c)	hew_io_sim_putc(c)

/*  機能コード  */
#define GETC	0x01210000
#define PUTC	0x01220000

/*
 *   HEWのI/Oシミュレーション機能呼び出し
 */
void hew_io_sim(uint32_t code, void *adr);

void hew_io_sim_putc(char_t c)
{
	uint8_t buf = (uint8_t)c;		/*  バッファ  */
	uint8_t *parmblk = &buf;		/*  パラメータブロック  */
	uint8_t **p = &parmblk;			/*  　　・その先頭アドレス  */
	uint32_t code = PUTC;			/*  機能コード  */
	
	hew_io_sim(code, p);
}

extern bool_t hew_io_sim_snd_chr(char_t c);

bool_t hew_io_sim_snd_chr(char_t c)
{
	hew_io_sim_putc(c);
	return true;
}

extern int_t hew_io_sim_rcv_chr(void);

int_t hew_io_sim_rcv_chr(void) {
	uint8_t buf;					/*  バッファ  */
	uint8_t *parmblk = &buf;		/*  パラメータブロック  */
	uint8_t **p = &parmblk;			/*  　　・その先頭アドレス  */
	uint32_t code = GETC;			/*  機能コード  */
	
	hew_io_sim(code, p);
	return(buf);
}

#endif /* TOPPERS_HEW_SIMULATOR */

/*
 *	システムログの低レベル出力のための文字出力
 */
void
target_fput_log(char_t c)
{
	if (c == '\n') {
		TARGET_PUT_CHAR('\r');
	}
	TARGET_PUT_CHAR(c);
}
