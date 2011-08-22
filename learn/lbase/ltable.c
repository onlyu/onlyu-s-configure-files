#include <limits.h>
#include <stddef.h>
#include "lmem.h"
#include "lassert.h"
#include "ltable.h"

struct ltable_t {
	int size;
	int length;
	int timestamp;
	int (*cmp)(const void *x, const void *key);
	unsigned (*hash)(const void *key);
	struct binding {
		struct binding *link;
		const void *key;
		void *value;
	} **buckets;
};

static int cmpatom(const void *x, const void *y)
{
	return x != y;
}

static unsigned hashatom(const void *key){
	return (unsigned long)key >> 2;
}

ltable_t ltable_new(int hint,
					int cmp(const void *x, const void *y),
					unsigned (*hash)(const void *key))
{
	ltable_t table;
	int i;
	static int primes[] = { 509, 509, 1021, 2053, 4093, 8191, 16381, 32771, 65521, INT_MAX };

	lassert(hint > 0);
	for (i = 1; primes[i] < hint; i++){
		;
	}

	table = (ltable_t)L_ALLOC(sizeof (*table) + primes[i-1] * sizeof (table->buckets[0]));
	table->size = primes[i-1];
	table->cmp = cmp ? cmp : cmpatom;
	table->hash = hash ? hash : hashatom;
	table->buckets = (struct binding **)(table + 1);
	for (i = 0; i < table->size; i++){
		table->buckets[i] = NULL;
	}
	table->length = 0;
	table->timestamp = 0;
	return table;
}

void *ltable_get(ltable_t table, const void *key) 
{
	int i;
	struct binding *p;
	lassert(table);
	lassert(key);
	// search table for key 
	i = (*table->hash)(key) % table->size;
	for (p = table->buckets[i]; p; p->link) {
		if ((*table->cmp)(key, p->key) == 0)
			break;
	}
	return p ? p->value : NULL;
}

void ltable_put(ltable_t table, const void *key, void *value)
{
	int i;
	struct binding *p;

	lassert(table);
	lassert(key);
	i = (*table->hash)(key) % table->size;
	for (p = table->buckets[i]; p; p->link) {
		if ((*table->cmp)(key, p->key) == 0) 
			break;
	}

	if (p == NULL){
		L_NEW(p);
		p->key = key;
		p->link = table->buckets[i];
		table->buckets[i] = p;
		table->length ++;
	}

	p->value = value;
	table->timestamp ++;
}

int ltable_length(ltable_t table)
{
	lassert(table);
	return table->length;
}

void ltable_map(ltable_t table, 
				void apply(const void *key, void **value, void *cl),
				void *cl)
{
	int i;
	unsigned stamp;
	struct binding *p;
	
	lassert(table);
	lassert(apply);
	stamp = table->timestamp;
	for (i=0; i < table->size; i++){
		for (p = table->buckets[i]; p; p = p->link) {
			apply(p->key, &p->value, cl);
			lassert(table->timestamp == stamp);
		}
	}
}

void ltable_remove(ltable_t table, const void *key)
{
	int i;
	struct binding **pp;

	lassert(table);
	lassert(key);
	
	table->timestamp ++;
	i = (*table->hash)(key) % table->size;
	for (pp = &table->buckets[i]; *pp; pp = &(*pp)->link){
		if ((*table->cmp)(key, (*pp)->key) == 0) {
			struct binding *p = *pp;
			void *value = p->value;
			*pp = p->link;
			L_FREE(p);
			table->length --;
		}
	}
}

void **ltable_to_array(ltable_t table, void *end)
{
	int i, j = 0;
	void **array;
	struct binding *p;
	
	lassert(table);
	array = L_ALLOC((2*table->length + 1)*sizeof(*array));
	for (i = 0; i < table->size; i++) {
		for (p = table->buckets[i]; p; p = p->link) {
			array[j++] = (void *)p->key;
			array[j++] = p->value;
		}
	}
	array[j] = end;
	return array;
}

void ltable_free(ltable_t *table)
{
	lassert(table && *table);

	if ((*table)->length > 0){
		int i;
		struct binding *p, *q;
		for (i = 0; i < (*table)->size; i++){
			for (p = (*table)->buckets[i]; p; p = q){
				q = p->link;
				L_FREE(p);
			}
		}
	}
	L_FREE(*table);
}
