#include <kernel.h>
#include "kernel/kernel_int.h"
#include "kernel_cfg.h"

#include "arch/cc.h"
#include "bsc.h"
#include "debug.h"
#include "eth_api.h"
#include "irq.h"

extern void TLFA1_error_reset(void);

void eth_init(u8_t *hwaddr)
{
	bsc_init();
	irq_init();

	TLFA1_Init(hwaddr);
	printf("%02x:%02x:%02x:%02x:%02x:%02x\r\n", hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);
	TLFA1_Interrupt_Init(0, 0, TLFA1_error_reset, TLFA1_error_reset);
	TLFA1_SendEnableCtrl(1);
	TLFA1_RecvEnableCtrl(1);
	TLFA1_Start();

	ena_int(IRQ0_VECTOR);
	return;
}

int eth_input_len(void)
{
	int len;
	wai_sem(SEM_ETH);
	len = (int)TLFA1_GetRecvPacket(0, 0);
	return len;
}

u16_t eth_input_buf(void *payload, u16_t len)
{
	if(TLFA1_GetRecvPacket((unsigned char *)payload, (int)len))
	{
		if(E_OK != sig_sem(SEM_RECV))
		{
			TLFA1_error_reset();
		}
	}
	return len;
}

void eth_input_ack(void)
{
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
	TLFA1_SetSendPacket((unsigned char *)payload, (int)len);
	return;
}

void eth_output_end(void)
{
	sig_sem(SEM_ETH);
	return;
}

void eth_int(intptr_t exinf)
{
	if(TLFA1_Interrupt())
	{
		if(E_OK != isig_sem(SEM_RECV))
		{
			TLFA1_error_reset();
		}
	}
	return;
}
