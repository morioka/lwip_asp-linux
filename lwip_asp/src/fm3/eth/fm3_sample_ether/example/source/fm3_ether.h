/*
 * fm3_ether.h
 *
 *  Created on: 2012/01/01
 *      Author: Kentaro Sekimoto
 */
#ifndef FM3_ETHER_H_
#define FM3_ETHER_H_

#include "common.h"
#include "mcu.h"
#include <stdint.h>

#define FM3_ETHER_PRIORITY 10

#define ETH_CLKG_MACEN	0	// 2bits
#define ETH_MODE_IFMODE 0
#define ETH_MODE_RST0	8
#define	ETH_MODE_RST1	9
#define ETH_MODE_PPSSWL	28

// GMAC Register 0 (MCR) - MCR (MAC Configuration Register) Address 0000h
// MCR レジスタは、送受信のオペレーティングモードを設定します。
#define GMAC_MCR_B_RE	2		// E (Receiver Enable)
#define GMAC_MCR_B_TE	3		// TE (Transmitter Enable)
#define GMAC_MCR_B_DC	4		// DC (Deferral Check)
#define GMAC_MCR_B_BL	5		// BL (Back-off Limit)
#define GMAC_MCR_B_ACS	7		// ACS (Automatic Pad/CRC Stripping)
#define GMAC_MCR_B_LUD	8		// LUD(Link Up/Down in RGMII)
#define GMAC_MCR_B_DR	9		// DR (Disable Retry)
#define GMAC_MCR_B_IPC	10		// IPC (Checksum Offload)
#define GMAC_MCR_B_DM	11		// DM (Duplex mode)
#define GMAC_MCR_B_LM	12		// LM (Loop-back Mode)
#define GMAC_MCR_B_DO	13		// DO (Disable Receive OWN)
#define GMAC_MCR_B_FES	14		// FES(Speed)
#define GMAC_MCR_B_PS	15		// PS (Port Select)
#define GMAC_MCR_B_DCRS	16		// DCRS(Disable Carrier Sense During Transaction)
#define GMAC_MCR_B_IFG	17		// IFG (Inter-Frame GAP)
#define GMAC_MCR_B_JE	20		// JE (Jumbo Frame Enable)
#define GMAC_MCR_B_BE	21		// BE (Frame Burst Enable)
#define GMAC_MCR_B_JD	22		// JD (Jabber Disable)
#define GMAC_MCR_B_WD	23		// WD (Watchdog Disable)
#define GMAC_MCR_B_TC	24		// TC (Transmit Configuration in RGMII)
#define GMAC_MCR_B_CST	25		// CST (CRC stripping for Type frames)
#define	GMAC_MCR_RE		(1 << GMAC_MCR_B_RE)
#define	GMAC_MCR_TE		(1 << GMAC_MCR_B_TE)
#define	GMAC_MCR_DC		(1 << GMAC_MCR_B_DC)
#define	GMAC_MCR_BL		(1 << GMAC_MCR_B_BL)
#define	GMAC_MCR_ACS	(1 << GMAC_MCR_B_ACS)
#define	GMAC_MCR_LUD	(1 << GMAC_MCR_B_LUD)
#define	GMAC_MCR_DR		(1 << GMAC_MCR_B_DR)
#define	GMAC_MCR_IPC	(1 << GMAC_MCR_B_IPS)
#define	GMAC_MCR_DM		(1 << GMAC_MCR_B_DM)
#define	GMAC_MCR_LM		(1 << GMAC_MCR_B_LM)
#define	GMAC_MCR_DO		(1 << GMAC_MCR_B_DO)
#define	GMAC_MCR_FES	(1 << GMAC_MCR_B_FES)
#define	GMAC_MCR_PS		(1 << GMAC_MCR_B_PS)
#define	GMAC_MCR_DCRS	(1 << GMAC_MCR_B_DCRS)
#define	GMAC_MCR_IFG	(1 << GMAC_MCR_B_IFG)
#define	GMAC_MCR_JE		(1 << GMAC_MCR_B_JE)
#define	GMAC_MCR_BE		(1 << GMAC_MCR_B_BE)
#define	GMAC_MCR_JD		(1 << GMAC_MCR_B_JD)
#define	GMAC_MCR_WD		(1 << GMAC_MCR_B_WD)
#define	GMAC_MCR_TC		(1 << GMAC_MCR_B_TC)
#define	GMAC_MCR_CST	(1 << GMAC_MCR_B_CST)

// GMAC Register 1 (MFFR) - MFFR (MAC Frame Filter Register) Address 00004h
// MAC Frame Filter レジスタは、受信フレームのフィルタリング制御を行います。
#define GMAC_MFFR_B_PR		0	// PR (Promiscuous Mode)
#define GMAC_MFFR_B_HUC		1	// HUC (Hash Unicast)
#define GMAC_MFFR_B_HMC		2	// HMC (Hash Multicast)
#define GMAC_MFFR_B_DAIF	3	// DAIF (DA Inverse Filtering)
#define GMAC_MFFR_B_PM		4	// PM (Pass All Multicast)
#define GMAC_MFFR_B_DB		5	// DB (Disable Broadcast Frames)
#define GMAC_MFFR_B_PCF		6	// PCF (Pass Control Frames)
#define GMAC_MFFR_B_SAIF	8	// SAIF (Source Address Inverse Filter)
#define GMAC_MFFR_B_SAF		9	// SAF (Source Address Filter)
#define GMAC_MFFR_B_HPF		10	// HPF (Hash or Perfect Filter)
#define GMAC_MFFR_B_RA		31	// RA (Receive All)
#define GMAC_MFFR_PR	(1 << GMAC_MFFR_B_PR)	// PR (Promiscuous Mode)
#define GMAC_MFFR_HUC	(1 << GMAC_MFFR_B_HUC)	// HUC (Hash Unicast)
#define GMAC_MFFR_HMC	(1 << GMAC_MFFR_B_HMC)	// HMC (Hash Multicast)
#define GMAC_MFFR_DAIF	(1 << GMAC_MFFR_B_DAIF)	// DAIF (DA Inverse Filtering)
#define GMAC_MFFR_PM	(1 << GMAC_MFFR_B_PM)	// PM (Pass All Multicast)
#define GMAC_MFFR_DB	(1 << GMAC_MFFR_B_DB)	// DB (Disable Broadcast Frames)
#define GMAC_MFFR_PCF	(1 << GMAC_MFFR_B_PCF)	// PCF (Pass Control Frames)
#define GMAC_MFFR_SAIF	(1 << GMAC_MFFR_B_SAIF)	// SAIF (Source Address Inverse Filter)
#define GMAC_MFFR_SAF	(1 << GMAC_MFFR_B_SAF)	// SAF (Source Address Filter)
#define GMAC_MFFR_HPF	(1 << GMAC_MFFR_B_HPF)	// HPF (Hash or Perfect Filter)
#define GMAC_MFFR_RA	(1 << GMAC_MFFR_B_RA)	// RA (Receive All)


// GMAC Register 2, 3 (MHTRH, MHTRL)
// MHTRH (MAC Hash Table Register (High)) Address 0008h
// MHTRL (MAC Hash Table Register (Low))
// MHTRH およびMHTRL レジスタは、グループアドレス・フィルタリングを設定します。

// GMAC Register 4 (GAR) - GAR (GMII/MII Address Register) Address 0010h
// GMII Address レジスタは、マネジメントインタフェースを通じて外部PHY へのマネジメン
// トサイクルをコントロールします。
#define GMAC_GAR_B_GB	0
#define GMAC_GAR_B_GW	1
#define GMAC_GAR_B_CR	2
#define GMAC_GAR_B_GR	6
#define GMAC_GAR_B_PA	11
#define	GMAC_GAR_GB	(1 << GMAC_GAR_B_GB)
#define	GMAC_GAR_GW	(1 << GMAC_GAR_B_GW)
#define	GMAC_GAR_CR	(1 << GMAC_GAR_B_CR)
#define	GMAC_GAR_GR	(1 << GMAC_GAR_B_GR)
#define	GMAC_GAR_PA	(1 << GMAC_GAR_B_PA)

// GMAC Register 5 (GDR) - GDR (GMII/MII Data Register) Address 0014h
// GMII Data レジスタは、PHY レジスタにライトデータを設定します。また、PHY レジスタか
// らのリードデータが格納されます。

// GMAC Register 6 (FCR) - FCR (Flow Control Register) Address 0018h
// Flow Control レジスタは、GMAC のフローコントロールモジュールによるControl (Pause
// Command)フレームの生成および受信をコントロールします。
#define GMAC_FCR_B_FCB	0	// FCB/BPA (Flow Control Busy/Backpressure Activate)
#define GMAC_FCR_B_TFE	1	// TFE (Transmit Flow Control Enable)
#define GMAC_FCR_B_RFE	2	// RFE (Receive Flow Control Enable)
#define GMAC_FCR_B_UP	3	// UP (Unicast Pause Frame detect)
#define GMAC_FCR_B_PLT	4	// PLT (Pause Low Threshold)
#define GMAC_FCR_B_DZPQ	7	// DZPQ (Disable Zero-Quanta Pause) - Reserved
#define GMAC_FCR_B_PT	16	// PT (Pause Time)

// GMAC Register 7 (VTR)
// VLAN Tag レジスタは、VLAN フレームを識別する値を設定します。

// GMAC Register 10 (RWFFR)
// このレジスタは、Wake-up フレームのFilter パターンを設定します。

// GMAC Register 11 (PMTR)
// このレジスタは、Wake-up リクエスト・イベントをプログラムし、Wake-up イベントをモニタします

// GMAC Register 12 (LPICSR)
// このレジスタは、LPI の機能をコントロールし、LPI 割込みステータスを提供します

// GMAC Register 13 (LPITCR) - LPITCR (LPI Timers Control Register) Address 0034h
// このレジスタは、LPI ステートのタイムアウト値を設定します。GMAC がLPI パターンを送
// 信する時間、および通常の送信を再開するまで待機する時間を指定します。

// GMAC Register 14 (ISR) - ISR (Interrupt Status Register) Address 0038h
// このレジスタは、割込みステータスを表示します。

// GMAC Register 15 (IMR)
// Interrupt Mask Register ビットによって、Interrupt Status Register の対応するイベントによる
// 割込み信号(INT_SBD)をマスクできます。

// GMAC Register 16 (MAR0H)
// MAC Address0 High レジスタは、ステーションの最初のMAC アドレス(6 バイト)の上位16
// ビットを設定します。

// GMAC Register 17 (MAR0L)
// MAC Address0 Low レジスタは、ステーションの最初のMAC アドレス(6 バイト)の下位32
// ビットを設定します。

// GMAC Register 18, 20, 22, …, 542 (MAR1H, 2H, 3H, …,31H)
// MAC Address High レジスタ(1-31)は、ステーションの2〜32 番目のMAC アドレス(6 バイト)
// の上位16 ビットを設定します。

// GMAC Register 19, 21, 23, …, 543 (MAR1L, 2L, 3L, …,31L)
// MAC Address Low レジスタ(1-31)は、ステーションの2〜32 番目のMAC アドレス(6 バイト)
// の下位32 ビットを設定します。

// GMAC Register 54 (RGSR) - RGSR (RGMII Status Register) Address 00D8h
// RGMII ステータスレジスタは、RGMII がPHY から受信したステータス信号を示します。

// GMAC Register 448 (TSCR) - TSCR (Time Stamp Control Register) Address 0700h
// このレジスタは、レシーバのタイムスタンプ生成のためのシステムタイムジェネレータの動
// 作とPTP パケットのスヌーピングをコントロールします。

// GMAC Register 449 (SSIR) - SSIR (Sub-Second Increment Register) Address 0704h
// Coarse Update モード(Time Stamp Control Register のTFCU ビット)では、このレジスタ内
// の値はPTP_CLK のクロックサイクルごとにシステムタイムに加算されます。Fine Update
// モードでは、このレジスタ内の値はアキュムレータがオーバフローになったときにシステム
// タイムに加算されます。

// GMAC Register 450 (STSR) - STSR (System Time - Seconds Register) Address 0708h
// System Time - Seconds レジスタは、System Time - Nanoseconds レジスタと合わせて、現
// 在保持されているシステムタイムの値を示します。この値は継続的に更新されますが、クロ
// ックドメイン転送遅延(PTP_CLK からSYS_CLK)のために実際の時間からは少し遅延します。

// GMAC Register 451 (STNR) - STNR (System Time - Nanoseconds Register) Address 070Ch
// System Time - Nanoseconds レジスタは、System Time - Seconds レジスタと合わせて使用
// します。

// GMAC Register 452 (STSUR) - STSUR (System Time - Seconds Update Register) Address 0710h
// System Time - Seconds Update レジスタは、System Time - Nanoseconds Update レジスタ
// と合わせて、保持されているシステムタイムを初期化または更新します。Time Stamp Control
// レジスタでTSINIT またはTSUPDT ビットをセットする前に、この両方のレジスタに書き込
// む必要があります。

// GMAC Register 453 (STNUR) - STSNUR (System Time - Nanoseconds Update Register) Address 0714h
// System Time - Nanoseconds Update レジスタは、System Time - Seconds Update レジスタ
// と合わせて使用します。

// GMAC Register 454 (TSAR)- TSAR (Time Stamp Addend Register) Address 0718h
// このレジスタ値は、システムタイムがFine Update モードに設定されている(GMAC Register
// 448 のTFCU ビット)ときにのみ使用されます。PTP_CLK のクロックサイクルごとにこのレ
// // ジスタの内容が32 ビットのアキュムレータに加算され、アキュムレータがオーバフローにな
// ったときにシステムタイムが更新されます。

// GMAC Register 455 (TTSR) - TTSR (Target Time Seconds Register) Address 071Ch
// Target Time Seconds register は、Target Time Nanoseconds レジスタと合わせて使用され、
// システムタイムがこれらのレジスタでプログラムされている値を超えたときに割込みイベン
// ト(レジスタ458 のTSTART ビット, またはレジスタ14[9]のTS 割込みビット)をスケジュー
// ルします。

// GMAC Register 456 (TTNR) - TTNR (Target Time Nanoseconds Register) Address 0720h
// Target Time Nanoseconds レジスタは、Target Time Seconds register と合わせて使用します。

// GMAC Register 457 (STHWSR) - STHWSR (System Time - Higher Word Seconds Register) Address 0724h
// このレジスタは、タイムスタンプの48 ビット秒値の上位16 ビットを読み出します。

// GMAC Register 458 (TSR) - TSR (Time Stamp Status Register) Address 0728h
// このレジスタは、システムタイムカウンタへの操作のステータスを表示します。

// GMAC Register 459 (PPSCR) - PPSCR (PPS Control Register) Address 072Ch
// このレジスタは、PTPPPS 出力端子の制御を行います。

// GMAC Register 460 (ATNR) - ATNR (Auxiliary Time Stamp - Nanoseconds Register) Address 0730h
// このレジスタは、Reserved レジスタです。

// GMAC Register 461 (ATSR) - ATSR (Auxiliary Time Stamp - Seconds Register) Address 0734h
// このレジスタは、Reserved レジスタです

// DMA Register 0 (BMR)
// このレジスタは、DMA の動作モードを設定します。
#define DMA_BMR_B_SWR	0
#define DMA_BMR_B_DA	1
#define DMA_BMR_B_DSL	2
#define DMA_BMR_B_ATDS	7
#define DMA_BMR_B_PBL	8
#define DMA_BMR_B_PR	14
#define DMA_BMR_B_FB	16
#define DMA_BMR_B_RPBL	17
#define DMA_BMR_B_USP	23
#define DMA_BMR_B_8xPBL	24
#define DMA_BMR_B_AAL	25
#define DMA_BMR_B_MB	26
#define DMA_BMR_B_TXPR	27
#define	DMA_BMR_SWR	(1 << DMA_BMR_B_SWR)
#define	DMA_BMR_DA	(1 << DMA_BMR_B_DA)
#define	DMA_BMR_FB	(1 << DMA_BMR_B_FB)
#define	DMA_BMR_AAL	(1 << DMA_BMR_B_AAL)
#define	DMA_BMR_MB	(1 << DMA_BMR_B_MB)

// DMA Register 1 (TPDR)
// Transmit Poll Demand レジスタは、送信DMA に対して、現在のディスクリプタがDMA によ
// って所有されているかどうかをチェックすることを指示します。

// DMA Register 2 (RPDR)
// Receive Poll Demand レジスタは、受信DMA が新しいディスクリプタをチェックすることを
// 指示します。

// DMA Register 3 (RDLAR)
// Receive Descriptor List Address レジスタは、Receive Descriptor List の先頭を指定します。

// DMA Register 4 (TDLAR)
// Transmit Descriptor List Address レジスタは、Transmit Descriptor List の先頭を指定します。

// DMA Register 5 (SR)
// ステータスレジスタは、DMA がホストにレポートするすべてのステータスビットを含みます。
#define DMA_SR_B_TI		0		// Transmit Interrupt
#define DMA_SR_B_TPS	1		// Transmit Process Stopped
#define DMA_SR_B_TU		2		// Transmit Buffer Unavailable
#define DMA_SR_B_TJT	3		// Transmit Jabber Timeout
#define DMA_SR_B_OVF	4		// Receive overflow
#define DMA_SR_B_UNF	5		// Transmit underflow
#define DMA_SR_B_RI		6		// Receive Interrupt
#define DMA_SR_B_RU		7		// Receive Buffer Unavailable
#define DMA_SR_B_RPS	8		// Receive Process Stopped
#define DMA_SR_B_RWT	9		// RWT Receive Watchdog Timeout
#define DMA_SR_B_ETI	10		// Early Transmit Interrupt
#define DMA_SR_B_FBI	13		// Fatal Bus Error Interrupt
#define DMA_SR_B_ERI	14		// Early Receive Interrupt
#define DMA_SR_B_AIS	15		// Abnormal Interrupt Summary
#define DMA_SR_B_NIS	16		// Normal Interrupt Summary
#define DMA_SR_B_RS		17		// RS Receive Process State
#define DMA_SR_B_TS		20		// Transmit Process State
#define DMA_SR_B_EB		23		// Error Bits
#define DMA_SR_B_GLI	26		// GMAC Line interface Interrupt (Reserved)
#define DMA_SR_B_GMI	27		// GMAC MMC Interrupt
#define DMA_SR_B_GPI	28		// GMAC PMT Interrupt
#define DMA_SR_B_TTI	29		// Time-Stamp Trigger Interrupt
#define DMA_SR_B_GLPII	30		// GMAC LPI Interrupt
#define DMA_SR_TPS		(1 << DMA_SR_B_PS)
#define DMA_SR_TI		(1 << DMA_SR_B_TI)
#define DMA_SR_TU		(1 << DMA_SR_B_TU)
#define DMA_SR_TJT		(1 << DMA_SR_B_TJT)
#define DMA_SR_OVF		(1 << DMA_SR_B_OVF)
#define DMA_SR_UNF		(1 << DMA_SR_B_UNF)
#define DMA_SR_RI		(1 << DMA_SR_B_RI)
#define DMA_SR_RU		(1 << DMA_SR_B_RU)
#define DMA_SR_RPS		(1 << DMA_SR_B_RPS)
#define DMA_SR_RWT		(1 << DMA_SR_B_RWT)
#define DMA_SR_ETI		(1 << DMA_SR_B_ETI)
#define DMA_SR_FBI		(1 << DMA_SR_B_FBI)
#define DMA_SR_ERI		(1 << DMA_SR_B_ERI)
#define DMA_SR_AIS		(1 << DMA_SR_B_AIS)
#define DMA_SR_NIS		(1 << DMA_SR_B_NIS)
#define DMA_SR_RS		(1 << DMA_SR_B_RS)
#define DMA_SR_TS		(1 << DMA_SR_B_TS)
#define DMA_SR_EB		(1 << DMA_SR_B_EB)
#define DMA_SR_GLI		(1 << DMA_SR_B_GLI)
#define DMA_SR_GMI		(1 << DMA_SR_B_GMI)
#define DMA_SR_GPI		(1 << DMA_SR_B_CPI)
#define DMA_SR_TTI		(1 << DMA_SR_B_TTI)
#define DMA_SR_GLPII	(1 << DMA_SR_B_GLPII)

// DMA Register 6 (OMR) Operation Mode Register
// Operation Mode レジスタは、送受信オペレーティングモードおよびコマンドを指定します。
// このレジスタは、DMA 初期化時の最後に書き込みます。
#define DMA_OMR_B_SR	1		// Start/Stop Recieve
#define DMA_OMR_B_OSF	2		// Operate on Second Frame
#define DMA_OMR_B_RTC	4		// Receive Threshold Control
#define DMA_OMR_B_FUF	6		// Foward Undersized Good Frames
#define DMA_OMR_B_FEF	7		// Forward Error Frames
#define DMA_OMR_B_ST	13		// Start/Stop Transmission Command
#define DMA_OMR_B_TTC	14		// Transmit Threshold Control
#define DMA_OMR_B_FTF	20		// Flush Tramsmit FIFO
#define DMA_OMR_B_TSF	21		// Transmit Store Forward
#define DMA_OMR_B_DEF	24		// Disable Flushing of Received Frames
#define DMA_OMR_B_RSF	25		// Receive Store and Forward
#define DMA_OMR_B_DT	26		// Disable Dropping of TCP/IP Chechsum Error Frames
#define DMA_OMR_SR		(1 << DMA_OMR_B_SR)		// Start/Stop Recieve
#define DMA_OMR_OSF		(1 << DMA_OMR_B_OSF)	// Operate on Second Frame
#define DMA_OMR_RTC		(1 << DMA_OMR_B_RTC)	// Receive Threshold Control
#define DMA_OMR_FUF		(1 << DMA_OMR_B_FUF)	// Foward Undersized Good Frames
#define DMA_OMR_FEF		(1 << DMA_OMR_B_FEF)	// Forward Error Frames
#define DMA_OMR_ST		(1 << DMA_OMR_B_ST)		// Start/Stop Transmission Command
#define DMA_OMR_TTC		(1 << DMA_OMR_B_TTC)	// Transmit Threshold Control
#define DMA_OMR_FTF		(1 << DMA_OMR_B_FTF)	// Flush Tramsmit FIFO
#define DMA_OMR_TSF		(1 << DMA_OMR_B_TSF)	// Transmit Store Forward
#define DMA_OMR_DEF		(1 << DMA_OMR_B_DEF)	// Disable Flushing of Received Frames
#define DMA_OMR_RSF		(1 << DMA_OMR_B_RSF)	// Receive Store and Forward
#define DMA_OMR_DT		(1 << DMA_OMR_B_DT)		// Disable Dropping of TCP/IP Chechsum Error Frames


// DMA Register 7 (IER) Interrupt Enable Register
// このレジスタは、DMA からの割込みを許可します。
#define DMA_IER_B_TIE	0		// Transmit Process Stopped
#define DMA_IER_B_TSE	1		// Transmit Process Stopped
#define DMA_IER_B_TUE	2		// Transmit Buffer Unavailable
#define DMA_IER_B_TJE	3		// Transmit Jabber Timeout
#define DMA_IER_B_OVE	4		// Receive Overflow Enable
#define DMA_IER_B_UNE	5		// Transmit Underflow Enable
#define DMA_IER_B_RIE	6		// Receive Interrupt Enable
#define DMA_IER_B_RUE	7		// Receive Buffer Unavailable Enable
#define DMA_IER_B_RSE	8		// Receive Process Stopped Enable
#define DMA_IER_B_RWE	9		// Receive Watchdog Timeout Enable
#define DMA_IER_B_ETE	10		// Early Transmit Interrupt Enable
#define DMA_IER_B_FBE	13		// Fatal Bus Error Enable
#define DMA_IER_B_ERE	14		// Early Receive Interrupt Enable
#define DMA_IER_B_AIE	15		// Abnormal Interrupt Summary Enable
#define DMA_IER_B_NIE	16		// Normal Interrupt Summary Enable
#define	DMA_IER_TIE		(1 << DMA_IER_B_TIE)
#define	DMA_IER_TUE		(1 << DMA_IER_B_TUE)
#define	DMA_IER_RIE		(1 << DMA_IER_B_RIE)
#define	DMA_IER_RUE		(1 << DMA_IER_B_RUE)
#define DMA_IER_ERE		(1 << DMA_IER_B_ERE)
#define DMA_IER_NIE		(1 << DMA_IER_B_NIE)

// DMA Register 8 (MFBOCR)
// DMA には、受信中に失われたフレームの数を追跡する2 つのカウンタがあります。このレジ
// スタはカウンタの現在の値を報告します。このカウンタは診断のために使用されます。

// DMA Register 9 (RIWTR)
// このレジスタに0 以外の値が書き込まれたとき、RI (DMA Register 5[6])のWatchdog Timer
// がイネーブルされます

// DMA Register 11 (AHBSR)
// このレジスタは、AHB マスタ・インタフェースの状態を表示します。
#define DMA_AHBSR_B_AHBS	0
#define	DMA_AHBSR_AHBS	(1 << DMA_AHBSR_B_AHBS)

// DMA Register 18 (CHTDR)
// Current Host Transmit Descriptor レジスタは、DMA によって読み込まれた現在の送信ディス
// クリプタの開始アドレスを示します。

// DMA Register 19 (CHRDR)
// Current Host Receive Descriptor レジスタは、DMA によって読み込まれた現在の受信ディス
// クリプタの開始アドレスを示します。

// DMA Register 20 (CHTBAR)
// Current Host Transmit Buffer Address レジスタは、DMA によって読み込まれる現在の送信バ
// ッファアドレスを示します。

// DMA Register 21 (CHRBAR)
// Host Receive Buffer Address レジスタは、DMA によって読み込まれる現在の受信バッファア
// ドレスを示します。

// MMC コントロールレジスタ(mmc_cntl) Address:0100h
// MMC コントロールレジスタは、マネジメントカウンタのオペレーションモードを設定します。

// MMC 受信割込みレジスタ(mmc_intr_rx) Address:0104h
// このレジスタは、各受信統計カウンタからの割込みを表示します。

// MMC 送信割込みレジスタ(mmc_intr_tx) Address:0108h
// このレジスタは、各送信統計カウンタの割込みを表示します。

// MMC 受信割込みマスク割込みレジスタ(mmc_intr_mask_rx) Address:010Ch
// このレジスタは、各受信統計カウンタからの割込みをマスクします。

// MMC 送信割込みマスクレジスタ(mmc_intr_mask_tx)Address:0110h
// このレジスタは、各送信統計カウンタからの割込みをマスクします。

// MMC 受信チェックサムオフロード割込みマスクレジスタ(mmc_ipc_intr_mask_rx) Address:0200h
// このレジスタは、各受信IPC(チェックサムオフロード)統計カウンタからの割込みをマスクします。

// MMC 受信チェックサムオフロード割込みレジスタ(mmc_ipc_intr_rx) Address:0208h
// このレジスタは、各受信IPC(チェックサムオフロード)統計カウンタからの割込みを表示します。

// Transmit Enhanced Descriptor 0 (TDES0)
// TDES0 は、送信フレームの制御ビット, ステータスビットにより構成されます。
#define TDES0_B_DB		0		// DB (Deferred Bit)
#define TDES0_B_UF		1		// CC (Collision Count)
#define TDES0_B_ED		2		// ED (Excessive Deferral)
#define TDES0_B_CC		3		// ED (Excessive Deferral)
#define TDES0_B_VF		7		// VF (VLAN Frame)
#define TDES0_B_EC		8		// EC (Excessive Collision)
#define TDES0_B_LCO		9		// LCO (Late Collision)
#define TDES0_B_NC		10		// NC (No Carrier)
#define TDES0_B_LC		11		// LC (Loss of Carrier)
#define TDES0_B_IPE		12		// IPE (IP Payload Error)
#define TDES0_B_FF		13		// FF(Frame Flushed)
#define TDES0_B_JT		14		// JT (Jabber Timeout)
#define TDES0_B_ES		15		// ES (Error Summary)
#define TDES0_B_IHE		16		// IHE (IP Header Error)
#define TDES0_B_TTSS	17		// TTSS (Transmit Time Stamp Status)
#define TDES0_B_TCH		20		// TCH (Second Address Chained)
#define TDES0_B_TER		21		// TER (Transmit End of Ring)
#define TDES0_B_CIC		22		// CIC (Checksum lnsertion Control)
#define TDES0_B_TTSE	25		// TTSE (Transmit Time Stamp Enable)
#define TDES0_B_DP		26		// DP (Disable Pad)
#define TDES0_B_DC		27		// DC (Disable CRC)
#define TDES0_B_FS		28		// FS (First Segment)
#define TDES0_B_LS		29		// LS (Last Segment)
#define TDES0_B_IC		30		// IC (Interrupt on Completion)
#define TDES0_B_OWN		31		// OWN (OWN bit)

// Transmit Enhanced Descriptor 1 (TDES1)
// TDES1 は、送信バッファ1, 2 のサイズを指定します。

// Transmit Enhanced Descriptor 2 (TDES2)
// TDES2 は、送信バッファ1 の物理アドレスを指定します。

// Transmit Enhanced Descriptor 3 (TDES3)
// TDES3 は、送信バッファ2 の物理アドレスもしくは次のディスクリプタの物理アドレスを指定します。

// Transmit Enhanced Descriptor 6 (TDES6)
// TDES6 には、送信時にキャプチャされたタイムスタンプ下位32 ビットがDMA により格納\されます。

// Transmit Enhanced Descriptor 7 (TDES7)
// TDES7 には、送信時にキャプチャされたタイムスタンプ上位32 ビットがDMA により格納されます。

// Receive Enhanced Descriptor 0 (RDES0)
// RDES0 は、受信フレームの制御ビット, ステータスビットにより構成されます。
#define RDES0_B_ESA		0		// ESA (Extended Status Available)
#define RDES0_B_CE		1		// CE (CRC Error)
#define RDES0_B_DBE		2		// DE (Dribble Bit Error)
#define RDES0_B_RE		3		// RE (Receive Error)
#define RDES0_B_RWT		4		// RWT (Receive Watchdog Timeout)
#define RDES0_B_FT		5		// FT (Frame Type)
#define RDES0_B_LC		6		// LC (Late Collision)
#define RDES0_B_TS		7		// TS (Time Stamp)
#define RDES0_B_LS		8		// LS (Last Descriptor)
#define RDES0_B_FS		9		// FS (First Descriptor)
#define RDES0_B_VLAN	10		// VLAN (VLAN tag)
#define RDES0_B_OE		11		// OE (Overflow Error)
#define RDES0_B_LE		12		// LE (Length Error)
#define RDES0_B_SAF		13		// SAF(Source Address Filter Fail)
#define RDES0_B_DE		14		// DE (Descriptor Error)
#define RDES0_B_ES		15		// ES (Error Summary)
#define RDES0_B_FL		16		// FL[13:0] (Frame Length)
#define RDES0_B_AFM		30		// AFM(Destination Address Filter Fail)
#define RDES0_B_OWN		31		// OWN (OWN bit)

// Receive Enhanced Descriptor 1 (RDES1)
// RDES2 は、受信バッファ1, 2 のサイズと制御情報を指定します。

// Receive Enhanced Descriptor 2 (RDES2)
// RDES2 は、受信バッファ1 の物理アドレスを指定します。

// Receive Enhanced Descriptor 3 (RDES3)
// RDES3 は、受信バッファ2 の物理アドレスもしくは次のディスクリプタの物理アドレスを指定します。

// Receive Enhanced Descriptor 4 (RDES4)
// RDES4 には、受信フレームの拡張ステータス情報が格納されます。

// Receive Enhanced Descriptor 6 (RDES6)
// RDES6 には、受信時にキャプチャされたタイムスタンプ下位32 ビットがDMA により格納されます。

// Receive Enhanced Descriptor 7 (RDES7)
// RDES7 には、受信時にキャプチャされたタイムスタンプ上位32 ビットがDMA により格納されます。


typedef struct stc_emac_dma_tdes0_field
{
	unsigned char DB		: 1;
	unsigned char UF		: 1;
	unsigned char ED		: 1;
	unsigned char CC		: 4;
	unsigned char VF		: 1;
	unsigned char EC		: 1;
	unsigned char LCO		: 1;
	unsigned char NC		: 1;
	unsigned char LC		: 1;
	unsigned char IPE		: 1;
	unsigned char FF		: 1;
	unsigned char JT		: 1;
	unsigned char ES		: 1;
	unsigned char IHE		: 1;
	unsigned char TTSS	: 1;
	unsigned char Reserved1: 2;
	unsigned char TCH		: 1;
	unsigned char TER		: 1;
	unsigned char CIC		: 2;
	unsigned char Reserved2: 1;
	unsigned char TTSE	: 1;
	unsigned char DP		: 1;
	unsigned char DC		: 1;
	unsigned char FS		: 1;
	unsigned char LS		: 1;
	unsigned char IC		: 1;
	unsigned char OWN		: 1;
} stc_emac_dma_tdes0_field_t;

typedef struct stc_emac_dma_tdes1_field
{
#if 0
	unsigned short TBS1		: 13;
	unsigned short Reserved1	: 3;
	unsigned short TBS2		: 13;
	unsigned short Reserved2	: 3;
#else
	unsigned short TBS1		: 16;
	unsigned short TBS2		: 16;
#endif
} stc_emac_dma_tdes1_field_t;

typedef struct stc_emac_dma_tdes2_field
{
	unsigned int B1AP		: 32;
} stc_emac_dma_tdes2_field_t;

typedef struct stc_emac_dma_tdes3_field
{
	unsigned int B2AP		: 32;
} stc_emac_dma_tdes3_field_t;

typedef struct _EMAC_DMA_TXDESC {
	union {
	    unsigned int TDES0;
	    stc_emac_dma_tdes0_field_t TDES0_f;
	  };
	union {
	    unsigned int TDES1;
	    stc_emac_dma_tdes1_field_t TDES1_f;
	  };
	union {
	    unsigned int TDES2;
	    stc_emac_dma_tdes2_field_t TDES2_f;
	  };
	union {
	    unsigned int TDES3;
	    stc_emac_dma_tdes3_field_t TDES3_f;
	  };
} EMAC_DMA_TXDESC;

typedef struct stc_emac_dma_rdes0_field
{
	unsigned char ESA		: 1;
	unsigned char CE		: 1;
	unsigned char DE		: 1;
	unsigned char RE		: 1;
	unsigned char RWT		: 1;
	unsigned char FT		: 1;
	unsigned char LC		: 1;
	unsigned char TS		: 1;
	unsigned char LS		: 1;
	unsigned char FS		: 1;
	unsigned char VLAN	: 1;
	unsigned char OE		: 1;
	unsigned char LE		: 1;
	unsigned char SAF		: 1;
	unsigned char DBE		: 1;
	unsigned char ES		: 1;
	unsigned short FL		: 14;
	unsigned short AFM	: 1;
	unsigned short OWN	: 1;
} stc_emac_dma_rdes0_field_t;

typedef struct stc_emac_dma_rdes1_field
{
	unsigned short RBS1		: 13;
	unsigned short Reserved1	: 1;
	unsigned short RCH		: 1;
	unsigned short RER		: 1;
	unsigned short RBS2		: 13;
	unsigned short Reserved2	: 2;
	unsigned short DIC		: 1;
} stc_emac_dma_rdes1_field_t;

typedef struct stc_emac_dma_rdes2_field
{
	unsigned int B1AP		: 32;
} stc_emac_dma_rdes2_field_t;

typedef struct stc_emac_dma_rdes3_field
{
	unsigned int B2AP		: 32;
} stc_emac_dma_rdes3_field_t;

typedef struct _EMAC_DMA_RXDESC {
	union {
	    unsigned int RDES0;
	    stc_emac_dma_rdes0_field_t RDES0_f;
	  };
	union {
	    unsigned int RDES1;
	    stc_emac_dma_rdes1_field_t RDES1_f;
	  };
	union {
	    unsigned int RDES2;
	    stc_emac_dma_rdes2_field_t RDES2_f;
	  };
	union {
	    unsigned int RDES3;
	    stc_emac_dma_rdes3_field_t RDES3_f;
	  };
} EMAC_DMA_RXDESC;

// ========== 関数プロトタイプ  開始 ==========
unsigned short ntohs(unsigned short n);
int PHY_Write(unsigned int phy_addr, unsigned int phy_register, unsigned int value, unsigned int timeout);
int PHY_Read(unsigned int phy_addr, unsigned int phy_register, unsigned int *value, unsigned int timeout);
EMAC_DMA_RXDESC *RXDESC_Received(void);
void Ether_Init(unsigned char *pmac);
void Packet_Send(unsigned char *p, unsigned int size);
void RxPacket_Dump(void);
void TxPacket_Dump(void);
void DMA_Register_Dump(void);
void PHY_Register_Dump(void);
void TXDESC_Dump(void);
void RXDESC_Dump(void);
// ========== 関数プロトタイプ  終了 ==========
#endif /* FM3_ETHER_H_ */
