extern void debug_init(void);
extern void debug_printf(const char *control, ...);

#ifdef USE_DEBUG_PRINTF
//#define printf(...) debug_printf(__VA_ARGS__)
#define printf(...)
#else
#define printf(...)
#endif
