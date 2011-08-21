#ifndef LLIST_H
#define LLIST_H

typedef struct llist_t *llist_t;

struct llist_t {
	llist_t rest;
	void *first;
};

extern llist_t llist_append(llist_t list, llist_t tail);
extern llist_t llist_copy(llist_t list);
extern llist_t llist_list(void *x, ...);
extern llist_t llist_pop(llist_t list, void **x);
extern llist_t llist_push(llist_t list, void *x);
extern llist_t llist_reverse(llist_t list);
extern int llist_length(llist_t list);
extern void llist_free(llist_t *list);
extern void llist_map(llist_t list, void apply(void **x, void *cl), void *cl);
extern void **llist_to_array(llist_t list, void *end);

#endif
