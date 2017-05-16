/*
 *	TOPPERS/ASP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *									Toyohashi Univ. of Technology, JAPAN
 *	Copyright (C) 2007		by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2008-2010 by Industrial Technology Institute,
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
 *	$Id: start.S 1824 2010-07-02 06:50:03Z mit-kimai $
 */

/*
 *	カーネル対応のスタートアップモジュール(SH12A用)
 */

#define TOPPERS_MACRO_ONLY
#define UINT_C(val) 	(val)		 /* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		 /* ulong_t型の定数を作るマクロ */
#define CAST(type, val) (val)		 /* 型キャストを行うマクロ */
#include "kernel_impl.h"

	.text
	.global _start
	.align	2
_start:
	/*
	 *	SRを初期化する。
	 *	（全割込みロックとする。）
	 */
	mov.l  _iipm_disall_start, r0
	ldc    r0, sr

	/*
	 *	非タスクコンテキスト用のスタックの初期化
	 */
	mov.l  _kernel_istkpt_p, r1
	mov.l  @r1, r15

	/*
	 *	_hardware_init_hook を呼び出す。(0でない場合)
	 *
	 *	ターゲットハードウェアに依存して必要な初期化処理がある場合は，
	 *	hardware_init_hookという関数を用意する．
	 */
start_0:
	mov.l  _hardware_init_hook_start, r0
	tst    r0, r0
	bt	   start_1
#ifdef TOPPERS_SUPPORT_SH2A_INSTRUCTION
	jsr/n  @r0
#else /* TOPPERS_SUPPORT_SH2A_INSTRUCTION */
	jsr    @r0
	 nop
#endif /* TOPPERS_SUPPORT_SH2A_INSTRUCTION */
	
	/*
	 * bssセクションをクリア
	 *
	 *	各セクションは4バイト境界に配置されているので、
	 *	_endが4バイト境界でないとしても、
	 *	隣のセクションを上書きすることはない。
	 *	（下記のdataセクションの初期化も同様）
	 */
start_1:
	mov.l  _bss_start_p, r1
	mov.l  _end_p, r2
	cmp/eq r1, r2
	bt	   start_3
	mov    #0, r0
#ifdef TOPPERS_SUPPORT_SH2E_INSTRUCTION
	/*
	 *	下記の遅延スロット（(a)行）は、ループを脱出する際にも
	 *	実行されるので、予め、ループの回数を1回減らしておく。
	 *	（注意：_endの番地が3以下だと演算結果が負になり、NG）
	 */
	add    #-4, r2
start_2:
	cmp/hi r1, r2
	bt/s   start_2
	 mov.l  r0, @r1+	/*  (a) ソースレジスタがr0限定 */
#else /* TOPPERS_SUPPORT_SH2E_INSTRUCTION */
	/*
	 *	上記(a)行の逆スタック命令がなくても、末尾_end側から
	 *	コピーする方法もあるが、そこまではしていない。
	 *	（_endの4未満切り上げを求める必要がある。）
	 */
start_2:
	mov.l  r0, @r1
	add    #4, r1
	cmp/hi r1, r2
	bt	   start_2
#endif /* TOPPERS_SUPPORT_SH2E_INSTRUCTION */

	/*
	 *	dataセクションを初期化する（ROM化対応）．
	 *
	 *	__idata_start から __idata_end までを，__data_start 以降に
	 *	コピーする．
	 */
start_3:
	mov.l  __idata_start_p, r1
	mov.l  __idata_end_p, r2
	cmp/eq r1, r2
	bt	   start_5
	mov.l  __data_start_p, r3
#ifdef TOPPERS_SUPPORT_SH2E_INSTRUCTION
	/*
	 *	下記の遅延スロット（(b)行）は、ループを脱出する際にも
	 *	実行されるので、予め、ループの回数を1回減らしておく。
	 *	（注意：__idata_endの番地が3以下だと演算結果が負になり、NG）
	 */
	add    #-4, r2
start_4:
	mov.l  @r1+, r0
	cmp/hi r1, r2
	bt/s   start_4
	 mov.l  r0, @r3+	/*  (b)ソースレジスタがr0限定  */
#else /* TOPPERS_SUPPORT_SH2E_INSTRUCTION */
start_4:
	mov.l  @r1+, r0
	cmp/hi r1, r2
	mov.l  r0, @r3
	add    #4, r3
	bt	   start_4
#endif /* TOPPERS_SUPPORT_SH2E_INSTRUCTION */

	/*
	 *	_software_init_hook を呼び出す（0 でない場合）．
	 *
	 *	ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
	 *	理がある場合は，software_init_hook という関数を用意すれば
	 *	よい．
	 */
start_5:
	mov.l  _software_init_hook_start, r0
	tst    r0, r0
	bt	   start_6
#ifdef TOPPERS_SUPPORT_SH2A_INSTRUCTION
	jsr/n  @r0
#else /* TOPPERS_SUPPORT_SH2A_INSTRUCTION */
	jsr    @r0
	 nop
#endif /* TOPPERS_SUPPORT_SH2A_INSTRUCTION */

start_6:
    /*
     *  キャッシュの初期化（SH2A固有。ただし、sh7211を除く。）
     *  　
     *  　キャッシュの初期化はキャッシュ無効空間で行う必要があるため、
     *  　関数呼び出しの際に、分岐先アドレスのビット29をセットしている。
     *  　（0x2000,0000とor演算しているのは、最初から_cache_initが
     *  　　キャッシュ無効空間にマッピングされている場合の対策）
     *  　
     *  　アドレスマップ（外部メモリのみ抜粋）：
     *  　　0x0000,0000 - 0x1fff,ffff　キャッシュ有効空間
     *  　　0x2000,0000 - 0x3fff,ffff　キャッシュ無効空間（シャドー空間）
     */
#ifdef ENABLE_CACHE
	mov.l  _cache_init_p, r1
	mov.l  _non_cachable_addr_top, r2
	or     r2, r1
	jsr/n  @r1			/*  SH2A固有の処理なので、jsr/n命令が使用できる  */
#endif /* ENABLE_CACHE */

	/*
	 *	カーネルを起動する
	 */
	mov.l  _sta_ker_start, r0
#ifdef TOPPERS_SUPPORT_SH2A_INSTRUCTION
	jsr/n  @r0
#else /* TOPPERS_SUPPORT_SH2A_INSTRUCTION */
	jsr    @r0
	 nop
#endif /* TOPPERS_SUPPORT_SH2A_INSTRUCTION */

    /*
     *  キャッシュの初期化（SH2A固有。ただし、sh7211を除く。）
     */
#ifdef ENABLE_CACHE
_cache_init:
    mov.l _ccr1_addr, r1
								/*  キャッシュ・フラッシュ  */
    movi20 #(CCR1_ICF | CCR1_OCF), r0
    mov.l r2, @r1
    mov.l _ccr_init_val, r2		/*  値はターゲット依存  */
    mov.l r2, @r1
    mov.l @r1, r2				/*  ダミーリード  */
    rts/n						/*  （念のため、遅延スロットには配置しない）*/
#endif /* ENABLE_CACHE */

	.align 2
_iipm_disall_start:
	.long IIPM_DISALL
_kernel_istkpt_p:
	.long __kernel_istkpt
_bss_start_p:
	.long __bss_start
_end_p:
	.long _end
__idata_start_p:
	.long __idata_start
__idata_end_p:
	.long __idata_end
__data_start_p:
	.long __data_start

_hardware_init_hook_start:
	.long _hardware_init_hook
_software_init_hook_start:
	.long _software_init_hook
_sta_ker_start:
	.long _sta_ker

#ifdef ENABLE_CACHE
_cache_init_p:
	.long _cache_init
_non_cachable_addr_top:
	.long 0x20000000
_ccr1_addr:
	.long CCR1
_ccr_init_val:
	.long CCR1_INIT			/*  値はターゲット依存  */
#endif /* ENABLE_CACHE */


/*
 *  HEWのI/Oシミュレーション機能の呼び出し
 *  
 *  本来は、sys_support.Sに記述すべきだが、
 *  リンク後の_hew_io_sim_bodyのアドレスを
 *  アプリケーションによらず、一定にするため、
 *  スタートアップルーチンの直後に配置している。
 *  
 *  
 *  　以下の状態で呼ばれる。
 *  　　r4：機能コード
 *  　　r5：パラメータブロックへのポインタ
 */
#ifdef TOPPERS_HEW_SIMULATOR

    .section .text
    .align 2        
    .global _hew_io_sim
_hew_io_sim:
	mov   r4, r0			/*  機能コード  */
	mov   r5, r1			/*  パラメータブロックへのポインタ  */
	mov.l _hew_io_sim_body_addr, r3
    sts.l pr,@-r15
	jsr   @r3
	nop
    lds.l @r15+,pr
    rts
     nop

/*
 *  このアドレスに分岐することで、HEWのI/Oシミュレーション機能が
 *  作動するトリガとなる。
 *  
 */
_hew_io_sim_body:
    rts
     nop
	
	.align  4
_hew_io_sim_body_addr:
    .long  _hew_io_sim_body

#endif	/*  TOPPERS_HEW_SIMULATOR  */

