#include "target_test.h"

#define INT_ETH_RECV 27

//extern void eth_int(intptr_t exinf);
extern void eth_int();
extern void task_lwip(intptr_t exinf);
extern void task_thread(intptr_t exinf);
