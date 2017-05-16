/*
 * fm3_exti.h
 *
 *  Created on: 2012/03/04
 *      Author: share
 */

#ifndef FM3_EXTI_H_
#define FM3_EXTI_H_

#define FM3_EXTI_PRIORITY	11

// PA4 - INT03_0
// P45 - INT10_2

// ENIR - 外部割込み許可レジスタ[ENIR : ENable Interrupt request Register]
// 0 外部割込み要求の発生を禁止します
// 1 外部割込み要求の発生を許可します

// EIRR - 外部割込み要因レジスタ[EIRR : External Interrupt Request Register]
// 0 外部割込み要求検出なし
// 1 外部割込み要求検出あり

// EICL - 外部割込み要因クリアレジスタ[EICL: External Interrupt CLear register]
// 0 書込み時外部割込み要因をクリアします
// 1 書込み時影響しません

// ELVR - 外部割込み要求レベルレジスタ[ELVR : External interrupt LeVel Register]
// LBx LAx 説明
// 0 0 "L"レベルを検出
// 0 1 "H"レベルを検出
// 1 0 立上りエッジを検出
// 1 1 立下りエッジを検出

// ELVR1 - 外部割込み要求レベルレジスタ1 [ELVR1 : External interrupt LeVel Register 1]
// LBx LAx 説明
// 0 0 "L"レベルを検出
// 0 1 "H"レベルを検出
// 1 0 立上りエッジを検出
// 1 1 立下りエッジを検出

// NMIRR - マスク不能割込み要因レジスタ[NMIRR : Non Maskable Interrupt Request Register]
// 0 NMI 割込み要求検出なし
// 1 NMI 割込み要求検出あり

// NIMCL - マスク不能割込み要因クリアレジスタ[NMICL: Non Maskable Interrupt CLear register]
// 0 書込み時NMI 割込み要因をクリアします
// 1 書込み時影響しません
// 読出し時常に"1"が読み出されます

void EXTI_Init(void);

#endif /* FM3_EXTI_H_ */
