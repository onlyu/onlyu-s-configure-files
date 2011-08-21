#include <stdlib.h>
#include <string.h>
#include "lassert.h"
#include "lexcept.h"
#include "larena.h"


#define THRESHOLD 10

const lexcept_t arena_new_failed = { "Arena Creation Failed" };
const lexcept_t arena_failed = { "Arena Allocation Failed" };

struct larena_t {
	larena_t prev;
	char *avail;
	char *limit;
};

// checking types
union align {
	int i;
	long l;
	long *lp;
	void *p;
	void (*fp)(void);
	float f;
	double d;
	long double ld;
};

union lheader {
	struct larena_t b;
	union align a;
};

static larena_t freechunks;
static int nfree;

larena_t larena_new(void)
{
	larena_t arena = malloc(sizeof (*arena));
	if (arena == NULL)
		RAISE(arena_new_failed);
	arena->prev = NULL;
	arena->limit = arena->avail = NULL;
	return arena;
}

void larena_dispos(larena_t *ap)
{
	lassert(ap && *ap);
	larena_free(*ap);
	free(*ap);
	*ap = NULL;
}

void *larena_alloc(larena_t arena, long nbytes, const char *file, int line)
{
	lassert(arena);
	lassert(nbytes > 0);
	nbytes = ((nbytes + sizeof(union align) - 1)/ \
			(sizeof (union align))) * ( sizeof (union align));
	while (nbytes > arena->limit - arena->avail) {
		larena_t ptr;
		char *limit;
		if ((ptr = freechunks) != NULL){
			freechunks = freechunks->prev;
			nfree --;
			limit = ptr->limit;
		} else {
			long m = sizeof (union lheader) + nbytes + 10 * 1024;
			ptr = malloc(m);
			if (ptr == NULL) {
				if (file == NULL)
					RAISE(arena_failed);
				else
					lexcept_raise(&arena_failed, file, line);
			}
			limit = (char *)ptr + m;		
		}
		*ptr = *arena;
		arena->avail = (char *)((union lheader *)ptr + 1);
		arena->limit = limit;
		arena->prev = ptr;
	}	
	arena->avail += nbytes;
	return arena->avail - nbytes;
}

void *larena_calloc(larena_t arena, long count, long nbytes, const char *file, int line)
{
	void *ptr;

	lassert(count > 0);
	ptr = larena_alloc(arena, count*nbytes, file, line);
	memset(ptr, '\0', count*nbytes);
	return ptr;
}

void larena_free(larena_t arena)
{
	lassert(arena);
	while (arena->prev) {
		struct larena_t tmp = *arena->prev;
		if (nfree < THRESHOLD) {
			arena->prev->prev = freechunks;
			freechunks = arena->prev;
			nfree ++;
			freechunks->limit = arena->limit;
		} else {
			free(arena->prev);
		}
		*arena = tmp;
	}
	lassert(arena->limit == NULL);
	lassert(arena->avail == NULL);
}
