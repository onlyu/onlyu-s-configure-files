#ifndef LTABLE_H
#define LTABLE_H

typedef struct ltable_t *ltable_t;

extern ltable_t ltable_new(int hint, 
							int cmp(const void *x, const void *y),
							unsigned hash(const void *key));
extern void ltable_free(ltable_t *table);
extern int ltable_length(ltable_t table);
extern void ltable_put(ltable_t table, const void *key, void *value);
extern void *ltable_get(ltable_t table, const void *key);
extern void ltable_remove(ltable_t table, const void *key);
extern void ltable_map(ltable_t table, 
						void apply(const void *key, void **value, void *cl),
						void *cl);
extern void **ltable_to_array(ltable_t table, void *end);
	
#endif
