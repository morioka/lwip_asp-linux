#include "target_test.h"

/*#define INT_ETH_RECV 7 */	/* SIGBUS */

extern void eth_int(intptr_t exinf);
extern void task_lwip(intptr_t exinf);
extern void task_thread(intptr_t exinf);
