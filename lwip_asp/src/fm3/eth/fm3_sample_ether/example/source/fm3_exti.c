/*
 * fm3_exti.c
 *
 *  Created on: 2012/03/04
 *      Author: Kentaro Sekimoto
 */

#include "common.h"
#include "mcu.h"
#include "fm3_ether.h"
#include "fm3_exti.h"
#include "phy.h"
#include "debug.h"

extern uint32_t phy_addr;

void EXTI_Init(void)
{
	//debug_printf("EXTI_Init()\n");
	FM3_GPIO->PFRA |= 0x00000030;	// PA4, PA5を周辺ピン設定
	FM3_GPIO->EPFR06 = 0x00300000;	// INT03_0, INT10_2を選択
	FM3_EXTI->ENIR = 0x00000000;	// 外部割込み許可レジスタをクリア
	FM3_EXTI->ELVR = 0x00200080;	// 外部割込み要求レベルレジスタでINT3, INT10の 立上りエッジを検出
	FM3_EXTI->EICL = 0x00000000;	// 外部割込み要因クリアレジスタをクリア
	FM3_EXTI->ENIR = 0x00000408;	// 外部割込み許可レジスタでINT03, INT10を許可
}

void PHY_Interrupt(void)
{
	unsigned int reg23;
	unsigned int eirr = FM3_EXTI->EIRR;
	//debug_printf("EIRR %08x\n", eirr);
	FM3_EXTI->EICL &= ~eirr;								// 割込み要因のクリア
	//debug_printf("EIRR %08x\n", FM3_EXTI->EIRR);
	PHY_Read(phy_addr, PHY_REG_23, &reg23, MII_RD_TOUT);	// LINK状態の取得
	//debug_printf("PHY23 %08x\n", reg23);
	if (reg23 & 1) {
		debug_printf("PHY: Link Up\n");
	} else if (reg23 & 4) {
		debug_printf("PHY: Link Down\n");
	} else {
		debug_printf("External Interrupt\n");
	}
	PHY_Write(phy_addr, PHY_REG_23, reg23, MII_WR_TOUT);	// LINK状態のクリア
	//PHY_Read(phy_addr, PHY_REG_23, &reg23, MII_RD_TOUT);
	//debug_printf("PHY23 %08x\n", reg23);
}

void INT0_7_Handler(intptr_t exinf)
{
	PHY_Interrupt();
}

void INT8_31_Handler(intptr_t exinf)
{
	PHY_Interrupt();
}
