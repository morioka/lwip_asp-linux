#include <string.h>
#include <kernel.h>
#include "fm3_mb9bxxx.h"

#define CS	*BITBAND(FM3_GPIO_PDORC, 0x0c)	//PortC-C(CN3-24)
#define SK	*BITBAND(FM3_GPIO_PDORC, 0x0d)	//PortC-D(CN3-23)
#define DI	*BITBAND(FM3_GPIO_PDORC, 0x0e)	//PortC-E(CN3-20)
#define DO	*BITBAND(FM3_GPIO_PDIRC, 0x0f)	//PortC-F(CN3-22)

static int initialized = 0;

static void gpio_init(void)
{
	*BITBAND(FM3_GPIO_DDRC, 0x0c) = 1;
	*BITBAND(FM3_GPIO_DDRC, 0x0d) = 1;
	*BITBAND(FM3_GPIO_DDRC, 0x0e) = 1;
	*BITBAND(FM3_GPIO_DDRC, 0x0f) = 0;
	*BITBAND(FM3_GPIO_PCRC, 0x0f) = 1;

	CS = 0;
	SK = 0;
	DI = 0;

	dly_tsk(1);

	initialized = 1;
	return;
}

static int clock(void)
{
	int data;

	dly_tsk(1);
	SK = 1;
	dly_tsk(1);
	data = DO ? 1: 0;

	SK = 0;
	dly_tsk(1);

	return data;
}

unsigned short short_read(int address)
{
	int i;
	unsigned short data;

	if(!initialized)gpio_init();

	CS = 1;
	DI = 1;
	clock();
	DI = 1;
	clock();
	DI = 0;
	clock();

	for(i = 0; i < 6; i++)
	{
		DI = (address & (1 << 5)) ? 1 : 0;
		clock();
		address <<= 1;
	}

	DI = 0;

	data = 0;
	for(i = 0; i < 16; i++)
	{
		data <<= 1;
		data |= clock();
	}

	CS = 0;

	return data;
}

unsigned char *read_93c46(int add, int len, unsigned char *ucp)
{
	unsigned short *buf = (unsigned short*)ucp;

	add /= 2;
	len /= 2;
	while(len--)
	{
		*buf++ = short_read(add++);
		dly_tsk(1);
	}
	const unsigned char no_address[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	if(0 == memcmp(ucp, no_address, 6))
	{
		//ローカルMACアドレスを使用する
		const unsigned char local_address[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
		memcpy(ucp, local_address, 6);
	}
	return ucp;
}
