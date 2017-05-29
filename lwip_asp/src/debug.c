#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <kernel.h>
#include "kernel_cfg.h"
#include "syssvc/serial.h"
#include "debug.h"

#include "lwip/lwipopts.h"
#include "lwip/opt.h"

unsigned char debug_flags;

void debug_init(void)
{
#ifndef	LWIP_ASP_LINUX
	serial_opn_por(TASK_PORTID);
	serial_ctl_por(TASK_PORTID, (IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV));
#else
	debug_flags = LWIP_DBG_OFF;
	debug_flags |= (LWIP_DBG_ON|LWIP_DBG_TRACE|LWIP_DBG_STATE|LWIP_DBG_FRESH|LWIP_DBG_HALT);
#endif

	return;
}

void debug_printf(const char *control, ...)	/* 名前がぶつからないように */
{
	#ifdef USE_DEBUG_PRINTF
	wai_sem(SEM_DEBUG);

	char buf[256];
	va_list vargs;

	va_start(vargs, control);
	vsprintf(buf, control, vargs);
	va_end(vargs);

	serial_wri_dat(TASK_PORTID, (const char*)buf, strlen(buf));
	sig_sem(SEM_DEBUG);
	#endif
	return;
}
