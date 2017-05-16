#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include <kernel.h>

typedef ID sys_sem_t;
typedef ID sys_mbox_t;
typedef ID sys_thread_t;

#define sys_sem_valid( x ) ( ( ( *x ) == (sys_sem_t)NULL) ? 0 : 1 )
#define sys_sem_set_invalid( x ) ( ( *x ) = (sys_sem_t)NULL )
#define sys_mbox_valid( x ) ( ( ( *x ) == (sys_mbox_t)NULL) ? 0 : 1 )
#define sys_mbox_set_invalid( x ) ( ( *x ) = (sys_mbox_t)NULL )

/* let sys.h use binary semaphores for mutexes */
#define LWIP_COMPAT_MUTEX 1

#endif /* __ARCH_SYS_ARCH_H__ */
