#include <stdarg.h>
#include <stddef.h>
#include "lassert.h"
#include "lmem.h"
#include "llist.h"


llist_t llist_append(llist_t list, llist_t tail)
{
	llist_t *p = &list;
	
	while (*p)
		p = &(*p)->rest;
	*p = tail;
	return list;
}

llist_t llist_copy(llist_t list)
{
	llist_t head, *p = &head;

	for ( ; list; list = list->rest) {
		L_NEW(*p);
		(*p)->first = list->first;
		p = &(*p)->rest;
	}
	*p = NULL;
	return head;
}

llist_t llist_list(void *x, ...)
{
	va_list ap;
	llist_t list, *p = &list;

	va_start(ap, x);
	for ( ; x; x = va_arg(ap, void *)){
		L_NEW(*p);
		(*p)->first = x;
		p = &(*p)->rest;
	}
	*p = NULL;
	return list;
}

llist_t llist_pop(llist_t list, void **x)
{
	llist_t p;

	lassert(list);
	
	p = list->rest;
	if (x) {
		*x = list->first;
		L_FREE(list);
		return p;
	} else {
		return list;
	}
}

llist_t llist_push(llist_t list, void *x)
{
	llist_t p;

	L_NEW(p);
	p->rest = list;	
	p->first = x;
	return p;
}

llist_t llist_reverse(llist_t list)
{
	llist_t head = NULL;
	llist_t next = NULL;

	for (; list; list = next) {
		head = list->rest;
		list->rest = head;
		head = list;	
	}
	return head;
}

int llist_length(llist_t list)
{
	int count;
	
	for (count = 0; list; list = list->rest) 
		count ++;
	return count;
}

void llist_free(llist_t *list)
{
	llist_t next;
	
	lassert(list);
	
	for (; *list; *list = next) {
		next = (*list)->rest;
		L_FREE(*list);
	}
}

void llist_map(llist_t list, void apply(void **x, void *cl), void *cl)
{
	lassert(apply);

	for ( ; list; list = list->rest) {
		apply(&list->first, cl);
	}
}

void **llist_to_array(llist_t list, void *end)
{
	int i, n = llist_length(list);
	void **array = L_ALLOC((n + 1) * sizeof (*array) );

	for (i = 0; i < n; i++){
		array[i] = list->first;
		list = list->rest;
	}
	array[i] = end;
	return array;
}



