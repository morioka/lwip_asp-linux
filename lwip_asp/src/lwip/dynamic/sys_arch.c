#include <string.h>
#include <kernel.h>
#include "kernel_cfg.h"
#if 1
#include "debug.h"
#else
#ifdef USE_DEBUG_PRINTF
#undef USE_DEBUG_PRINTF
#endif
#define printf(...)
#endif

#include "arch/sys_arch.h"

#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"

void sys_init(void)
{
	return;
}

err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	err_t ret = ERR_VAL;

	printf("sys_sem_new in\n");
	if(NULL != sem)
	{
		T_CSEM csem = { TA_TPRI, (uint_t)count, (uint_t)1 };
		ID id = acre_sem(&csem);
		if(0 < id)
		{
			*sem = id;
			ret = ERR_OK;
			printf("sys_sem_new out %d, %d\n", *sem, count);
		}
		else
		{
			*sem = (sys_sem_t)NULL;
			printf("sys_sem_new out ERROR!\n");
		}
	}
	return ret;
}

void sys_sem_free(sys_sem_t *sem)
{
	printf("sys_sem_free in %d\n", *sem);
	del_sem(*sem);
	*sem = (sys_sem_t)NULL;
	printf("sys_sem_free out\n");
	return;
}

void sys_sem_signal(sys_sem_t *sem)
{
	printf("sys_sem_signal in %d\n", *sem);
	sig_sem(*sem);
	printf("sys_sem_signal out %d\n", *sem);
	return;
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	SYSTIM systim0, systim1;
	ER ercd;

	get_tim(&systim0);
	printf("sys_arch_sem_wait in %d\n", *sem);
	if(timeout)
	{
		ercd = twai_sem(*sem, (TMO)timeout);
	}
	else
	{
		ercd = wai_sem(*sem);
	}

	if(E_OK == ercd)
	{
		get_tim(&systim1);
		printf("sys_arch_sem_wait out %d, E_OK\n", *sem);
		return (u32_t)(systim1 - systim0);
	}
	printf("sys_arch_sem_wait out %d, SYS_ARCH_TIMEOUT\n", *sem);
	return SYS_ARCH_TIMEOUT;
}

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	err_t ret = ERR_VAL;

	printf("sys_mbox_new in\n");
	if(NULL != mbox)
	{
		T_CDTQ cdtq = { TA_NULL, (uint_t)size, (void *)NULL };
		ID id = acre_dtq(&cdtq);
		if(0 < id)
		{
			*mbox = id;
			ret = ERR_OK;
			printf("sys_mbox_new out %d, %d\n", *mbox, size);
		}
		else
		{
			*mbox = (sys_mbox_t)NULL;
			printf("sys_mbox_new out ERROR!\n");
		}
	}
	return ret;
}

void sys_mbox_free(sys_mbox_t *mbox)
{
	printf("sys_mbox_free in %d\n", *mbox);
	del_dtq(*mbox);
	*mbox = (sys_mbox_t)NULL;
	printf("sys_mbox_free out\n");
	return;
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
	printf("sys_mbox_post in %d, 0x%08x\n", *mbox, msg);
	snd_dtq(*mbox, (intptr_t)msg);
	printf("sys_mbox_post out %d, 0x%08x\n", *mbox, msg);
	return;
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	err_t ret = ERR_VAL;

	printf("sys_mbox_trypost in %d, 0x%08x\n", *mbox, msg);
	if(E_OK == psnd_dtq(*mbox, (intptr_t)msg))
	{
		ret = ERR_OK;
	}
	printf("sys_mbox_trypost out %d, 0x%08x %d\n", *mbox, msg, ret);
	return ret;
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	SYSTIM systim0, systim1;
	ER ercd;

	get_tim(&systim0);
	printf("sys_arch_mbox_fetch in %d\n", *mbox);
	if(timeout)
	{
		ercd = trcv_dtq(*mbox, (intptr_t *)msg, timeout);
	}
	else
	{
		ercd = rcv_dtq(*mbox, (intptr_t*)msg);
	}

	if(E_OK == ercd)
	{
		get_tim(&systim1);
		printf("sys_arch_mbox_fetch out %d, E_OK\n", *mbox);
		return (u32_t)(systim1 - systim0);
	}
	printf("sys_arch_mbox_fetch out %d, SYS_ARCH_TIMEOUT\n", *mbox);
	return SYS_ARCH_TIMEOUT;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	printf("sys_arch_mbox_tryfetch in %d\n", *mbox);
	if(E_OK == prcv_dtq(*mbox, (intptr_t *)msg))
	{
		printf("sys_arch_mbox_tryfetch out %d, 0x%08x, ERR_OK\n", *mbox, *msg);
		return (u32_t)ERR_OK;
	}
	printf("sys_arch_mbox_tryfetch out %d, SYS_MBOX_EMPTY\n", *mbox);
	return SYS_MBOX_EMPTY;
}

sys_thread_t sys_thread_new(const char *name, void(*thread)(void *), void *arg, int stacksize, int prio)
{
	printf("sys_thread_new in\n");

	T_CTSK ctsk = { TA_ACT, (intptr_t)arg, (TASK)thread, (PRI)prio, (SIZE)stacksize, (STK_T *)NULL };
	ID id = acre_tsk(&ctsk);

	printf("sys_thread_new %s, %d, %d, %d\n", name, stacksize, prio, id);
	return (sys_thread_t)id;
}

sys_prot_t sys_arch_protect(void)
{
	bool_t state = sns_loc();
	if(!state)
	{
		loc_cpu();
	}
	return (sys_prot_t)(~state);
}

void sys_arch_unprotect(sys_prot_t pval)
{
	if(pval)
	{
		unl_cpu();
	}
	return;
}

u32_t sys_now(void)
{
	SYSTIM systim;

	get_tim(&systim);
	return (u32_t)systim;
}
