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
 *	$Id: sh7262_config.h 2142 2011-06-27 07:24:40Z mit-kimai $
 */

/*
 *		プロセッサ依存モジュール（SH7262用）
 *
 *	このインクルードファイルは，target_config.h（または，そこからインク
 *	ルードされるファイル）のみからインクルードされる．他のファイルから
 *	直接インクルードしてはならない．
 */

#ifndef TOPPERS_SH7262_CONFIG_H
#define TOPPERS_SH7262_CONFIG_H

#ifndef TOPPERS_TARGET_CONFIG_H
#error  target_config.h is not included!
#endif	/*  TOPPERS_TARGET_CONFIG_H  */

/*
 * CPU例外番号に関する定義
 */
#define TMIN_EXCNO		UINT_C(4)		/*	GENERAL_ILLEGAL_INSTRUCTION_VECTOR	*/
#define TMAX_EXCNO		UINT_C(63)		/*	TRAPA_INST_VECTOR  */
#define TNUM_EXC		(TMAX_EXCNO - TMIN_EXCNO + 1U)

/*
 * 割込みハンドラ番号に関する定義
 */
#define TMIN_INHNO		UINT_C(64)
#define TMAX_INHNO		UINT_C(312)
#define TNUM_INH		(TMAX_INHNO - TMIN_INHNO + 1U)

/*
 * 割込み番号に関する定義
 */
#define TMIN_INTNO		UINT_C(64)
#define TMAX_INTNO		UINT_C(312)
#define TNUM_INT		(TMAX_INTNO - TMIN_INTNO + 1U)

/*	IRQ  */
#define TMIN_INTNO_IRQ 	64U
#define TMAX_INTNO_IRQ	71U

/*
 *	割込み要求ライン毎の優先度設定レジスタの設定情報
 */
#define IPR_INFO_TBL_DATA	\
	{IPR01_h, 12U}, /*  64 IRQ0 */ \
	{IPR01_h,  8U}, /*  65 IRQ1 */ \
	{IPR01_h,  4U}, /*  66 IRQ2 */ \
	{IPR01_h,  0U}, /*  67 IRQ3 */ \
	{IPR02_h, 12U}, /*  68 IRQ4 */ \
	{IPR02_h,  8U}, /*  69 IRQ5 */ \
	{IPR02_h,  4U}, /*  70 IRQ6 */ \
	{IPR02_h,  0U}, /*  71 IRQ7 */ \
\
	{0U, 0U},	  /*  72 */ \
	{0U, 0U},	  /*  73 */ \
	{0U, 0U},	  /*  74 */ \
	{0U, 0U},	  /*  75 */ \
	{0U, 0U},	  /*  76 */ \
	{0U, 0U},	  /*  77 */ \
	{0U, 0U},	  /*  78 */ \
	{0U, 0U},	  /*  79 */ \
\
	{IPR05_h, 12U}, /*  80 PINT0 */ \
	{IPR05_h, 12U}, /*  81 PINT1 */ \
	{IPR05_h, 12U}, /*  82 PINT2 */ \
	{IPR05_h, 12U}, /*  83 PINT3 */ \
	{IPR05_h, 12U}, /*  84 PINT4 */ \
	{IPR05_h, 12U}, /*  85 PINT5 */ \
	{IPR05_h, 12U}, /*  86 PINT6 */ \
	{IPR05_h, 12U}, /*  87 PINT7 */ \
\
	{0U, 0U},	  /*  88 */ \
	{0U, 0U},	  /*  89 */ \
	{0U, 0U},	  /*  90 */ \
	{0U, 0U},	  /*  91 */ \
	{0U, 4U},	  /*  92 */ \
	{0U, 0U},	  /*  93 */ \
	{0U, 0U},	  /*  94 */ \
	{0U, 0U},	  /*  95 */ \
	{0U, 0U},	  /*  96 */ \
	{0U, 0U},	  /*  97 */ \
	{0U, 0U},	  /*  98 */ \
	{0U, 0U},	  /*  99 */ \
	{0U, 0U},	  /* 100 */ \
	{0U, 0U},	  /* 101 */ \
	{0U, 0U},	  /* 102 */ \
	{0U, 0U},	  /* 103 */ \
	{0U, 0U},	  /* 104 */ \
	{0U, 0U},	  /* 105 */ \
	{0U, 0U},	  /* 106 */ \
	{0U, 0U},	  /* 107 */ \
\
	{IPR06_h, 12U}, /* 108 DMAC0 TEI0 */ \
	{IPR06_h, 12U}, /* 109 DMAC0 HEI0 */ \
\
	{0U, 0U},	  /* 110 */ \
	{0U, 0U},	  /* 111 */ \
\
	{IPR06_h,  8U}, /* 112 DMAC1 TEI1 */ \
	{IPR06_h,  8U}, /* 113 DMAC1 HEI1 */ \
\
	{0U, 0U},	  /* 114 */ \
	{0U, 0U},	  /* 115 */ \
\
	{IPR06_h,  4U}, /* 116 DMAC2 TEI2 */ \
	{IPR06_h,  4U}, /* 117 DMAC2 HEI2 */ \
\
	{0U, 0U},	  /* 118 */ \
	{0U, 0U},	  /* 119 */ \
\
	{IPR06_h,  0U}, /* 120 DMAC3 TEI3 */ \
	{IPR06_h,  0U}, /* 121 DMAC3 HEI3 */ \
\
	{0U, 0U},	  /* 122 */ \
	{0U, 0U},	  /* 123 */ \
\
	{IPR07_h, 12U}, /* 124 DMAC4 TEI4 */ \
	{IPR07_h, 12U}, /* 125 DMAC4 HEI4 */ \
\
	{0U, 0U},	  /* 126 */ \
	{0U, 0U},	  /* 127 */ \
\
	{IPR07_h,  8U}, /* 128 DMAC5 TEI5 */ \
	{IPR07_h,  8U}, /* 129 DMAC5 HEI5 */ \
\
	{0U, 0U},	  /* 130 */ \
	{0U, 0U},	  /* 131 */ \
\
	{IPR07_h,  4U}, /* 132 DMAC6 TEI6 */ \
	{IPR07_h,  4U}, /* 133 DMAC6 HEI6 */ \
\
	{0U, 0U},	  /* 134 */ \
	{0U, 0U},	  /* 135 */ \
\
	{IPR07_h,  0U}, /* 136 DMAC7 TEI7 */ \
	{IPR07_h,  0U}, /* 137 DMAC7 HEI7 */ \
\
	{0U, 0U},	  /* 138 */ \
	{0U, 0U},	  /* 139 */ \
\
	{IPR08_h, 12U}, /* 140 DMAC8 TEI8 */ \
	{IPR08_h, 12U}, /* 141 DMAC8 HEI8 */ \
\
	{0U, 0U},	  /* 142 */ \
	{0U, 0U},	  /* 143 */ \
\
	{IPR08_h,  8U}, /* 144 DMAC9 TEI9 */ \
	{IPR08_h,  8U}, /* 145 DMAC9 HEI9 */ \
\
	{0U, 0U},	  /* 146 */ \
	{0U, 0U},	  /* 147 */ \
\
	{IPR08_h,  4U}, /* 148 DMAC10 TEI10 */ \
	{IPR08_h,  4U}, /* 149 DMAC10 HEI10 */ \
\
	{0U, 0U},	  /* 150 */ \
	{0U, 0U},	  /* 151 */ \
\
	{IPR08_h,  0U}, /* 152 DMAC11 TEI11 */ \
	{IPR08_h,  0U}, /* 153 DMAC11 HEI11 */ \
\
	{0U, 0U},	  /* 154 */ \
	{0U, 0U},	  /* 155 */ \
\
	{IPR09_h, 12U}, /* 156 DMAC12 TEI12 */ \
	{IPR09_h, 12U}, /* 157 DMAC12 HEI12 */ \
\
	{0U, 0U},	  /* 158 */ \
	{0U, 0U},	  /* 159 */ \
\
	{IPR09_h,  8U}, /* 160 DMAC13 TEI13 */ \
	{IPR09_h,  8U}, /* 161 DMAC13 HEI13 */ \
\
	{0U, 0U},	  /* 162 */ \
	{0U, 0U},	  /* 163 */ \
\
	{IPR09_h,  4U}, /* 164 DMAC14 TEI14 */ \
	{IPR09_h,  4U}, /* 165 DMAC14 HEI14 */ \
\
	{0U, 0U},	  /* 166 */ \
	{0U, 0U},	  /* 167 */ \
\
	{IPR09_h,  0U}, /* 168 DMAC15 TEI15 */ \
	{IPR09_h,  0U}, /* 169 DMAC15 HEI15 */ \
\
	{IPR10_h, 12U}, /* 170 USB_USBI */ \
\
	{IPR10_h,  8U}, /* 171 VDC_VIVSYNCJ */ \
	{IPR10_h,  8U}, /* 172 VDC_VBUFERR */ \
	{IPR10_h,  8U}, /* 173 VDC_VIFIELDE */ \
	{IPR10_h,  8U}, /* 174 VDC_VOLINE */ \
\
	{IPR10_h,  4U}, /* 175 CMI0 */ \
	{IPR10_h,  0U}, /* 176 CMI1 */ \
	{IPR11_h, 12U}, /* 177 CMI */ \
\
	{IPR11_h,  8U}, /* 178 WDT_ITI */ \
\
	{IPR11_h,  4U}, /* 179 MTU0_TGI0A */ \
	{IPR11_h,  4U}, /* 180 MTU0_TGI0B */ \
	{IPR11_h,  4U}, /* 181 MTU0_TGI0C */ \
	{IPR11_h,  4U}, /* 182 MTU0_TGI0D */ \
	{IPR11_h,  0U}, /* 183 MTU0_TGI0V */ \
	{IPR11_h,  0U}, /* 184 MTU0_TGI0E */ \
	{IPR11_h,  0U}, /* 185 MTU0_TGI0F */ \
\
	{IPR12_h, 12U}, /* 186 MTU1_TGI1A */ \
	{IPR12_h, 12U}, /* 187 MTU1_TGI1B */ \
	{IPR12_h,  8U}, /* 188 MTU1_TGI1V */ \
	{IPR12_h,  8U}, /* 189 MTU1_TGI1U */ \
\
	{IPR12_h,  4U}, /* 190 MTU2_TGI2A */ \
	{IPR12_h,  4U}, /* 191 MTU2_TGI2B */ \
	{IPR12_h,  0U}, /* 192 MTU2_TGI2V */ \
	{IPR12_h,  0U}, /* 193 MTU2_TGI2U */ \
\
	{IPR13_h, 12U}, /* 194 MTU3_TGI3A */ \
	{IPR13_h, 12U}, /* 195 MTU3_TGI3B */ \
	{IPR13_h, 12U}, /* 196 MTU3_TGI3C */ \
	{IPR13_h, 12U}, /* 197 MTU3_TGI3D */ \
	{IPR13_h,  8U}, /* 198 MTU3_TGI3V */ \
\
	{IPR13_h,  4U}, /* 199 MTU4_TGI4A */ \
	{IPR13_h,  4U}, /* 200 MTU4_TGI4B */ \
	{IPR13_h,  4U}, /* 201 MTU4_TGI4C */ \
	{IPR13_h,  4U}, /* 202 MTU4_TGI4D */ \
	{IPR13_h,  0U}, /* 203 MTU4_TGI4V */ \
\
	{IPR14_h, 12U}, /* 204 PWM0 */ \
	{IPR14_h,  8U}, /* 205 PWM1 */ \
\
	{IPR14_h,  4U}, /* 206 AD_ADI */ \
\
	{IPR15_h, 12U}, /* 207 SSIF0_SSIF */ \
	{IPR15_h, 12U}, /* 208 SSIF0_SSIRXI */ \
	{IPR15_h, 12U}, /* 209 SSIF0_SSITXI */ \
	{IPR15_h,  8U}, /* 210 SSIF1_SSII */ \
	{IPR15_h,  8U}, /* 211 SSIF1_SSIRTI */ \
	{IPR15_h,  4U}, /* 212 SSIF2_SSII */ \
	{IPR15_h,  4U}, /* 213 SSIF2_SSIRTI */ \
	{IPR15_h,  0U}, /* 214 SSIF3_SSII */ \
	{IPR15_h,  0U}, /* 215 SSIF3_SSIRTI */ \
\
	{IPR16_h, 12U}, /* 216 SPDIF_SPDIFI */ \
\
	{IPR16_h,  8U}, /* 217 I2C0_STPI */ \
	{IPR16_h,  8U}, /* 218 I2C0_NAKI */ \
	{IPR16_h,  8U}, /* 219 I2C0_RXI */ \
	{IPR13_h,  8U}, /* 220 I2C0_TXI */ \
	{IPR13_h,  8U}, /* 221 I2C0_TEI */ \
	{IPR13_h,  4U}, /* 222 I2C1_STPI */ \
	{IPR16_h,  4U}, /* 223 I2C1_NAKI */ \
	{IPR13_h,  4U}, /* 224 I2C1_RXI */ \
	{IPR16_h,  4U}, /* 225 I2C1_TXI */ \
	{IPR16_h,  4U}, /* 226 I2C1_TEI */ \
	{IPR16_h,  0U}, /* 227 I2C2_STPI */ \
	{IPR16_h,  0U}, /* 228 I2C2_NAKI */ \
	{IPR16_h,  0U}, /* 229 I2C2_RXI */ \
	{IPR16_h,  0U}, /* 230 I2C2_TXI */ \
	{IPR16_h,  0U}, /* 231 I2C2_TEI */ \
\
	{IPR17_h, 12U}, /* 232 SCIF0_BRI */ \
	{IPR17_h, 12U}, /* 233 SCIF0_ERI */ \
	{IPR17_h, 12U}, /* 234 SCIF0_RXI */ \
	{IPR17_h, 12U}, /* 235 SCIF0_TXI */ \
\
	{IPR17_h,  8U}, /* 236 SCIF1_BRI */ \
	{IPR17_h,  8U}, /* 237 SCIF1_ERI */ \
	{IPR17_h,  8U}, /* 238 SCIF1_RXI */ \
	{IPR17_h,  8U}, /* 239 SCIF1_TXI */ \
\
	{IPR17_h,  4U}, /* 240 SCIF2_BRI */ \
	{IPR17_h,  4U}, /* 241 SCIF2_ERI */ \
	{IPR17_h,  4U}, /* 242 SCIF2_RXI */ \
	{IPR17_h,  4U}, /* 243 SCIF2_TXI */ \
\
	{IPR17_h,  0U}, /* 244 SCIF3_BRI */ \
	{IPR17_h,  0U}, /* 245 SCIF3_ERI */ \
	{IPR17_h,  0U}, /* 246 SCIF3_RXI */ \
	{IPR17_h,  0U}, /* 247 SCIF3_TXI */ \
\
	{IPR18_h, 12U}, /* 248 SCIF4_BRI */ \
	{IPR18_h, 12U}, /* 249 SCIF4_ERI */ \
	{IPR18_h, 12U}, /* 250 SCIF4_RXI */ \
	{IPR18_h, 12U}, /* 251 SCIF4_TXI */ \
\
	{IPR18_h,  8U}, /* 252 SCIF5_BRI */ \
	{IPR18_h,  8U}, /* 253 SCIF5_ERI */ \
	{IPR18_h,  8U}, /* 254 SCIF5_RXI */ \
	{IPR18_h,  8U}, /* 255 SCIF5_TXI */ \
\
	{IPR18_h,  4U}, /* 256 SCIF6_BRI */ \
	{IPR18_h,  4U}, /* 257 SCIF6_ERI */ \
	{IPR18_h,  4U}, /* 258 SCIF6_RXI */ \
	{IPR18_h,  4U}, /* 259 SCIF6_TXI */ \
\
	{IPR18_h,  0U}, /* 260 SCIF7_BRI */ \
	{IPR18_h,  0U}, /* 261 SCIF7_ERI */ \
	{IPR18_h,  0U}, /* 262 SCIF7_RXI */ \
	{IPR18_h,  0U}, /* 263 SCIF7_TXI */ \
\
	{IPR19_h, 12U}, /* 264 SIOF_SIOF */ \
\
	{IPR19_h,  4U}, /* 265 SPI0_SPEI */ \
	{IPR19_h,  4U}, /* 266 SPI0_SPRI */ \
	{IPR19_h,  4U}, /* 267 SPI0_SPTI */ \
	{IPR19_h,  0U}, /* 268 SPI1_SPEI */ \
	{IPR19_h,  0U}, /* 269 SPI1_SPRI */ \
	{IPR19_h,  0U}, /* 270 SPI1_SPTI */ \
\
	{IPR20_h, 12U}, /* 271 CAN0_ERS */ \
	{IPR20_h, 12U}, /* 272 CAN0_OVR */ \
	{IPR20_h, 12U}, /* 273 CAN0_RM0 */ \
	{IPR20_h, 12U}, /* 274 CAN0_RM1 */ \
	{IPR20_h, 12U}, /* 275 CAN0_SLE */ \
	{IPR20_h,  8U}, /* 276 CAN1_ERS */ \
	{IPR20_h,  8U}, /* 277 CAN1_OVR */ \
	{IPR20_h,  8U}, /* 278 CAN1_RM0 */ \
	{IPR20_h,  8U}, /* 279 CAN1_RM1 */ \
	{IPR20_h,  8U}, /* 280 CAN1_SLE */ \
\
	{IPR20_h,  4U}, /* 281 IEB */ \
\
	{IPR20_h,  0U}, /* 282 CDD_ISY */ \
	{IPR20_h,  0U}, /* 283 CDD_IERR */ \
	{IPR20_h,  0U}, /* 284 CDD_ITARG */ \
	{IPR20_h,  0U}, /* 285 CDD_ISEC */ \
	{IPR20_h,  0U}, /* 286 CDD_IBUF */ \
	{IPR20_h,  0U}, /* 287 CDD_IREADY */ \
\
	{IPR21_h, 12U}, /* 288 FLMC_STEI */ \
	{IPR21_h, 12U}, /* 289 FLMC_TENDI */ \
	{IPR21_h, 12U}, /* 290 FLMC_TREQ0I */ \
	{IPR21_h, 12U}, /* 291 FLMC_TREQ1I */ \
\
	{IPR21_h,  8U}, /* 292 SDHIF_SDHI3 */ \
	{IPR21_h,  8U}, /* 293 SDHIF_SDHI0 */ \
	{IPR21_h,  8U}, /* 294 SDHIF_SDHI1 */ \
\
	{0U, 0U},	  /* 295 */ \
\
	{IPR21_h,  4U}, /* 296 RTC_ARM */ \
	{IPR21_h,  4U}, /* 297 RTC_PRD */ \
	{IPR21_h,  4U}, /* 298 RTC_ACUP */ \
\
	{IPR22_h, 12U}, /* 299 SRC0_OVF */ \
	{IPR22_h, 12U}, /* 300 SRC0_UDF */ \
	{IPR22_h, 12U}, /* 301 SRC0_CEF */ \
	{IPR22_h, 12U}, /* 302 SRC0_ODFI */ \
	{IPR22_h, 12U}, /* 303 SRC0_IDEI */ \
	{IPR22_h,  8U}, /* 304 SRC1_OVF */ \
	{IPR22_h,  8U}, /* 305 SRC1_UDF */ \
	{IPR22_h,  8U}, /* 306 SRC1_CEF */ \
	{IPR22_h,  8U}, /* 307 SRC1_ODFI */ \
	{IPR22_h,  8U}, /* 308 SRC1_IDEI */ \
\
	{0U, 0U},	  /* 309 */ \
\
	{IPR22_h,  0U}, /* 310 DCU_DCUEI */ \
	{IPR22_h,  0U}, /* 311 DCU_OFFI */ \
	{IPR22_h,  0U}, /* 312 DCU_IFEI */


#ifndef TOPPERS_MACRO_ONLY
/*
 * 割込み優先レベル設定レジスタの初期化
 * 　ペリファラルの数によって、割込み優先レベル設定レジスタの構成が
 * 　異なるため、チップ依存部で定義する。
 */
Inline void
init_ipr(void)
{
	sil_wrh_mem((uint16_t *)IPR01_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR02_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR05_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR06_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR07_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR08_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR09_h, 0x0000U);
//	sil_wrh_mem((uint16_t *)IPR10_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR11_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR12_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR13_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR14_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR15_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR16_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR17_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR18_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR19_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR20_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR21_h, 0x0000U);
	sil_wrh_mem((uint16_t *)IPR22_h, 0x0000U);
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *	プロセッサ依存モジュール（SH2A用）
 */
#include "sh12a_gcc/sh2a_config.h"

#endif /* TOPPERS_SH7262_CONFIG_H */
