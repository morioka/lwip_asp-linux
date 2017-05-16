/*
 *  TINET (TCP/IP Protocol Stack)
 * 
 *  Copyright (C) 2001-2009 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
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
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: $
 */

/*
 *  ターゲット依存モジュール（AKI-H8/3069F用）
 */

#include <sil.h>
#include "sh12a_gcc/sh7262.h"

#include <tinet_defs.h>
#include <tinet_config.h>

#if defined(SUPPORT_ETHER)

/*
 *  ed_bus_init -- ターゲット依存部のバスの初期化
 */

void
ed_bus_init (void)
{
	/* ~IRQ0/PE0 */
	sil_wrh_mem((void*)PECR0, (sil_reh_mem((void*)PECR0) & ~0x000f) | 0x0003);

	/* ~CS2/PJ2 */
	sil_wrh_mem((void*)PJCR0, (sil_reh_mem((void*)PJCR0) & ~0x0f00) | 0x0300);
	/* ~RD/PC1 */
	sil_wrh_mem((void*)PCCR0, (sil_reh_mem((void*)PCCR0) & ~0x00f0) | 0x0010);
	/* ~WE0/~DQML/PC3 */
	sil_wrh_mem((void*)PCCR0, (sil_reh_mem((void*)PCCR0) & ~0xf000) | 0x1000);

	/* A0 */
	sil_wrh_mem((void*)PJCR0, (sil_reh_mem((void*)PJCR0) & ~0x000f) | 0x0005);
	/* A1〜A3 */
	sil_wrh_mem((void*)PBCR0, (sil_reh_mem((void*)PBCR0) & ~0xfff0) | 0x1110);
	/* A4〜A5 */
	sil_wrh_mem((void*)PBCR1, (sil_reh_mem((void*)PBCR1) & ~0x00ff) | 0x0011);

	/* D0〜D3 */
	sil_wrh_mem((void*)PDCR0, 0x1111);
	/* D4〜D7 */
	sil_wrh_mem((void*)PDCR1, 0x1111);

	sil_wrw_mem((void*)0xfffc000c, //CS2BCR,
		(7<<28) |	/* IWW		ライト−リード／ライト−ライトサイクル間アイドル指定：アイドルサイクルなし */
		(7<<25) |	/* IWRWD	別空間リード−ライトサイクル間アイドル指定：１アイドルサイクル挿入 */
		(7<<22) |	/* IWRWS	同一空間リード−ライトサイクル間アイドル指定：１アイドルサイクル挿入 */
		(7<<19) |	/* IWRRD	別空間リード−リードサイクル間アイドル指定：１アイドルサイクル挿入 */
		(7<<16) |	/* IWRRS	同一空間リード−リードサイクル間アイドル指定：１アイドルサイクル挿入 */
		(0<<12) |	/* TYPE		メモリ種別：通常空間 */
		(0<<11) |	/* ENDIAN	エンディアン指定：ビッグエンディアンとして動作 */
		(1<< 9) 	/* BSZ		データバス幅指定：8ビット */
	);

	sil_wrw_mem((void*)0xfffc0030, //CS2WCR,
		(0<<20) |	/* BAS		バイト選択つきSRAMバイトアクセス選択 */
		(12<<7) |	/* WR		アクセスウェイトサイクル数：3サイクル */
		(0<< 6) |	/* WM		外部ウェイトマスク指定：外部ウェイト入力無視 */
	);

	}

/*
 *  ed_inter_init -- ターゲット依存部の割込みの初期化
 */

void
ed_inter_init (void)
{
	}

#endif	/* of #if defined(SUPPORT_ETHER) */
