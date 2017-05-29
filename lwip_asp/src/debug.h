#ifndef	DEBUG_PRINTF_H
#define DEBUG_PRINTF_H

extern void debug_init(void);
extern void debug_printf(const char *control, ...);

#ifndef LWIP_ASP_LINUX
#ifdef USE_DEBUG_PRINTF
#define printf(...) debug_printf(__VA_ARGS__)
//#define printf(...) debug_printf(__VA_ARGS__)
//#define printf(fmt, ...) debug_printf(fmt, ## __VA_ARGS__)
//#define	printf	debug_printf
#else
#define printf(...)
#endif
#endif	/* !LWIP_ASP_LINUX */

#endif /* DEBUG_PRINTF_H */
