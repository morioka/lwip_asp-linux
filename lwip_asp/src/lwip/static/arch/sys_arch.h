#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include <kernel.h>

struct pk_sem {
	ID id;
	int used;
};

struct pk_dtq {
	ID id;
	int used;
};

struct pk_tsk {
	ID id;
	int used;
	char name[128];
	void (*thread)(void *);
	void *arg;
	int stacksize;
	int prio;
};

typedef struct pk_sem *sys_sem_t;
typedef struct pk_dtq *sys_mbox_t;
typedef struct pk_tsk *sys_thread_t;

#define sys_sem_valid( x ) ( ( ( *x ) == (sys_sem_t)NULL) ? 0 : 1 )
#define sys_sem_set_invalid( x ) ( ( *x ) = (sys_sem_t)NULL )
#define sys_mbox_valid( x ) ( ( ( *x ) == (sys_mbox_t)NULL) ? 0 : 1 )
#define sys_mbox_set_invalid( x ) ( ( *x ) = (sys_mbox_t)NULL )

/* let sys.h use binary semaphores for mutexes */
#define LWIP_COMPAT_MUTEX 1

#endif /* __ARCH_SYS_ARCH_H__ */
