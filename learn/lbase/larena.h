#ifndef LARENA_H
#define LARENA_H

#include "lexcept.h"

typedef struct larena_t *larena_t;

extern const lexcept_t arena_new_failed;
extern const lexcept_t arena_failed;

extern larena_t larena_new(void);
extern void larena_dispose(larena_t *ap);

extern void *larena_alloc(larena_t arena, long nbytes, const char *file, int line);
extern void *larena_calloc(larena_t arena, long count, long nbytes, const char *file, int line);
extern void larena_free(larena_t arena);

#endif

