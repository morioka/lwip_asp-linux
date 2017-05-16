#include "kernel/kernel_impl.h"
#include "kernel/task.h"

void init_tskinictxb(TSKINICTXB *tskinictxb, void *stk, T_CTSK *pk_ctsk)
{
	tskinictxb->stksz = pk_ctsk->stksz;
	tskinictxb->stk_bottom = stk + ROUND_STK_T(pk_ctsk->stksz);
	return;
}

void term_tskinictxb(TSKINICTXB *tskinictxb)
{
//	if ((p_tinib->tskatr & TA_MEMALLOC) != 0U) {
			kernel_free(tskinictxb->stk_bottom - ROUND_STK_T(tskinictxb->stksz));
//	}
	return;
}
