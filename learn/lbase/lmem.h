#ifndef LMEM_H
#define LMEM_H

#include <stdlib.h>
#include "lexcept.h"

extern const lexcept_t mem_failed;

extern void *lmem_alloc(long nbyte, const char *file, int line);
extern void *lmem_calloc(long count, long nbyte, const char *file, int line);
extern void lmem_free(void *ptr, const char *file, int line);
extern void *lmem_resize(void *ptr, long nbytes, const char *file, int line);

#define L_ALLOC(nbytes) \
	lmem_alloc((nbytes), __FILE__, __LINE__)
#define L_CALLOC(count, nbytes) \
	lmem_calloc((count), (nbytes), __FILE__, __LINE__)
#define L_NEW(p) \
	((p) = L_ALLOC((long)sizeof *(p)))
#define L_NEWO(p) \
	((p) = L_CALLOC(1, (long)sizeof *(p)))
#define L_FREE(ptr) \
	((void)(lmem_free((ptr), __FILE__, __LINE__), (ptr) = 0))
#define L_RESIZE(ptr, nbytes) \
	((ptr) = lmem_resize((ptr), (nbytes), __FILE__, __LINE__))

#endif
