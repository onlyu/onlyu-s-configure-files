#include <stdlib.h>
#include <string.h>
#include "stack.h"
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
int __push_string(ST_S *stackp, char *value)
{
	char *store_name = NULL;
	int len, index;

	if (stackp->index + 1 >= MAX_STACK - 1)
	{
		return 0;
	}

	len = strlen(value);
	store_name = malloc(len + 1);
	if (!store_name)
	{
		return 0;
	}

	stackp->index++;
	index = stackp->index;
	strlcpy(store_name, value, len + 1);
	stackp->store[index] = store_name;
	return 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
//不可重入
int __pop_string(ST_S *stackp, char **value)
{
	static char pop_string[POP_STR_LEN + 1];
	int index;

	index = stackp->index;
	if (index < 0)
	{
		return 0;
	}

	strlcpy(pop_string, stackp->store[index], POP_STR_LEN);
	free(stackp->store[index]);
	stackp->store[index] = NULL;
	stackp->index--;
	*value = pop_string;
	return 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int init_stack_string(ST_S **stackpp)
{
	ST_S *stackp = NULL;

	stackp = malloc(sizeof(ST_S) );
	if (!stackp)
	{
		return 0;
	}
	bzero(stackp, sizeof(ST_S) );

	stackp->push = __push_string;
	stackp->pop = __pop_string;
	stackp->index =  - 1;

	*stackpp = stackp;
	return 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void free_stack_string(ST_S *stackp)
{
	int i;

	for (i = 0; i <= stackp->index; i++)
	{
		free(stackp->store[i]);
	}
	free(stackp);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int __push_int(ST_I *stackp, int num)
{
	int index;

	if (stackp->index + 1 >= MAX_STACK - 1)
	{
		return 0;
	}

	stackp->index++;
	index = stackp->index;
	stackp->store[index] = num;
	return 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int __pop_int(ST_I *stackp, int *num)
{
	int index, ret;

	index = stackp->index;
	if (index < 0)
	{
		return 0;
	}

	ret = stackp->store[index];
	stackp->index--;
	*num = ret;
	return 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int init_stack_int(ST_I **stackpp)
{
	ST_I *stackp = NULL;

	stackp = malloc(sizeof(ST_I) );
	if (!stackp)
	{
		return 0;
	}
	bzero(stackp, sizeof(ST_I) );

	stackp->push = __push_int;
	stackp->pop = __pop_int;
	stackp->index =  - 1;

	*stackpp = stackp;
	return 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void free_stack_int(ST_I *stackp)
{
	free(stackp);
}
