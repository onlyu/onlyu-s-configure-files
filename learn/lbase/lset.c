#include <limits.h>
#include <stddef.h>
#include "lmem.h"
#include "lassert.h"
#include "larith.h"
#include "lset.h"

struct lset_t {
	int length;
	unsigned timestamp;
	int (*cmp)(const void *x, const void *y);
	unsigned (*hash)(const void *x);
	int size;
	struct member{
		struct member *link;
		const void *member;
	} **buckets;
};

static int cmpatom(const void *x, const void *y)
{
	return x != y;
}

static unsigned hashatom(const void *x)
{
	return (unsigned) x >> 2;
}

lset_t lset_new(int hint,
			 	int cmp(const void *x, const void *y),
				unsigned hash(const void *x))
{
	lset_t set;
	int i;
	static int primes[] = { 509, 509, 1021, 2053, 4093, 8191, 16381, 32771, 65521, INT_MAX };
	
	lassert(hint >= 0);
	for (i = 1; primes[i] < hint; i++) ;
	set = (lset_t) L_ALLOC(sizeof (*set) + primes[i-1]*sizeof (set->buckets[0]));
	set->size = primes[i-1];
	set->cmp = cmp ? cmp : cmpatom;
	set->hash = hash ? hash : hashatom;
	set->buckets = (struct member **)(set + 1);
	for (i = 0; i < set->size; i++)
		set->buckets[i] = NULL;
	set->length = 0;
	set->timestamp = 0;
	return set;
}

int lset_member(lset_t set, const void *member) 
{
	int i;
	struct member *p;

	lassert(set);
	lassert(member);
	i = (*set->hash)(member) % set->size;
	for (p = set->buckets[i]; p; p = p->link)
		if ((*set->cmp)(member, p->member) == 0)
			break;
	return p != NULL;
}

void lset_put(lset_t set, const void *member)
{
	int i;
	struct member *p;

	lassert(set);
	lassert(member);
	i = (*set->hash)(member) % set->size;
	for (p = set->buckets[i]; p; p = p->link)
		if ((*set->cmp)(member, p->member) == 0)
			break;
	if (p == NULL){
		L_NEW(p);
		p->member = member;
		p->link = set->buckets[i];
		set->buckets[i] = p;
		set->length ++;
	} else {
		p->member = member;
	}
	set->timestamp ++;
	
}

void *lset_remove(lset_t set, const void *member)
{
	int i;
	struct member **pp;
	
	lassert(set);
	lassert(member);
	set->timestamp ++;

	i = (*set->hash)(member) % set->size;

	for (pp = &set->buckets[i]; *pp; pp = &(*pp)->link){
		if((*set->cmp)(member, (*pp)->member) == 0) {
			struct member *p = *pp;
			**pp = *p->link;
			member = p->member;
			L_FREE(p);
			set->length --;
			return (void *)member;
		}
	}	
	return NULL;
}

int lset_legnth(lset_t set)
{
	lassert(set);
	return set->length;
}

void lset_free(lset_t *set)
{
	lassert(set && *set);
	if ((*set)->length > 0) {
		int i;
		struct member *p, *q;
		for (i = 0; i < (*set)->size; i++)
			for (p = (*set)->buckets[i]; p; p = q) {
				q = p->link;
				L_FREE(p);
			}
	}
	L_FREE(*set);
}

void lset_map(lset_t set,
			  void apply(const void *member, void *cl),
			  void *cl)
{
	int i;
	unsigned stamp;
	struct member *p;

	lassert(set);
	lassert(apply);
	
	stamp = set->timestamp;
	for (i = 0; i < set->size; i++)
		for (p = set->buckets[i]; p; p = p->link) {
			apply(p->member, cl);
			assert(set->timestamp == stamp);
		}
}

void **lset_to_array(lset_t set, void *end)
{
	int i, j = 0;
	void **array;
	struct member *p;

	lassert(set);
	array = L_ALLOC((set->length + 1) * sizeof (*array));
	for (i = 0; i < set->size; i++)
		for (p = set->buckets[i]; p; p = p->link)
			array[j++] = (void *)p->member;

	array[j] = end;
	return array;
}

#define FOR_EACH(q, t) \
	int i;\
	for (i = 0; i < t->size; i++)\
		for (q = t->buckets[i]; q; q = q->link)

#define ADD_TO_SET(q, set)\ 
	{ \
		struct member *p; \
		const void *member = q->member; \
		int i = (*set->hash)(member) % set->size; \
		L_NEW(p); \
		p->member = member; \
		p->link = set->buckets[i]; \
		set->buckets[i] = p; \
		set->length ++; \
	}


static lset_t copy(lset_t t, int hint)
{
	lset_t set;
	
	lassert(t);
	set = lset_new(hint, t->cmp, t->hash);
	{
		struct member *q;
		FOR_EACH(q, t) {
			ADD_TO_SET(q, set);
		}
	}
	return set;
}

lset_t lset_union(lset_t s, lset_t t)
{
	if (s == NULL) {
		lassert(t);
		return copy(t, t->size);
	} else if ( t == NULL )
		return copy(s, s->size);
	else {
		lset_t set = copy(s, larith_max(s->size, t->size));
		lassert(s->cmp == t->cmp && s->hash == t->hash);
		{
			struct member *q;
			FOR_EACH(q, t){
				lset_put(set, q->member);
			}
		}
		return set;
	}
}

lset_t lset_inter(lset_t s, lset_t t)
{
	if (s == NULL){
		lassert(t);
		return lset_new(t->size, t->cmp, t->hash);
	} else if (t == NULL){
		return lset_new(s->size, s->cmp, s->hash);
	}else if (s->length < t->length){
		return lset_inter(t, s);
	}else{
		lset_t set = lset_new(larith_min(s->size, t->size), s->cmp, s->hash);
		lassert(s->cmp == t->cmp && s->hash == t->hash);
		{
			struct member *q;
			FOR_EACH(q, t){
				if (lset_member(s, q->member))
					ADD_TO_SET(q, set);
			}
		}
		return set;
	}
}

lset_t lset_minus(lset_t s, lset_t t)
{
	if (s = NULL){
		lassert(t);
		return copy(t, t->size);
	} else if (t == NULL) {
		return copy(s, s->size);
	} else {
		lset_t set = lset_new(larith_min(s->size, t->size), s->cmp, s->hash);
		lassert(s->cmp == t->cmp && s->hash == t->hash);
		{
			struct member *q;
			FOR_EACH(q, t){
				if (!lset_member(s, q->member))
					ADD_TO_SET(q, set);
			}
		}
		return set;
	}

}

lset_t lset_diff(lset_t s, lset_t t)
{
	if (s = NULL){
		lassert(t);
		return copy(t, t->size);
	} else if (t == NULL){
		return copy(s, s->size);
	} else {
		lset_t set = Set_new(larith_min(s->size, t->size), s->cmp, s->hash);
		lassert(s->cmp == t->cmp && s->hash == t->hash);
		{
			struct member *q;
			FOR_EACH(q, t){
				if (!lset_member(s, q->member))
					ADD_TO_SET(q, set);
			}
		}
		{ lset_t u = t; t = s; s = u; }
		{
			struct member *q;
			FOR_EACH(q, t){
				if (!lset_member(s, q->member))
					ADD_TO_SET(q, set);
			}
		}
		return set;
	}
}
