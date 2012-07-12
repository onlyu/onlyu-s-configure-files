#include <stdlib.h>
#include "lstack.h"

struct lstack_t {
	int count;
	struct elem{
		void *obj;
		struct elem *link;
	} *head;
};

inline struct elem *_new_node()
{
	return (struct elem *)malloc(sizeof(struct elem));
}

inline void _free_node(struct elem *node)
{
	assert(node);
	free(node);
}

lstack_t lstack_new(void)
{
	lstack_t lstack = (lstack_t)malloc(sizeof(lstack_t));
	lstack->count = 0;
	lstack->head = NULL;
	return lstack;
}

int lstack_is_empty(lstack_t lstack)
{
	assert(lstack);
	return lstack->count == 0;
}

int lstack_size(lstack_t lstack)
{
	assert(lstack);
	return lstack->count;
}

void lstack_push(lstack_t lstack, void *item)
{
	assert(lstack);
	struct elem *elem =	_new_node(); 
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
	assert(node != NULL);
	void *item = node->obj;
	lstack->head = node->link;
	lstack->count --;
	_free_node(node);
	return item;	
}

void lstack_free(lstack_t lstack)
{
	assert(lstack);
	while(lstack->head){
		struct elem *node = lstack->head;
		lstack->head = node->link;
		_free_node(node);			
	}
	free(lstack);
}

