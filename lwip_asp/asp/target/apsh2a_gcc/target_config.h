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
 *	$Id: target_config.h 1824 2010-07-02 06:50:03Z mit-kimai $
 */

/*
 *	ターゲット依存モジュール（APSH2A用）
 *
 *	カーネルのターゲット依存部のインクルードファイル．kernel_impl.hのター
 *	ゲット依存部の位置付けとなる．
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *	ターゲットシステムのハードウェア資源の定義
 */
#include "apsh2a.h"

/*
 *	トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"

/*
 *	出力するログ種別の定義
 *	　必要に応じてasp/arch/logtrace/trace_dump.cのtrace_print()に
 *	　対応するログ種別を追記する。
 */
#define LOG_INH_ENTER			/*	 17(0x11)  */
#define LOG_INH_LEAVE			/*	145(0x91)  */
#define LOG_EXC_ENTER			/*	 22(0x16)  */
#define LOG_EXC_LEAVE			/*	150(0x96)  */

#endif /* TOPPERS_ENABLE_TRACE */

/*
 *	ターゲット依存の文字出力に使用するポートの定義
 */
#define TARGET_PUTC_PORTID 2

/*
 *	ASPカーネル動作時のメモリマップと関連する定義
 */

/*
 *	デフォルトの非タスクコンテキスト用のスタック領域の定義
 */
/*  スタックサイズ  */
#ifdef ROM_BOOT				/*	ROM化  */
#define DEFAULT_ISTKSZ	  0x8000U
#else	/*	ROM_BOOT  */	/*	デバッグ時	*/
#define DEFAULT_ISTKSZ	  0x2000U
#endif /*  ROM_BOOT  */

/*	スタック領域の底 */
#define DEFAULT_ISTK_BOTTOM		INNER_RAM_PAGE4_END

/*	スタック領域の先頭番地（スタック領域の底ではないので、注意） */
#define DEFAULT_ISTK	  (void *)(DEFAULT_ISTK_BOTTOM - DEFAULT_ISTKSZ)

#ifndef TOPPERS_MACRO_ONLY

/*
 *	ターゲットシステム依存の初期化
 */
extern void target_initialize(void);

/*
 *	ターゲットシステムの終了
 *
 *	システムを終了する時に使う．
 */
extern void target_exit(void) NoReturn;

#endif /* TOPPERS_MACRO_ONLY */

/*
 *	微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#ifdef ROM_BOOT		/*	ROM化の場合  */
#define SIL_DLY_TIM1	 75
#define SIL_DLY_TIM2	 21

#else				/*	RAM上でデバッグする場合  */
#define SIL_DLY_TIM1	 1466
#define SIL_DLY_TIM2	  682

#endif


/*
 *	STBCR2〜4の初期値（各1バイト）
 */
	/*
	 *	・H-UDI
	 *	　・ROM化時：停止
	 *	　・デバッグ時：動作
	 *	・UBCは停止
	 *	・DMACは停止
	 */
#ifdef ROM_BOOT		/*	ROM化の場合  */
#define STBCR2_VALUE	 (STBCR2_HUDI | STBCR2_UCB | STBCR2_DMAC)
#else				/*	RAM上でデバッグする場合  */
#define STBCR2_VALUE	 (STBCR2_UCB | STBCR2_DMAC)
#endif

	/*
	 *	・低消費電力モード時に端子状態を保持する。
	 *	・MTU2Sは停止
	 *	・MTU2は停止
	 *	・POE2は停止
	 *	・IIC3は停止
	 *	・ADCは停止
	 *	・DACは停止
	 *	・フラッシュメモリは動作
	 */
#define STBCR3_VALUE	 (STBCR3_HZ | STBCR3_MTU2S | STBCR3_MTU2 | STBCR3_POE2 \
						  | STBCR3_IIC3 | STBCR3_ADC | STBCR3_DAC)

	/*
	 *	・SCFI0は停止
	 *	・SCFI1は動作
	 *	・SCFI2は停止
	 *	・SCFI3は停止
	 *	・CMTは動作
	 *	・WAVEIFは停止
	 */
#define STBCR4_VALUE	 (STBCR4_SCIF0 | STBCR4_SCIF2 |  STBCR4_SCIF3 | STBCR4_WAVEIF)


/*
 *  割込み許可が有効になるまでの時間待ちをするためのnop命令
 *  
 *  　SH7211は一般的なキャッシュを内蔵していない代わりに、
 *  　内蔵フラッシュROMに対してのみ有効なROMキャッシュを
 *  　内蔵している。このROMキャッシュは常にONで、OFFにはできない。
 *  　そのため、ENABLE_CACHEマクロではなく、ROM_BOOTマクロを
 *  　用いて、遅延に必要な命令数を判別している。
 */
#ifdef ROM_BOOT		/*  ROMキャッシュの影響を受ける場合  */
	/*
	 *  ROMキャッシュの効果を考慮
	 *  　　CPUからINTCに伝達されるまでの遅延
	 *  　　（スーパスカラで3Iφcyc分＝6命令） 
	 */

	/*  ディスパッチャ用  */
#define LDC_NOP_DISPATCHER		nop; nop; nop; nop; nop; nop

	/*  set_sr用：このターゲットでは必要なし  */
// #define LDC_NOP_SET_SR			"nop; nop"

	/*  TOPPERS_set_iipm用はtarget_sil.hに記述する。  */

#endif	/*  ROM_BOOT  */

/*
 *	プロセッサ依存モジュール（SH7211用）
 */
#include "sh12a_gcc/sh7211_config.h"

#endif	/* TOPPERS_TARGET_CONFIG_H */
