/*
 * fm3_sample_ether.h
 *
 *  Created on: 2012/02/26
 *      Author: Kentaro Sekimoto
 */

#ifndef FM3_SAMPLE_ETHER_H_
#define FM3_SAMPLE_ETHER_H_

#define ETH_CH0
//#define ETH_CH1

//#define ICS1894
//#define LAN8187
#define LAN8700

//#define DEBUG_DUMP
#if defined(ICS1894) || defined(LAN8187)
#define MY_MAC_ADDR0	{0x02, 0x00, 0x00, 0x00, 0x00, 0x00}
#define MY_MAC_ADDR1	{0x02, 0x00, 0x00, 0x00, 0x00, 0x01}
#endif

#endif /* FM3_SAMPLE_ETHER_H_ */
