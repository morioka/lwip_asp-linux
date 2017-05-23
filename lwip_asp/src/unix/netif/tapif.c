/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "netif/tapif.h"

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>


#include "lwip/debug.h"

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/ip.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"

#include "netif/etharp.h"

#if defined(LWIP_DEBUG) && defined(LWIP_TCPDUMP)
#include "netif/tcpdump.h"
#endif /* LWIP_DEBUG && LWIP_TCPDUMP */

#define IFCONFIG_BIN "/sbin/ifconfig "

#if defined(linux)
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#define DEVTAP "/dev/net/tun"
#define IFCONFIG_ARGS "tap0 inet %d.%d.%d.%d"
#elif defined(openbsd)
#define DEVTAP "/dev/tun0"
#define IFCONFIG_ARGS "tun0 inet %d.%d.%d.%d link0"
#else /* others */
#define DEVTAP "/dev/tap0"
#define IFCONFIG_ARGS "tap0 inet %d.%d.%d.%d"
#endif

#define IFNAME0 't'
#define IFNAME1 'p'

#ifndef TAPIF_DEBUG
#define TAPIF_DEBUG LWIP_DBG_OFF
#endif

struct tapif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
  int fd;
};

/* Forward declarations. */
static void  tapif_input(struct netif *netif);

static void tapif_thread(void *data);

#ifdef	LWIP_ASP_LINUX

#include <pthread.h>
#include <signal.h>

static pthread_t h_tapif_thread = 0;

#define	RXDESC_NUM 4

typedef struct {
    u16_t len;
    void* buf;
} EMAC_DMA_RXDESC;

static EMAC_DMA_RXDESC rxdesc_buf[RXDESC_NUM];
static char rx_buf[RXDESC_NUM][2048];


static u16_t rxdesc_rp = 0;
static u16_t rxdesc_wp = 0;

#include "kernel_cfg.h"

#endif

/*-----------------------------------------------------------------------------------*/
#ifdef	LWIP_ASP_LINUX
static void
eth_init_low(struct netif *netif)
#else
static void
low_level_init(struct netif *netif)
#endif
{
  struct tapif *tapif;
  char buf[sizeof(IFCONFIG_ARGS) + sizeof(IFCONFIG_BIN) + 50];
#ifdef	LWIP_ASP_LINUX
  int i;
#endif

  tapif = (struct tapif *)netif->state;

  /* Obtain MAC address from network interface. */

  /* (We just fake an address...) */
  tapif->ethaddr->addr[0] = 0x1;
  tapif->ethaddr->addr[1] = 0x2;
  tapif->ethaddr->addr[2] = 0x3;
  tapif->ethaddr->addr[3] = 0x4;
  tapif->ethaddr->addr[4] = 0x5;
  tapif->ethaddr->addr[5] = 0x6;

  /* Do whatever else is needed to initialize interface. */

  tapif->fd = open(DEVTAP, O_RDWR);
  LWIP_DEBUGF(TAPIF_DEBUG, ("tapif_init: fd %d\n", tapif->fd));
  if(tapif->fd == -1) {
#ifdef linux
    perror("tapif_init: try running \"modprobe tun\" or rebuilding your kernel with CONFIG_TUN; cannot open "DEVTAP);
#else
    perror("tapif_init: cannot open "DEVTAP);
#endif
    exit(1);
  }

#ifdef linux
  {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TAP|IFF_NO_PI;
    if (ioctl(tapif->fd, TUNSETIFF, (void *) &ifr) < 0) {
      perror("tapif_init: "DEVTAP" ioctl TUNSETIFF");
      exit(1);
    }
  }
#endif /* Linux */

  sprintf(buf, IFCONFIG_BIN IFCONFIG_ARGS,
           ip4_addr1(&(netif->gw)),
           ip4_addr2(&(netif->gw)),
           ip4_addr3(&(netif->gw)),
           ip4_addr4(&(netif->gw)));

  LWIP_DEBUGF(TAPIF_DEBUG, ("tapif_init: system(\"%s\");\n", buf));
  system(buf);
#ifdef	LWIP_ASP_LINUX
  memset(rxdesc_buf, 0, sizeof(EMAC_DMA_RXDESC) * RXDESC_NUM);
  for (i = 0; i < RXDESC_NUM; i++) {
    rxdesc_buf[i].buf = rx_buf[i];
  }
  rxdesc_rp = 0;
  rxdesc_wp = 0;

  sta_cyc(INT_ETH_RECV);

  if (pthread_create(&h_tapif_thread, NULL, (void *(*)(void *))tapif_thread, netif) != 0) {
    perror("tapif_init: cannot create tapif_thread");
    exit(1);
  }
#else
  sys_thread_new("tapif_thread", tapif_thread, netif, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
#endif

}
/*-----------------------------------------------------------------------------------*/
/*
 * low_level_output():
 *
 * Should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 */
/*-----------------------------------------------------------------------------------*/
#ifdef	LWIP_ASP_LINUX
static err_t
eth_output_netif_low(struct netif *netif, struct pbuf *p)
#else
static err_t
low_level_output(struct netif *netif, struct pbuf *p)
#endif
{
  struct pbuf *q;
  char buf[1514];
  char *bufptr;
  struct tapif *tapif;

  tapif = (struct tapif *)netif->state;
#if 0
    if(((double)rand()/(double)RAND_MAX) < 0.2) {
    printf("drop output\n");
    return ERR_OK;
    }
#endif
  /* initiate transfer(); */

  bufptr = &buf[0];

#ifdef LWIP_ASP_LINUX
  q = p;	// pbuf-chain already extracted in upper layer.
  {
#else
  for(q = p; q != NULL; q = q->next) {
#endif
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
    /* send data from(q->payload, q->len); */
    memcpy(bufptr, q->payload, q->len);
    bufptr += q->len;
  }

  /* signal that packet should be sent(); */
#ifdef LWIP_ASP_LINUX
  if(write(tapif->fd, buf, q->len) == -1) {
#else
  if(write(tapif->fd, buf, p->tot_len) == -1) {
#endif
    perror("tapif: write");
  }
  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
/*
 * low_level_input():
 *
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 */
/*-----------------------------------------------------------------------------------*/
#ifdef	LWIP_ASP_LINUX
static err_t
tapif_input_low(struct tapif *tapif)
#else
static struct pbuf *
low_level_input(struct tapif *tapif)
#endif
{
#ifndef LWIP_ASP_LINUX
  struct pbuf *p, *q;
#endif
  u16_t len;
  char buf[1514];
#ifndef LWIP_ASP_LINUX
  char *bufptr;
#endif

  /* Obtain the size of the packet and put it into the "len"
     variable. */
  len = read(tapif->fd, buf, sizeof(buf));
#if 0
    if(((double)rand()/(double)RAND_MAX) < 0.2) {
    printf("drop\n");
    return NULL;
    }
#endif

#ifdef LWIP_ASP_LINUX
  if (((rxdesc_wp+1) % RXDESC_NUM) != rxdesc_rp) {
    memcpy( rxdesc_buf[rxdesc_wp].buf, buf, len);
    rxdesc_buf[rxdesc_wp].len = len;
    rxdesc_wp = (rxdesc_wp + 1) % RXDESC_NUM;
  } else {
    // drop?
  }
  return ERR_OK;
#else
  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

  if(p != NULL) {
    /* We iterate over the pbuf chain until we have read the entire
       packet into the pbuf. */
    bufptr = &buf[0];
    for(q = p; q != NULL; q = q->next) {
      /* Read enough bytes to fill this pbuf in the chain. The
         available data in the pbuf is given by the q->len
         variable. */
      /* read data into(q->payload, q->len); */
      memcpy(q->payload, bufptr, q->len);
      bufptr += q->len;
    }
    /* acknowledge that packet has been read(); */
  } else {
    /* drop packet(); */
  }

  return p;
#endif
}
/*-----------------------------------------------------------------------------------*/
static void
tapif_thread(void *arg)
{
  struct netif *netif;
  struct tapif *tapif;
  fd_set fdset;
  int ret;

  netif = (struct netif *)arg;
  tapif = (struct tapif *)netif->state;

  while(1) {
    FD_ZERO(&fdset);
    FD_SET(tapif->fd, &fdset);

    /* Wait for a packet to arrive. */
    ret = select(tapif->fd + 1, &fdset, NULL, NULL, NULL);

    if(ret == 1) {
      /* Handle incoming packet. */
      tapif_input(netif);
    } else if(ret == -1) {
      perror("tapif_thread: select");
    }
  }
}
/*-----------------------------------------------------------------------------------*/
/*
 * tapif_input():
 *
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface.
 *
 */
/*-----------------------------------------------------------------------------------*/
static void
tapif_input(struct netif *netif)
{
  struct tapif *tapif;
#ifndef LWIP_ASP_LINUX
  struct eth_hdr *ethhdr;
  struct pbuf *p;
#endif

  tapif = (struct tapif *)netif->state;

#ifdef	LWIP_ASP_LINUX
  if (tapif_input_low(tapif) != ERR_OK) {
    LWIP_DEBUGF(TAPIF_DEBUG, ("tapif_input: tapif_input_low returned NULL\n"));
    return;
  }
#else
  p = low_level_input(tapif);

  if(p == NULL) {
    LWIP_DEBUGF(TAPIF_DEBUG, ("tapif_input: low_level_input returned NULL\n"));
    return;
  }
  ethhdr = (struct eth_hdr *)p->payload;

  switch(htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
  case ETHTYPE_ARP:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    /* full packet send to tcpip_thread to process */
    if (netif->input(p, netif) != ERR_OK) {
      LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
       pbuf_free(p);
       p = NULL;
    }
    break;
  default:
    pbuf_free(p);
    break;
  }
#endif
}
/*-----------------------------------------------------------------------------------*/
/*
 * tapif_init():
 *
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 */
/*-----------------------------------------------------------------------------------*/
err_t
tapif_init(struct netif *netif)
{
  struct tapif *tapif;

  tapif = (struct tapif *)mem_malloc(sizeof(struct tapif));
  if (!tapif) {
    return ERR_MEM;
  }
  netif->state = tapif;
#ifndef LWIP_ASP_LINUX
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;
  netif->mtu = 1500;
#endif
  /* hardware address length */
  netif->hwaddr_len = 6;

  tapif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_IGMP;
#ifndef LWIP_ASP_LINUX

  low_level_init(netif);
#endif

  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/

/*
 * original: fm3_sample_ether_api.c
 */
//#include <kernel.h>
//#include "kernel/kernel_int.h"
#include "kernel_cfg.h"

#include "arch/cc.h"
#include "debug.h"
#include "eth_api.h"

#include <string.h>
//#include "93c46.h"
//#include "common.h"

#ifdef	LWIP_ASP_LINUX
void eth_init(struct netif *netif)
#else
void eth_init(u8_t *hwaddr)
#endif
{
#ifdef	LWIP_ASP_LINUX
	tapif_init(netif);
	eth_init_low(netif);
#else
	read_93c46(4, 6, hwaddr);
	printf("%02x:%02x:%02x:%02x:%02x:%02x\r\n", hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);
	Ether_Init(hwaddr);
	return;
#endif
}


static EMAC_DMA_RXDESC *prxdesc;
static char *ethp;

int eth_input_len(void)
{
	u16_t len = 0;

	wai_sem(SEM_ETH);
#ifndef	LWIP_ASP_LINUX
	prxdesc = RXDESC_Received();
	if( NULL != prxdesc )
	{
		len = prxdesc->RDES0_f.FL;
		ethp = (char *)prxdesc->RDES2;
	}
#else
	prxdesc = NULL;
	if (rxdesc_rp != rxdesc_wp) {
		prxdesc = &rxdesc_buf[rxdesc_rp];
		len = prxdesc->len;
		ethp = (char *)prxdesc->buf;
	}
#endif
	return len;
}

u16_t eth_input_buf(void *payload, u16_t len)
{
	if(len)
	{
		memcpy(payload, ethp, len);
		ethp += len;
		len -= len;
	}
	return len;
}

#ifdef	LWIP_ASP_LINUX
u16_t eth_input_buf_netif(struct netif *netif, struct pbuf *p)
{
	netif = netif;

	return eth_input_buf(p->payload, p->len);
}
#endif

void eth_input_ack(void)
{
#ifndef	LWIP_ASP_LINUX
	prxdesc->RDES0_f.OWN = 1;
#else
	if (prxdesc) {
		prxdesc->len = 0; // clear
		prxdesc = NULL;
		ethp = NULL;
		rxdesc_rp = (rxdesc_rp + 1) % RXDESC_NUM; // rp goes next
	}
#endif
	return;
}

void eth_input_end(void)
{
	sig_sem(SEM_ETH);
	return;
}

void eth_output_start(void)
{
	wai_sem(SEM_ETH);
	return;
}

void eth_output(void *payload, u16_t len)
{
#ifndef	LWIP_ASP_LINUX
	Packet_Send((unsigned char *)payload, (unsigned int)len);
#else
        perror("eth_output: use eth_output_netif, instead of eth_output.");
#endif
	return;
}

#ifdef	LWIP_ASP_LINUX
void eth_output_netif(struct netif *netif, struct pbuf *p)
{
	pbuf_ref(p);
	eth_output_netif_low(netif, p);
	pbuf_free(p);
}
#endif

void eth_output_end(void)
{
	sig_sem(SEM_ETH);
	return;
}

void eth_int(intptr_t exinf)
{
#ifndef LWIP_ASP_LINUX
	ETH_IRQHandler(FM3_ETHERNET_MAC0);
#else
	if (rxdesc_rp != rxdesc_wp) 
#endif
	isig_sem(SEM_RECV);
}
