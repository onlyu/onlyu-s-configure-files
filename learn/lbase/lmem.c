#include <stdlib.h>
#include <stddef.h>
#include "lmem.h"
#include "lassert.h"
#include "lexcept.h"

const lexcept_t mem_failed = { "Allocation Failed" };

void *lmem_alloc(long nbytes, const char *file, int line)
{
	void *ptr;
	
	lassert(nbytes > 0);
	ptr = malloc(nbytes);
	if (ptr == NULL){
		if (file == NULL)
			L_RAISE(mem_failed);
		else
			lexcept_raise(&mem_failed, file, line);
	}
	return ptr;
}

void *lmem_calloc(long count, long nbytes, const char *file, int line)
{
	void *ptr;

	lassert(count > 0);
	lassert(nbytes > 0);
	ptr = calloc(count, nbytes);
	if (ptr == NULL)
		if (file == NULL)
			L_RAISE(mem_failed);
		else
			lexcept_raise(&mem_failed, file, line);
	return ptr;
}

void lmem_free(void *ptr, const char *file, int line)
{
	if (ptr)
		free(ptr);
}

void *lmem_resize(void *ptr, long nbytes, const char *file, int line)
{
	lassert(ptr);
	lassert(nbytes > 0);
	ptr = realloc(ptr, nbytes);
	if (ptr == NULL)
		if (file == NULL)
			L_RAISE(mem_failed);
		else
			lexcept_raise(&mem_failed, file, line);
	return ptr;
}
