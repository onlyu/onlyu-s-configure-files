#include <stdlib.h>
#include <string.h>
#include "list.h"

int _add_int(LIST_I *listp, int num)
{
	int tail;
	if (listp->_used >= listp->_max)
	{
		return 0;
	}

	tail = listp->_tail;
	listp->_store[tail] = num;
	listp->_tail = (tail + 1) % listp->_max;
	listp->_used++;

	return 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int _get_int(LIST_I *listp, int *num)
{
	int head, ret;

	if (listp->_used <= 0)
	{
		return 0;
	}

	head = listp->_head;
	ret = listp->_store[head];
	listp->_head = (head + 1) % listp->_max;
	listp->_used--;

	*num = ret;
	return 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int init_list_int(LIST_I **listpp, int max_list)
{
	LIST_I *listp = NULL;
	int *storep = NULL;

	listp = malloc(sizeof(LIST_I) );
	if (!listp)
	{
		return 0;
	}
	bzero(listp, sizeof(LIST_I) );

	storep = malloc(max_list *sizeof(int) );
	if (!storep)
	{
		return 0;
	}

	listp->_store = storep;
	listp->add = _add_int;
	listp->get = _get_int;
	listp->_max = max_list;

	*listpp = listp;
	return 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void free_list_int(LIST_I *listp)
{
	free(listp);
}
