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
 * ターゲット依存モジュール（cq_frk_fm3用）
 */
#include "kernel_impl.h"
#include <sil.h>

#include "fm3_mb9bxxx.h"
#include "target_serial.h"

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void){

	static uint32_t u32IoRegisterRead;

	/*
	 *  ハードウェア・ウオッチドグ・タイマの停止
	 */
	sil_wrw_mem((uint32_t*)FM3_HWWDT_WDG_LCK, 0x1ACCE551);
	sil_wrw_mem((uint32_t*)FM3_HWWDT_WDG_LCK, 0xE5331AAE);
	sil_wrb_mem((uint8_t*)FM3_HWWDT_WDG_CTL, 0);

	/*
	 *  クロックの設定
	 */
	sil_wrb_mem((uint8_t*)FM3_CRG_BSC_PSR, 0);
	sil_wrb_mem((uint8_t*)FM3_CRG_APBC0_PSR, 0);
	sil_wrb_mem((uint8_t*)FM3_CRG_APBC1_PSR, APBC1_PSR_APBC1EN | 1);
	sil_wrb_mem((uint8_t*)FM3_CRG_APBC2_PSR, APBC2_PSR_APBC2EN | 1);
	sil_wrb_mem((uint8_t*)FM3_CRG_SWC_PSR, SWC_PSR_TESTB | 3);
	sil_wrb_mem((uint8_t*)FM3_CRG_TTC_PSR, 0);

	sil_wrb_mem((uint8_t*)FM3_CRG_CSW_TMR, (5 << 4) | 0x0C);

	sil_wrb_mem((uint8_t*)FM3_CRG_SCM_CTL, sil_reb_mem((uint8_t*)FM3_CRG_SCM_CTL) | SCM_CTL_MOSCE);
	while (!(sil_reb_mem((uint8_t*)FM3_CRG_SCM_STR) & SCM_CTL_MOSCE));

	sil_wrb_mem((uint8_t*)FM3_CRG_PSW_TMR, 0);
	sil_wrb_mem((uint8_t*)FM3_CRG_PLL_CTL1, 1);
	sil_wrb_mem((uint8_t*)FM3_CRG_PLL_CTL2, 0x23);

	sil_wrb_mem((uint8_t*)FM3_CRG_SCM_CTL, sil_reb_mem((uint8_t*)FM3_CRG_SCM_CTL) | SCM_CTL_PLLE);
	while (!(sil_reb_mem((uint8_t*)FM3_CRG_SCM_STR) & SCM_CTL_PLLE));

	sil_wrb_mem((uint8_t*)FM3_CRG_SCM_CTL, sil_reb_mem((uint8_t*)FM3_CRG_SCM_CTL) | (2 << 5));

	do
	{
		u32IoRegisterRead = (sil_reb_mem((uint8_t*)FM3_CRG_SCM_CTL) & 0xE0); 
	}while ((sil_reb_mem((uint8_t*)FM3_CRG_SCM_STR) & 0xE0) != u32IoRegisterRead);

	/*
	 *  高速CRトリミングの設定
	 */
	if( 0x000003FF != (sil_rew_mem((uint32_t*)FM3_FLASH_IF_CRTRMM) & 0x000003FF) )
	{
		sil_wrw_mem((uint32_t*)FM3_CRTRIM_MCR_RLR, 0x1ACCE554);
		sil_wrw_mem((uint32_t*)FM3_CRTRIM_MCR_FTRM, sil_rew_mem((uint32_t*)FM3_FLASH_IF_CRTRMM));
		sil_wrw_mem((uint32_t*)FM3_CRTRIM_MCR_RLR, 0x00000000);
	}

	/*
	 * コア依存の初期化
	 */
	core_initialize();

	/*
	 *  バナー出力用のシリアル初期化
	 */
	target_uart_init(SIO_PORTID);
} 


/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
{
	/*
	 *  コア依存の終了処理
	 */
	core_terminate();

	/*
	 *  開発環境依存の終了処理
	 */
	while(1);
}


/*
 *  システムログの低レベル出力のための文字出力
 */
void
target_fput_log(char_t c)
{
	if (c == '\n') {
		sio_pol_snd_chr('\r', SIO_PORTID);
	}
	sio_pol_snd_chr(c, SIO_PORTID);
}
