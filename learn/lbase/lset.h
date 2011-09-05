#ifndef LSET_H
#define LSET_H

typedef struct lset_t *lset_t;

extern lset_t lset_new(int lint,
					   int cmp(const void *x, const void *y),
					   unsigned hash(const void *x));
extern void lset_free(lset_t *set);

extern int lset_length(lset_t set);
extern int lset_member(lset_t set, const void *member);
extern void lset_put(lset_t set, const void *member);
extern void *lset_remove(lset_t set, const void *member);

extern void lset_map(lset_t set,
					 void apply(const void *member, void *cl),
					 void *cl);
extern void **lset_to_array(lset_t set, void *end);

extern lset_t lset_union(lset_t s, lset_t t);
extern lset_t lset_inter(lset_t s, lset_t t);
extern lset_t lset_minus(lset_t s, lset_t t);
extern lset_t lset_diff(lset_t s, lset_t t);

#endif
