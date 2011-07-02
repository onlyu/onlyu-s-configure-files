#include <stdlib.h>
#include "lstack.h"

struct lstack_t {
	int count;
	struct elem{
		void *obj;
		struct elem *link;
	} *head;
};

lstack_t lstack_new(void)
{
	lstack_t lstack = (lstack_t)malloc(sizeof(lstack_t));
	lstack->count = 0;
	lstack->head = NULL;
	return lstack;
}

int lstack_empty(lstack_t lstack)
{
	assert(lstack);
	return lstack->count == 0;
}

void lstack_push(lstack_t lstack, void *item)
{
	assert(lstack);
	struct elem *elem = (struct elem *)malloc(sizeof(struct elem));
	elem->obj = item;
	elem->link = lstack->head;
	lstack->head = elem;
	lstack->count ++;
}

void *lstack_pop(lstack_t lstack)
{
	assert(lstack);
	assert(lstack->count >= 0);
	struct elem *node = lstack->head;
	assert(node != 0);
	void *item = node->obj;
	lstack->head = node->link;
	lstack->count --;
	free(node);
	return item;	
}

void lstack_free(lstack_t lstack)
{
	assert(lstack);
	while(lstack->head){
		struct elem *node = lstack->head;
		lstack->head = node->link;
		free(node);			
	}
	free(lstack);
}

