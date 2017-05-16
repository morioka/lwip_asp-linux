/*
 *	TOPPERS/ASP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2007-2011 by Industrial Technology Institute,
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
 *	$Id: sh7262.h 2156 2011-07-19 06:03:20Z mit-kimai $
 */

/*
 *	SH7262のハードウェア資源の定義
 */

#ifndef TOPPERS_SH7262_H
#define TOPPERS_SH7262_H

/*
 *	例外ベクタ
 */
#define POWER_ON_RESET_VECTOR				0
#define MANUAL_RESET_VECTOR					2
#define GENERAL_ILLEGAL_INSTRUCTION_VECTOR	4
#define RAM_ERROR_VECTOR					5
#define SLOT_ILLEGAL_INSTRUCTION_VECTOR		6
#define CPU_ADDRESS_ERROR_VECTOR			9
#define DMA_ADDRESS_ERROR_VECTOR			10
#define FPU_VECTOR							13
#define BANK_OVER_FLOW_VECTOR				15
#define BANK_UNDER_FLOW_VECTOR				16
#define DIVIDE_BY_ZERO_VECTOR				17

/*
 *	割込みベクタ
 */
#define NMI_VECTOR 	   				11
#define IRQ0_VECTOR 	   			64
#define IRQ1_VECTOR 	   			65
#define IRQ2_VECTOR 	   			66
#define IRQ3_VECTOR 	   			67
#define IRQ4_VECTOR 	   			68
#define IRQ5_VECTOR 	   			69
#define IRQ6_VECTOR 	   			70
#define IRQ7_VECTOR    				71

#define PINT0_VECTOR				80
#define PINT1_VECTOR				81
#define PINT2_VECTOR				82
#define PINT3_VECTOR				83
#define PINT4_VECTOR				84
#define PINT5_VECTOR				85
#define PINT6_VECTOR				86
#define PINT7_VECTOR				87

#define DMAC0_TEI0_VECTOR		 	108
#define DMAC0_HEI0_VECTOR		 	109
#define DMAC1_TEI1_VECTOR		 	112
#define DMAC1_HEI1_VECTOR		 	113
#define DMAC2_TEI2_VECTOR		 	116
#define DMAC2_HEI2_VECTOR		 	117
#define DMAC3_TEI3_VECTOR		 	120
#define DMAC3_HEI3_VECTOR		 	121
#define DMAC4_TEI4_VECTOR		 	124
#define DMAC4_HEI4_VECTOR		 	125
#define DMAC5_TEI5_VECTOR		 	128
#define DMAC5_HEI5_VECTOR		 	129
#define DMAC6_TEI6_VECTOR		 	132
#define DMAC6_HEI6_VECTOR		 	133
#define DMAC7_TEI7_VECTOR		 	136
#define DMAC7_HEI7_VECTOR		 	137
#define DMAC7_TEI8_VECTOR			140
#define DMAC7_HEI8_VECTOR			141
#define DMAC7_TEI9_VECTOR			144
#define DMAC7_HEI9_VECTOR			145
#define DMAC7_TEI10_VECTOR			148
#define DMAC7_HEI10_VECTOR			149
#define DMAC7_TEI11_VECTOR			152
#define DMAC7_HEI11_VECTOR			153
#define DMAC7_TEI12_VECTOR			156
#define DMAC7_HEI12_VECTOR			157
#define DMAC7_TEI13_VECTOR			160
#define DMAC7_HEI13_VECTOR			161
#define DMAC7_TEI14_VECTOR			164
#define DMAC7_HEI14_VECTOR			165
#define DMAC7_TEI15_VECTOR			168
#define DMAC7_HEI15_VECTOR			169

#define USB_USBI_VECTOR				170

#define VDC_VIVSYNCJ_VECTOR			171
#define VDC_VBUFERR_VECTOR			172
#define VDC_VIFIELDE_VECTOR			173
#define VDC_VOLINE_VECTOR			174

#define CMI0_VECTOR 	 			175
#define CMI1_VECTOR 	 			176
#define CMI_VECTOR		 			177

#define WDT_ITI_VECTOR	 			178

#define MTU0_TGI0A_VECTOR		 	179
#define MTU0_TGI0B_VECTOR		 	180
#define MTU0_TGI0C_VECTOR		 	181
#define MTU0_TGI0D_VECTOR		 	182
#define MTU0_TGI0V_VECTOR		 	183
#define MTU0_TGI0E_VECTOR		 	184
#define MTU0_TGI0F_VECTOR		 	185

#define MTU1_TGI1A_VECTOR		 	186
#define MTU1_TGI1B_VECTOR		 	187
#define MTU1_TGI1V_VECTOR		 	188
#define MTU1_TGI1U_VECTOR		 	189

#define MTU2_TGI2A_VECTOR		 	190
#define MTU2_TGI2B_VECTOR		 	191
#define MTU2_TGI2V_VECTOR		 	192
#define MTU2_TGI2U_VECTOR		 	193

#define MTU3_TGI3A_VECTOR		 	194
#define MTU3_TGI3B_VECTOR		 	195
#define MTU3_TGI3C_VECTOR		 	196
#define MTU3_TGI3D_VECTOR		 	197
#define MTU3_TGI3V_VECTOR		 	198

#define MTU4_TGI4A_VECTOR		 	199
#define MTU4_TGI4B_VECTOR		 	200
#define MTU4_TGI4C_VECTOR		 	201
#define MTU4_TGI4D_VECTOR		 	202
#define MTU4_TGI4V_VECTOR		 	203

#define PWM0_VECTOR					204
#define PWM1_VECTOR					205

#define AD_ADI_VECTOR				206

#define SSIF0_SSIF_VECTOR			207
#define SSIF0_SSIRXI_VECTOR			208
#define SSIF0_SSITXI_VECTOR			209
#define SSIF1_SSII_VECTOR			210
#define SSIF1_SSIRTI_VECTOR			211
#define SSIF2_SSII_VECTOR			212
#define SSIF2_SSIRTI_VECTOR			213
#define SSIF3_SSII_VECTOR			214
#define SSIF3_SSIRTI_VECTOR			215

#define SPDIF_SPDIFI_VECTOR			216

#define I2C0_STPI_VECTOR			217
#define I2C0_NAKI_VECTOR			218
#define I2C0_RXI_VECTOR				219
#define I2C0_TXI_VECTOR				220
#define I2C0_TEI_VECTOR				221
#define I2C1_STPI_VECTOR			222
#define I2C1_NAKI_VECTOR			223
#define I2C1_RXI_VECTOR				224
#define I2C1_TXI_VECTOR				225
#define I2C1_TEI_VECTOR				226
#define I2C2_STPI_VECTOR			227
#define I2C2_NAKI_VECTOR			228
#define I2C2_RXI_VECTOR				229
#define I2C2_TXI_VECTOR				230
#define I2C2_TEI_VECTOR				231

#define SCIF0_BRI_VECTOR   			232
#define SCIF0_ERI_VECTOR   			233
#define SCIF0_RXI_VECTOR   			234
#define SCIF0_TXI_VECTOR   			235

#define SCIF1_BRI_VECTOR   			236
#define SCIF1_ERI_VECTOR   			237
#define SCIF1_RXI_VECTOR   			238
#define SCIF1_TXI_VECTOR   			239

#define SCIF2_BRI_VECTOR   			240
#define SCIF2_ERI_VECTOR   			241
#define SCIF2_RXI_VECTOR   			242
#define SCIF2_TXI_VECTOR   			243

#define SCIF3_BRI_VECTOR   			244
#define SCIF3_ERI_VECTOR   			245
#define SCIF3_RXI_VECTOR   			246
#define SCIF3_TXI_VECTOR   			247

#define SCIF4_BRI_VECTOR			248
#define SCIF4_ERI_VECTOR			249
#define SCIF4_RXI_VECTOR			250
#define SCIF4_TXI_VECTOR			251

#define SCIF5_BRI_VECTOR			252
#define SCIF5_ERI_VECTOR			253
#define SCIF5_RXI_VECTOR			254
#define SCIF5_TXI_VECTOR			255

#define SCIF6_BRI_VECTOR			256
#define SCIF6_ERI_VECTOR			257
#define SCIF6_RXI_VECTOR			258
#define SCIF6_TXI_VECTOR			259

#define SCIF7_BRI_VECTOR			260
#define SCIF7_ERI_VECTOR			261
#define SCIF7_RXI_VECTOR			262
#define SCIF7_TXI_VECTOR			263

#define SIOF_SIOF_VECTOR			264

#define SPI0_SPEI_VECTOR			265
#define SPI0_SPRI_VECTOR			266
#define SPI0_SPTI_VECTOR			267
#define SPI1_SPEI_VECTOR			268
#define SPI1_SPRI_VECTOR			269
#define SPI1_SPTI_VECTOR			270

#define CAN0_ERS_VECTOR				271
#define CAN0_OVR_VECTOR				272
#define CAN0_RM0_VECTOR				273
#define CAN0_RM1_VECTOR				274
#define CAN0_SLE_VECTOR				275
#define CAN1_ERS_VECTOR				276
#define CAN1_OVR_VECTOR				277
#define CAN1_RM0_VECTOR				278
#define CAN1_RM1_VECTOR				279
#define CAN1_SLE_VECTOR				280

#define IEB_VECTOR					281

#define CDD_ISY_VECTOR				282
#define CDD_IERR_VECTOR				283
#define CDD_ITARG_VECTOR			284
#define CDD_ISEC_VECTOR				285
#define CDD_IBUF_VECTOR				286
#define CDD_IREADY_VECTOR			287

#define FLMC_STEI_VECTOR			288
#define FLMC_TENDI_VECTOR			289
#define FLMC_TREQ0I_VECTOR			290
#define FLMC_TREQ1I_VECTOR			291

#define SDHIF_SDHI3_VECTOR			292
#define SDHIF_SDHI0_VECTOR			293
#define SDHIF_SDHI1_VECTOR			294

#define RTC_ARM_VECTOR				296
#define RTC_PRD_VECTOR				297
#define RTC_ACUP_VECTOR				298

#define SRC0_OVF_VECTOR				299
#define SRC0_UDF_VECTOR				300
#define SRC0_CEF_VECTOR				301
#define SRC0_ODFI_VECTOR			302
#define SRC0_IDEI_VECTOR			303
#define SRC1_OVF_VECTOR				304
#define SRC1_UDF_VECTOR				305
#define SRC1_CEF_VECTOR				306
#define SRC1_ODFI_VECTOR			307
#define SRC1_IDEI_VECTOR			308

#define DCU_DCUEI_VECTOR			310
#define DCU_OFFI_VECTOR				311
#define DCU_IFEI_VECTOR				312
/*
 * 割込みコントローラレジスタ（INTC）
 */
#define ICR0_h	UINT_C(0xfffe0800)
#define ICR1_h	UINT_C(0xfffe0802)
#define IRQRR_h	UINT_C(0xfffe0806)
#define IBCR_h	UINT_C(0xfffe080c)
#define IBNR_h	UINT_C(0xfffe080e)
 #define IBNR_BE0	 UINT_C(0x4000)
 #define IBNR_BOVE	 UINT_C(0x2000)

#define IPR01_h	UINT_C(0xfffe0818)
#define IPR02_h	UINT_C(0xfffe081a)
#define IPR05_h	UINT_C(0xfffe0820)
#define IPR06_h	UINT_C(0xfffe0c00)
#define IPR07_h	UINT_C(0xfffe0c02)
#define IPR08_h	UINT_C(0xfffe0c04)
#define IPR09_h	UINT_C(0xfffe0c06)
#define IPR10_h	UINT_C(0xfffe0c08)
#define IPR11_h	UINT_C(0xfffe0c0a)
#define IPR12_h	UINT_C(0xfffe0c0c)
#define IPR13_h	UINT_C(0xfffe0c0e)
#define IPR14_h	UINT_C(0xfffe0c10)
#define IPR15_h	UINT_C(0xfffe0c12)
#define IPR16_h	UINT_C(0xfffe0c14)
#define IPR17_h	UINT_C(0xfffe0c16)
#define IPR18_h	UINT_C(0xfffe0c18)
#define IPR19_h	UINT_C(0xfffe0c1a)
#define IPR20_h	UINT_C(0xfffe0c1c)
#define IPR21_h	UINT_C(0xfffe0c1e)
#define IPR22_h	UINT_C(0xfffe0c20)

#define IRQ_POSEDGE   UINT_C(0x02)
#define IRQ_NEGEDGE   UINT_C(0x01)

#define INTC_BASE	ICR0_w	/*	ベースアドレス	*/

/*
 *	コンペアマッチ・タイマ（CMT）
 */
#define CMSTR_h     UINT_C(0XFFFEC000)
#define CMCSR_0_h   UINT_C(0XFFFEC002)
#define CMCNT_0_h   UINT_C(0XFFFEC004)
#define CMCOR_0_h   UINT_C(0XFFFEC006)
#define CMCSR_1_h   UINT_C(0XFFFEC008)
#define CMCNT_1_h   UINT_C(0XFFFEC00A)
#define CMCOR_1_h   UINT_C(0XFFFEC00C)

#define CMSTR_STR0	 UINT_C(0x0001)
#define CMSTR_STR1	 UINT_C(0x0002)
#define CMCSR_CMF	 UINT_C(0x0080)
#define CMCSR_CMIE	 UINT_C(0x0040)

#define CMT_BASE	CMSTR_h	/*	ベースアドレス	*/

/*
 *	FIFO付きシリアルコミュニケーションインターフェース(SCIF)レジスタ
 */
#define SH_SCIF0_BASE	UINT_C(0xFFFE8000)	/*	ベースアドレス	*/
#define SH_SCIF1_BASE	UINT_C(0xFFFE8800)
#define SH_SCIF2_BASE	UINT_C(0xFFFE9000)
#define SH_SCIF3_BASE	UINT_C(0xFFFE9800)
#define SH_SCIF4_BASE	UINT_C(0xFFFEA000)
#define SH_SCIF5_BASE	UINT_C(0xFFFEA800)
#define SH_SCIF6_BASE	UINT_C(0xFFFEB000)
#define SH_SCIF7_BASE	UINT_C(0xFFFEB800)

/*
 *	ピンファンクションコントローラ（PFC）
 */
#define PAIOR0	 UINT_C(0xFFFE3812)
#define PADR1	 UINT_C(0xFFFE3814)
#define PADR0	 UINT_C(0xFFFE3816)

#define PBCR5	 UINT_C(0xfffe3824)
#define PBCR4	 UINT_C(0xfffe3826)
#define PBCR3	 UINT_C(0xfffe3828)
#define PBCR2	 UINT_C(0xfffe382a)
#define PBCR1	 UINT_C(0xfffe382c)
#define PBCR0	 UINT_C(0xfffe382e)
#define PBIOR1	 UINT_C(0xfffe3830)
#define PBIOR0	 UINT_C(0xfffe3832)
#define PBDR1	 UINT_C(0xfffe3834)
#define PBDR0	 UINT_C(0xfffe3836)
#define PBPR1	 UINT_C(0xfffe3838)
#define PBPR0	 UINT_C(0xfffe383A)

#define PCCR2	 UINT_C(0xfffe384a)
#define PCCR1	 UINT_C(0xfffe384c)
#define PCCR0	 UINT_C(0xfffe384e)
#define PCIOR0	 UINT_C(0xfffe3852)
#define PCDR0	 UINT_C(0xfffe3856)
#define PCPR0	 UINT_C(0xfffe385A)

#define PDCR3	 UINT_C(0xfffe3868)
#define PDCR2	 UINT_C(0xfffe386a)
#define PDCR1	 UINT_C(0xfffe386c)
#define PDCR0	 UINT_C(0xfffe386e)
#define PDIOR0	 UINT_C(0xfffe3872)
#define PDDR0	 UINT_C(0xfffe3876)
#define PDPR0	 UINT_C(0xfffe387a)

#define PECR1	 UINT_C(0xfffe388c)
#define PECR0	 UINT_C(0xfffe388e)
#define PEIOR0	 UINT_C(0xfffe3892)
#define PEDR0	 UINT_C(0xfffe3896)
#define PEPR0	 UINT_C(0xfffe389a)

#define PFCR3	 UINT_C(0xfffe38a8)
#define PFCR2	 UINT_C(0xfffe38aa)
#define PFCR1	 UINT_C(0xfffe38ac)
#define PFCR0	 UINT_C(0xfffe38ae)
#define PFIOR0	 UINT_C(0xfffe38b2)
#define PFDR0	 UINT_C(0xfffe38b6)
#define PFPR0	 UINT_C(0xfffe38ba)

#define PGCR7	 UINT_C(0xfffe38c0)
#define PGCR6	 UINT_C(0xfffe38c2)
#define PGCR5	 UINT_C(0xfffe38c4)
#define PGCR4	 UINT_C(0xfffe38c6)
#define PGCR3	 UINT_C(0xfffe38c8)
#define PGCR2	 UINT_C(0xfffe38ca)
#define PGCR1	 UINT_C(0xfffe38cc)
#define PGCR0	 UINT_C(0xfffe38ce)
#define PGIOR1	 UINT_C(0xfffe38d0)
#define PGIOR0	 UINT_C(0xfffe38d2)
#define PGDR1	 UINT_C(0xfffe38d4)
#define PGDR0	 UINT_C(0xfffe38d6)
#define PGPR1	 UINT_C(0xfffe38d8)
#define PGPR0	 UINT_C(0xfffe38da)

#define PHCR1	 UINT_C(0xfffe38ec)
#define PHCR0	 UINT_C(0xfffe38ee)
#define PHPR0	 UINT_C(0xfffe38fa)

#define PJCR2	 UINT_C(0xfffe390a)
#define PJCR1	 UINT_C(0xfffe390c)
#define PJCR0	 UINT_C(0xfffe390e)
#define PJIOR0	 UINT_C(0xfffe3912)
#define PJDR0	 UINT_C(0xfffe3916)
#define PJPR0	 UINT_C(0xfffe391a)

#define PKCR2	 UINT_C(0xfffe392a)
#define PKCR1	 UINT_C(0xfffe392c)
#define PKCR0	 UINT_C(0xfffe392e)
#define PKIOR0	 UINT_C(0xfffe3932)
#define PKDR0	 UINT_C(0xfffe3936)
#define PKPR0	 UINT_C(0xfffe393a)

#define PFC_BASE	PAIOR0	/*	ベースアドレス	*/

/*
 *	クロックパルス発振器（CPG）
 */
#define FRQCR_h		UINT_C(0xfffe0010)

/*
 *	低消費電力モード関連（PDM：Power-Down Modes）
 */
#define STBCR_b			UINT_C(0xfffe0014)
#define STBCR2_b		UINT_C(0xfffe0018)
 #define STBCR2_HUDI	UINT_C(0x80)
 #define STBCR2_DMAC	UINT_C(0x20)
 #define STBCR2_FPU		UINT_C(0x10)
#define STBCR3_b		UINT_C(0xfffe0408)
 #define STBCR3_HZ		UINT_C(0x80)
 #define STBCR3_IEBC	UINT_C(0x40)
 #define STBCR3_MTU2	UINT_C(0x20)
 #define STBCR3_SDHC0	UINT_C(0x10)
 #define STBCR3_SDHC1	UINT_C(0x08)
 #define STBCR3_ADC		UINT_C(0x04)
 #define STBCR3_RTC		UINT_C(0x01)
#define STBCR4_b		UINT_C(0xfffe040C)
 #define STBCR4_SCIF0	UINT_C(0x80)
 #define STBCR4_SCIF1	UINT_C(0x40)
 #define STBCR4_SCIF2	UINT_C(0x20)
 #define STBCR4_SCIF3	UINT_C(0x10)
 #define STBCR4_SCIF4	UINT_C(0x08)
 #define STBCR4_SCIF5	UINT_C(0x04)
 #define STBCR4_SCIF6	UINT_C(0x02)
 #define STBCR4_SCIF7	UINT_C(0x01)
#define STBCR5_b	UINT_C(0xfffe0410)
#define STBCR6_b	UINT_C(0xfffe0414)
#define STBCR7_b	UINT_C(0xfffe0418)
 #define STBCR7_SIOF	UINT_C(0x80)
 #define STBCR7_SPDIF	UINT_C(0x40)
 #define STBCR7_VDC3	UINT_C(0x10)
 #define STBCR7_CMT		UINT_C(0x04)
 #define STBCR7_FLC		UINT_C(0x01)

#define SYSCR1_b		UINT_C(0xfffe0402)
#define SYSCR2_b		UINT_C(0xfffe0404)

#define PDM_BASE	STBCR_b		/*	ベースアドレス	*/
#define PDM_BASE2	SYSCR1_b	/*	ベースアドレス	*/

/*
 *	バスステートコントローラ（BSC）
 */
#define CMNCR_w		UINT_C(0xfffc0000)
#define CS3BCR_w	UINT_C(0xfffc0010)
#define CS3WCR_w	UINT_C(0xfffc0034)
#define SDCR_w		UINT_C(0xfffc004c)
#define RTCSR_w		UINT_C(0xfffc0050)
#define RTCOR_w		UINT_C(0xfffc0058)

#define BSC_BASE	CMNCR_w	/*	ベースアドレス	*/

/*
 *	内蔵RAM
 */
#define INNER_RAM_END		UINT_C(0xfff89000)


/*
 *  マルチファンクションタイマパルスユニットMTU2
 */

/*
 *  共通
 */
#define TSTR_b		0xfffe4280U
#define TSTR_CST0	0x01U
#define TSTR_CST1	0x02U
#define TSTR_CST2	0x04U
#define TSTR_CST3	0x40U
#define TSTR_CST4	0x80U


/*
 *  チャネル0
 */
#define TCR_0_b		0xfffe4300U
#define TIER_0_b	0xfffe4304U
#define TSR_0_b		0xfffe4305U
#define TCNT_0_h	0xfffe4306U

/*
 *  チャネル1
 */
#define TCR_1_b		0xfffe4380U
#define TIER_1_b	0xfffe4384U
#define TSR_1_b		0xfffe4385U
#define TCNT_1_h	0xfffe4386U

/*
 *  チャネル2
 */
#define TCR_2_b		0xfffe4000U
#define TIER_2_b	0xfffe4004U
#define TSR_2_b		0xfffe4005U
#define TCNT_2_h	0xfffe4006U

/*
 *  チャネル3
 */
#define TCR_3_b		0xfffe4200U
#define TIER_3_b	0xfffe4208U
#define TSR_3_b		0xfffe422cU
#define TCNT_3_h	0xfffe4210U

/*
 *  チャネル4
 */
#define TCR_4_b		0xfffe4201U
#define TIER_4_b	0xfffe4209U
#define TSR_4_b		0xfffe422dU
#define TCNT_4_h	0xfffe4212U


/*  タイマステータスレジスタ（TSR）  */
#define TCR_TCFV	0x10U	/*  オーバフローフラグ  */

/*  タイマインタラプトイネーブルレジスタ（TIER）  */
#define TIER_TCIEV	0x10U	/*  オーバフローインタラプトイネーブル  */


#endif /* TOPPERS_SH7262_H */
