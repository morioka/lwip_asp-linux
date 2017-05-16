#include <kernel.h>
#include "kernel/kernel_int.h"
#include "kernel_cfg.h"

#include "arch/cc.h"
#include "debug.h"
#include "eth_api.h"
#include "irq.h"

void eth_init(u8_t *hwaddr)
{
	irq_init();

	MACInit(hwaddr);
	printf("%02x:%02x:%02x:%02x:%02x:%02x\r\n", hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);

	dly_tsk(6 * 1000);
	ena_int(IRQ2_VECTOR);
	return;
}

int eth_input_len(void)
{
	int len;
	wai_sem(SEM_ETH);
	len = (int)enc424j600PacketReceive_len();
	return len;
}

u16_t eth_input_buf(void *payload, u16_t len)
{
	enc424j600PacketReceive(payload, (int)len);
	return len;
}

void eth_input_ack(void)
{
	return;
}

void eth_input_end(void)
{
	enc424j600PacketReceive_end();
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
	enc424j600PacketSend(len, payload);
	return;
}

void eth_output_end(void)
{
	sig_sem(SEM_ETH);
	return;
}

void eth_int(intptr_t exinf)
{
	isig_sem(SEM_RECV);
	return;
}
