/*
 *	TOPPERS/ASP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *								Toyohashi Univ. of Technology, JAPAN
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
 *	$Id: target_support.S 1824 2010-07-02 06:50:03Z mit-kimai $
 */

/*
 *	ターゲット依存モジュール アセンブリ言語部（APSH2A）
 */

#define  TOPPERS_MACRO_ONLY
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */
#include "kernel_impl.h"


/*	CMCOR_0レジスタに設定する値  */
#define CMCOR_0_VAL		((((PCLOCK / 8)/1000000) * 100)-1)

#if CMCOR_0_VAL > 0xffff
#error CMCOR_0 value > 0xffff(word data) !
#endif	/*	CMCOR_0_VAL > 0xffff  */

/*
 *	低レベルのターゲット依存の初期化
 *
 *	スタートアップモジュールの中で，メモリの初期化の前に呼び出される．
 */

/*
 *	ROM化時に，DATAセクションのサイズが0だと，BSSがTXTの直下に配置され
 *	るため，DATAセクションのサイズを0にしないためのダミー変数を定義．
 *	コンパイラの最適化により削除されるのを防ぐため、アセンブラで記述し
 *	ている。
 */
	.data
_dummy:
	.byte	0x00

	.text
	.align 2
	.global _hardware_init_hook
_hardware_init_hook:
	/*
	 *	クロック発信器(CPG)設定
	 *	　IΦ=160MHz,BΦ=40MHz,PΦ=40MHz
	 */
	mov.l _frqcr_addr_h, r1
	movi20 #0x1303, r2
	mov.w r2, @r1

	/* 省電力モードの設定 */

		/*	sleep命令の実行で、スリープモードへ遷移  */
	mov.l _stbcr_addr_b, r1
	mov   #0x00, r2
	mov.b r2, @r1

	/*
	 *	各モジュールの電源供給の設定
	 *	　使用するモジュールへの電源供給の設定をここで行わないと
	 *	　例えば、下記の「100usec待ち」が期待した動作にならない。
	 */
	mov   #(STBCR2_VALUE), r2
	mov.b r2, @((STBCR2_b - STBCR_b), r1)

	mov.l _pdm_base2_addr, r1
	mov   #(STBCR3_VALUE), r2
	mov.b r2, @((STBCR3_b - PDM_BASE2), r1)

	mov   #(STBCR4_VALUE), r2
	mov.b r2, @((STBCR4_b - PDM_BASE2), r1)

	/*
	 *	内部RAMを有効にする
	 *
	 *	システムコントロールレジスタの設定の注意事項
	 *　　・SYSCRnへのライト命令の直後にリード命令を配置する。
	 *　　・内蔵RAM空間以外にプログラムを配置する。
	 */
							/*  内蔵RAMへのアクセス有効  */
	mov   #0xff, r0
	mov.b r0, @((SYSCR1_b - PDM_BASE2), r1)	/*	r0=0xff  */
	mov.b @((SYSCR1_b - PDM_BASE2), r1), r3	/*	ダミーリード  */

							/*	内蔵RAMへのライト有効  */
	mov.b r0, @((SYSCR2_b - PDM_BASE2), r1)	/*	r0=0xff  */
	mov.b @((SYSCR2_b - PDM_BASE2), r1), r3	/*	ダミーリード  */

	/* バスステートコントローラ（BSC）の設定 */
	mov.l _bsc_base_addr, r1
	movi20 #0x00001010, r0
	mov.l r0, @((CMNCR_w - BSC_BASE),r1)
	movi20 #0x00004400, r0
	mov.l r0, @((CS3BCR_w - BSC_BASE),r1)
	movi20 #0x00002491, r0
	mov.l r0, @((CS3WCR_w - BSC_BASE),r1)
	movi20 #0x00000909, r0
	mov.l r0, @((SDCR_w - BSC_BASE),r1)
	mov.l _rtcsr_val, r0
	mov.l r0, @((RTCSR_w - BSC_BASE),r1)
	mov.l _rtcor_val, r0
	mov.l r0, @((RTCOR_w - BSC_BASE),r1)

	/* SDRAM 接続ポートの初期化 */
	mov.l _pfc_base_addr, r1
	movi20 #0x1111, r0
	mov.w r0, @((PACRL4_h - PFC_BASE),r1)
	mov.w r0, @((PACRL3_h - PFC_BASE),r1)	/*	r0=0x1111  */
	mov.w r0, @((PACRL2_h - PFC_BASE),r1)	/*	r0=0x1111  */
	movi20 #0x1110, r0
	mov.w r0, @((PACRL1_h - PFC_BASE),r1)

	mov   #0x00, r0
	mov.w r0, @((PBIORH_h - PFC_BASE),r1)
	mov.w r0, @((PBIORL_h - PFC_BASE),r1)	/*	r0=0x0000  */
	mov   #0x10, r0
	mov.w r0, @((PBCRH1_h - PFC_BASE),r1)
	mov   #0x11, r0
	mov.w r0, @((PBCRL3_h - PFC_BASE),r1)
	movi20 #0x0111, r0
	mov.w r0, @((PBCRL2_h - PFC_BASE),r1)
	movi20 #0x1010, r0
	mov.w r0, @((PBCRL1_h - PFC_BASE),r1)

	mov   #0x00, r0
	mov.w r0, @((PDIOR_h - PFC_BASE),r1)
	movi20 #0x1111, r0
	mov.w r0, @((PDCRL4_h - PFC_BASE),r1)
	mov.w r0, @((PDCRL3_h - PFC_BASE),r1)	/*	r0=0x1111  */
	mov.w r0, @((PDCRL2_h - PFC_BASE),r1)	/*	r0=0x1111  */
	mov.w r0, @((PDCRL1_h - PFC_BASE),r1)	/*	r0=0x1111  */

	/*
	 *	100usec待ち
	 */
	mov.l _cmt_base_addr, r1		/*	タイマカウント停止	*/
	mov   #0, r0
	mov.w r0, @((CMSTR_h - CMT_BASE),r1)
									/*	クロックセレクト：Pφ/8  */
	mov.w r0, @((CMCSR_0_h - CMT_BASE),r1)
									/*	カウンタ・クリア  */
	mov.w r0, @((CMCNT_0_h - CMT_BASE),r1)
	movi20 #(CMCOR_0_VAL), r0		/*	目標値設定	*/
	mov.w r0, @((CMCOR_0_h - CMT_BASE),r1)
	mov   #1, r0					/*	タイマカウント開始	*/
	mov.w r0, @((CMSTR_h - CMT_BASE),r1)

	movi20 #(CMCSR_CMF), r2
1:
	mov.w @((CMCSR_0_h - CMT_BASE),r1), r0
	tst   r2, r0				/*	コンペアマッチフラグの判定	*/
	bt	  1b

	mov.l _sdram_mode_addr, r1	/*	外部RAM上のモード設定レジスタ  */
	mov   #0x00, r2

	rts
	 mov.w r2, @r1

	.align 4
_frqcr_addr_h:
	.long FRQCR_h

_stbcr_addr_b:
	.long STBCR_b
_pdm_base2_addr:
	.long PDM_BASE2

_bsc_base_addr:
	.long BSC_BASE
_rtcsr_val:
	.long 0xa55a0008
_rtcor_val:
	.long 0xa55a009c

_pfc_base_addr:
	.long PFC_BASE

_cmt_base_addr:
	.long CMT_BASE

_sdram_mode_addr:
	.long 0xfffc5040

