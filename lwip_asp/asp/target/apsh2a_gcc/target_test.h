/*
 *	TOPPERS/ASP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2011 by Industrial Technology Institute,
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
 *	$Id: target_test.h 2212 2011-08-04 07:10:22Z mit-kimai $
 */

/*
 *		テストプログラムのターゲット依存定義（APSH2A用）
 */

#ifndef TOPPERS_TARGET_TEST_H
#define TOPPERS_TARGET_TEST_H

#define CPUEXC1				9		/* アドレスエラー例外 */

/*
 *	アドレスエラー例外発生関数
 *	　
 *	　例外発生時にスタックに退避されるPCの値は「最後に実行した命令」
 *	　の次命令の先頭アドレスを指している。
 *	　この「最後に実行した命令」とは実際にアドレスエラー例外を発生
 *	　した命令ではなく、例外発生時にパイプライン上で実行されている
 *	　後続の命令である。
 *	　（例外を発生した命令がメモリアクセスステージで、後続の命令が
 *	　　既に実行ステージにある場合）
 *	　
 *	　また、後続の命令に分岐命令が含まれる場合は、退避すべきPCの値が
 *	　書き換えられているケースがある。
 *	　
 *	　「最後に実行した命令」が例外発生箇所から何命令離れているか
 *	　一概には求められないため、ここではnop命令を挿入している。
 */
Inline void 
toppers_raise_cpu_exception(void)
{
	uint32_t tmp;
	uint32_t adr = 0xfffffec1U;		/*  奇数番地  */
	
	Asm(" mov.l @%1, %0 \n"			/*  アドレスエラー例外を発生  */
	    " nop           \n"			/*  ←スタックに退避されるアドレス  */
	    " nop           \n"
	    " nop           \n"
	    " nop           " : "=r"(tmp): "r"(adr));
}

#define RAISE_CPU_EXCEPTION	toppers_raise_cpu_exception()

/*
 *	タスクで使用するシリアルポートID
 */
#define TASK_PORTID 2

/*  TASK_LOOPを定義すれば、実行速度の計測は行わない  */
#ifdef TOPPERS_HEW_SIMULATOR

#define TASK_LOOP	(500000)			/* タスクのループ回数 */

#define LOOP_REF	(1000000/500)		/* 速度計測用のループ回数 */
#endif /* TOPPERS_HEW_SIMULATOR */


#endif /* TOPPERS_TARGET_TEST_H */
