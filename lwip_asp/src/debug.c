#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <kernel.h>
#include "kernel_cfg.h"
#include "syssvc/serial.h"
#include "debug.h"

void debug_init(void)
{
	serial_opn_por(TASK_PORTID);
	serial_ctl_por(TASK_PORTID, (IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV));
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

	serial_wri_dat(TASK_PORTID, (const char_t*)buf, strlen(buf));
	sig_sem(SEM_DEBUG);
	#endif
	return;
}
