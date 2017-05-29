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

#define LWIP_NUM_SEM (SEM_X - SEM_1 + 1)
#define LWIP_NUM_DTQ (DTQ_X - DTQ_1 + 1)
#define LWIP_NUM_TSK (TSK_X - TSK_1 + 1)

struct pk_sem pk_sem[LWIP_NUM_SEM];
struct pk_dtq pk_dtq[LWIP_NUM_DTQ];
struct pk_tsk pk_tsk[LWIP_NUM_TSK];

void sys_init(void)
{
	ID i;

	for(i = 0; i < LWIP_NUM_SEM; i++)
	{
		pk_sem[i].id = SEM_1 + i;
		pk_sem[i].used = 0;
	}

	for(i = 0; i < LWIP_NUM_DTQ; i++)
	{
		pk_dtq[i].id = DTQ_1 + i;
		pk_dtq[i].used = 0;
	}

	for(i = 0; i < LWIP_NUM_TSK; i++)
	{
		pk_tsk[i].id = TSK_1 + i;
		pk_tsk[i].used = 0;
	}
	return;
}

err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	err_t ret = ERR_VAL;
	ID i, id;

	wai_sem(SEM_SEM);
	printf("sys_sem_new in\n");
	for(i = 0; i < LWIP_NUM_SEM; i++)
	{
		if(!pk_sem[i].used)
		{
			pk_sem[i].used = 1;
			*sem = &pk_sem[i];
			id = pk_sem[i].id;
			ini_sem(id);

			if(count)
			{
				sig_sem(id);
			}
			ret = ERR_OK;
			break;
		}
	}

	if(ERR_VAL == ret)
	{
		*sem = (sys_sem_t)NULL;
		printf("sys_sem_new out ERROR!\n");
	}
	else
	{
		printf("sys_sem_new out %d, %d\n", id, count);
	}
	sig_sem(SEM_SEM);
	return ret;
}

void sys_sem_free(sys_sem_t *sem)
{
	printf("sys_sem_free in %d\n", (*sem)->id);
	(*sem)->used = 0;
	*sem = (sys_sem_t)NULL;
	printf("sys_sem_free out\n");
	return;
}

void sys_sem_signal(sys_sem_t *sem)
{
	printf("sys_sem_signal in %d\n", (*sem)->id);
	sig_sem((*sem)->id);
	printf("sys_sem_signal out %d\n", (*sem)->id);
	return;
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	SYSTIM systim0, systim1;
	ER ercd;

	get_tim(&systim0);
	printf("sys_arch_sem_wait in %d\n", (*sem)->id);
	if(timeout)
	{
		ercd = twai_sem((*sem)->id, (TMO)timeout);
	}
	else
	{
		ercd = wai_sem((*sem)->id);
	}

	if(E_OK == ercd)
	{
		get_tim(&systim1);
		printf("sys_arch_sem_wait out %d, E_OK\n", (*sem)->id);
		return (u32_t)(systim1 - systim0);
	}
	printf("sys_arch_sem_wait out %d, SYS_ARCH_TIMEOUT\n", (*sem)->id);
	return SYS_ARCH_TIMEOUT;
}

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	err_t ret = ERR_VAL;
	ID i, id;

	wai_sem(SEM_DTQ);
	printf("sys_mbox_new in\n");
	for(i = 0; i < LWIP_NUM_DTQ; i++)
	{
		if(!pk_dtq[i].used)
		{
			pk_dtq[i].used = 1;
			*mbox = &pk_dtq[i];
			id = pk_dtq[i].id;
			ini_dtq(id);
			ret = ERR_OK;
			break;
		}
	}

	if(ERR_VAL == ret)
	{
		*mbox = (sys_mbox_t)NULL;
		printf("sys_mbox_new out ERROR!\n");
	}
	else
	{
		printf("sys_mbox_new out %d, %d\n", id, size);
	}
	sig_sem(SEM_DTQ);
	return ret;
}

void sys_mbox_free(sys_mbox_t *mbox)
{
	printf("sys_mbox_free in %d\n", (*mbox)->id);
	(*mbox)->used = 0;
	*mbox = (sys_mbox_t)NULL;
	printf("sys_mbox_free out\n");
	return;
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
	printf("sys_mbox_post in %d, 0x%08x\n", (*mbox)->id, (unsigned int)msg);
	snd_dtq((*mbox)->id, (intptr_t)msg);
	printf("sys_mbox_post out %d, 0x%08x\n", (*mbox)->id, (unsigned int)msg);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	err_t ret = ERR_VAL;

	printf("sys_mbox_trypost in %d, 0x%08x\n", (*mbox)->id, (unsigned int)msg);
	if(E_OK == psnd_dtq((*mbox)->id, (intptr_t)msg))
	{
		ret = ERR_OK;
	}
	printf("sys_mbox_trypost out %d, 0x%08x, %d\n", (*mbox)->id, (unsigned int)msg, ret);
	return ret;
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	SYSTIM systim0, systim1;
	ER ercd;

	get_tim(&systim0);
	printf("sys_arch_mbox_fetch in %d\n", (*mbox)->id);
	if(timeout)
	{
		ercd = trcv_dtq((*mbox)->id, (intptr_t *)msg, timeout);
	}
	else
	{
		ercd = rcv_dtq((*mbox)->id, (intptr_t *)msg);
	}

	if(E_OK == ercd)
	{
		get_tim(&systim1);
		printf("sys_arch_mbox_fetch out %d, E_OK\n", (*mbox)->id);
		return (u32_t)(systim1 - systim0);
	}
	printf("sys_arch_mbox_fetch out %d, SYS_ARCH_TIMEOUT\n", (*mbox)->id);
	return SYS_ARCH_TIMEOUT;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	printf("sys_arch_mbox_tryfetch in %d\n", (*mbox)->id);
	if(E_OK == prcv_dtq((*mbox)->id, (intptr_t *)msg))
	{
		printf("sys_arch_mbox_tryfetch out %d, 0x%08x, ERR_OK\n", (*mbox)->id, (unsigned int)(*msg));
		return (u32_t)ERR_OK;
	}
	printf("sys_arch_mbox_tryfetch out %d, SYS_MBOX_EMPTY\n", (*mbox)->id);
	return SYS_MBOX_EMPTY;
}

sys_thread_t sys_thread_new(const char *name, void (*thread)(void *), void *arg, int stacksize, int prio)
{
	sys_thread_t res = NULL;
	ID i, id;

	wai_sem(SEM_TSK);
	printf("sys_thread_new in\n");
	for(i = 0; i < LWIP_NUM_TSK; i++)
	{
		if(!pk_tsk[i].used)
		{
			id = pk_tsk[i].id;
			if(E_OK == act_tsk(id))
			{
				pk_tsk[i].used = 1;
				strncpy(pk_tsk[i].name, name, 128);
				pk_tsk[i].thread = thread;
				pk_tsk[i].arg = arg;
				pk_tsk[i].stacksize = stacksize;
				pk_tsk[i].prio = prio;

				if(E_OK == chg_pri(id, (PRI)prio))
				{
					if(E_OK == snd_dtq(DTQ_TSK, (intptr_t)&pk_tsk[i]))
					{
						printf("sys_thread_new %s, %d, %d, %d\n", name, stacksize, prio, id);
						res = &pk_tsk[i];
						break;
					}
				}
			}
		}
	}
	sig_sem(SEM_TSK);
	return res;
}

void task_thread(intptr_t exinf)
{
	struct pk_tsk *ppk_tsk;
	rcv_dtq(DTQ_TSK, (intptr_t*)&ppk_tsk);
	printf("task_thread start %s, %d, %d, %d\n", ppk_tsk->name, ppk_tsk->stacksize, ppk_tsk->prio, ppk_tsk->id);

	(ppk_tsk->thread)(ppk_tsk->arg);

	ppk_tsk->used = 0;
	ext_tsk();
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
