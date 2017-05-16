/*
 * phy.h
 *
 *  Created on: 2012/03/04
 *      Author: Kentaro Sekimoto
 */

#ifndef PHY_H_
#define PHY_H_

// ========== PHY 開始 ==========
/* General PHY Registers */
#define PHY_REG_BMCR	0x00		// Basic Mode Control Register
#define PHY_REG_BMSR	0x01		// Basic Mode Status Register
#define PHY_REG_IDR1	0x02		// PHY Identifier 1
#define PHY_REG_IDR2	0x03		// PHY Identifier 2
#define PHY_REG_STS		0x10		// Status Register

/* Extended PHY Registers */
#define PHY_REG_19		19			// Extended Control Register 19
#define PHY_REG_20		20			// LED
#define PHY_REG_22		22			// Link
#define PHY_REG_23		23

#define BMCR_RESET		0x8000
#define BMSR_AUTO_DONE	0x0020

//#define PHY_AUTO_NEG	0x1000		// Select Auto Negotiation
#define PHY_AUTO_NEG	0x1200		// Select Auto Negotiation

#define STS_LINK_ON		0x1
#define STS_10_MBIT		0x2
#define STS_FULL_DUP	0x4

#define MII_WR_TOUT		0x00200000	// MII Write timeout count
#define MII_RD_TOUT		0x00200000	// MII Read timeout count

#define LAN8187_ADDR	0x06	// PHY device address for Wakamatsu NXP LAN board
#define ICS1894_ADDR	0x10	// PHY device address for Wakamatsu FM3 LAN board
#define LAN8700_ADDR	0x1f	// PHY device address for Will-elec WX-PHY-001 board
#define DP83848_ADDR	0x01	// PHY device address for DP83848

#define LAN8187_ID		0x20005C90	// PHY Identifier of LAN8187
#define ICS1894_ID		0x0015f450	// PHY Identifier of ICS1894

#define PHY_ADDR		LAN8700_ADDR

#define DEF_PHY_RESET_RETRY		3
#define DEF_PHY_AUTONEG_RETRY	1
#define DEF_PHY_RESET_STATUS_RETRY		10000
#define DEF_PHY_AUTONEG_STATUS_RETRY	100000
// ========== PHY 終了 ==========

#endif /* PHY_H_ */
