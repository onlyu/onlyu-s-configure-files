#ifndef _LSTACK_INCLUDE
#define _LSTACK_INCLUDE

typedef struct lstack_t *lstack_t;

extern lstack_t lstack_new(void);
extern int lstack_is_empty(lstack_t stk);
extern int lstack_size(lstack_t stk);
extern void lstack_push(lstack_t stk, void *item);
extern void *lstack_pop(lstack_t stk);
extern void lstack_free(lstack_t stk);

#endif
