//asp\doc\porting.txt
//6.15.1より

#include "kernel/kernel_impl.h"
#include "tlsf.h"

static bool_t	tlsf_initialized = false;

void
initialize_kmm(void)
{
	if (init_memory_pool(kmmsz, kmm) >= 0) {
		tlsf_initialized = true;
	}
}

void *
kernel_malloc(SIZE size)
{
	if (tlsf_initialized) {
		return(malloc_ex(size, kmm));
	}
	else {
		return(NULL);
	}
}

void
kernel_free(void *ptr)
{
	if (tlsf_initialized) {
		free_ex(ptr, kmm);
	}
}
