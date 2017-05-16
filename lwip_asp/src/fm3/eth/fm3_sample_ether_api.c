//#include <kernel.h>
//#include "kernel/kernel_int.h"
#include "kernel_cfg.h"

#include "arch/cc.h"
#include "debug.h"
#include "eth_api.h"

#include <string.h>
#include "93c46.h"
#include "common.h"

void eth_init(u8_t *hwaddr)
{
	read_93c46(4, 6, hwaddr);
	printf("%02x:%02x:%02x:%02x:%02x:%02x\r\n", hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);
	Ether_Init(hwaddr);
	return;
}

static EMAC_DMA_RXDESC *prxdesc;
static char *ethp;

int eth_input_len(void)
{
	u16_t len = 0;

	wai_sem(SEM_ETH);
	prxdesc = RXDESC_Received();
	if( NULL != prxdesc )
	{
		len = prxdesc->RDES0_f.FL;
		ethp = (char *)prxdesc->RDES2;
	}
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

void eth_input_ack(void)
{
	prxdesc->RDES0_f.OWN = 1;
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
	Packet_Send((unsigned char *)payload, (unsigned int)len);
	return;
}

void eth_output_end(void)
{
	sig_sem(SEM_ETH);
	return;
}

void eth_int(intptr_t exinf)
{
	ETH_IRQHandler(FM3_ETHERNET_MAC0);
	isig_sem(SEM_RECV);
}
