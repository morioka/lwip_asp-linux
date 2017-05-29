extern void debug_init(void);
extern void debug_printf(const char *control, ...);

#ifndef	LWIP_ASP_LINUX
#ifdef USE_DEBUG_PRINTF
#define printf(...) debug_printf(__VA_ARGS__)
#else
#define printf(...)
#endif
#endif
