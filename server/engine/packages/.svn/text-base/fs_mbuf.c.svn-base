
#include "fs_mbuf.h"

#include <event.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MIN_BLK_SIZE 4 
#define BLK_FACTOR 4 

#define BLK_SIZE  (1024*2)
//#define BLK_SIZE  (1)

inline static void blk_buf_init(fs_mbuf_blk_t *blk)
{
	blk->head = blk->tail = blk->buf;
}

fs_mbuf_blk_t *fs_mbuf_add_blk(fs_mbuf_t *mbuf, unsigned size)
{
	fs_mbuf_blk_t *blk = calloc(1, sizeof(fs_mbuf_blk_t));

	size = mbuf->hint_size > size ? mbuf->hint_size : size;
	size = BLK_FACTOR * ((size + BLK_FACTOR - 1) / BLK_FACTOR);
	if (size < MIN_BLK_SIZE) {
		size = MIN_BLK_SIZE;
	}

	blk->id = mbuf->blk_count;	
	blk->mbuf = mbuf;
	blk->buf = (char*)malloc(size);
	blk->size = size;
	blk->end = blk->buf + blk->size;
	blk_buf_init(blk);


	//STAILQ_INSERT_TAIL(&mbuf->blk_list, blk, next);
	blk->next = NULL;
	if (mbuf->tail == NULL) {
		mbuf->head = mbuf->tail = blk;
	} else {
		mbuf->tail->next = blk;
		mbuf->tail = blk;
	}
	
	mbuf->blk_count++;
	mbuf->blk_enq = blk;
	mbuf->alloc_size += blk->size;

	//printf("add blk.cnt:%d,id:%d,size:%d,total data:%d\n", mbuf->blk_count, blk->id, blk->size, mbuf->data_size);
	return blk;
}

void fs_mbuf_init(fs_mbuf_t *mbuf, unsigned blk_size)
{
	mbuf->hint_size = blk_size == 0 ? BLK_SIZE : blk_size;
	mbuf->data_size = 0;
	mbuf->blk_enq = NULL;
	mbuf->blk_deq = NULL;
	mbuf->blk_count = 0;
	mbuf->alloc_size = 0;
	//STAILQ_INIT(&mbuf->blk_list);
	mbuf->head = NULL;
	mbuf->tail = NULL;
	mbuf->blk_deq = fs_mbuf_add_blk(mbuf, mbuf->hint_size);
}

void fs_mbuf_destruct(fs_mbuf_t *mbuf)
{
	fs_mbuf_blk_t *blk, *tmp;

	for (blk = mbuf->head; blk && (tmp = blk->next, 1); blk = tmp) {
		mbuf->blk_count--;
		free(blk->buf);
		free(blk);
	}

	/*
	STAILQ_FOREACH_SAFE(blk, &mbuf->blk_list, next, tmp) {
		mbuf->blk_count--;
		free(blk);
	}
	*/

	//free(mbuf);
}

void fs_mbuf_reset(fs_mbuf_t *mbuf)
{
	fs_mbuf_blk_t *blk, *tmp;

	if (mbuf->blk_count > 1) {
		for (blk = mbuf->head; blk && (tmp = blk->next, 1); blk = tmp) {
			free(blk->buf);
			free(blk);
		}
		//printf("mbuf merge alloc_size:%d,blk:%d\n", mbuf->alloc_size, mbuf->blk_count);
		fs_mbuf_init(mbuf, mbuf->alloc_size);
	} else {
		assert(mbuf->blk_count == 1);
		blk_buf_init(mbuf->head);
		mbuf->data_size = 0;
		//printf("mbuf reset alloc_size:%d,blk:%d\n", mbuf->alloc_size, mbuf->blk_count);
	}
	assert(mbuf->head == mbuf->tail && mbuf->head == mbuf->blk_enq && mbuf->head == mbuf->blk_deq);
}


fs_net_mbuf_t *fs_net_mbuf_new(unsigned head_size)
{
	fs_net_mbuf_t *netbuf = calloc(1, sizeof(fs_net_mbuf_t));
	fs_net_mbuf_init(netbuf, head_size);
	return netbuf;
}

void fs_net_mbuf_free(fs_net_mbuf_t *netbuf)
{
	fs_mbuf_destruct(&netbuf->mbuf);
	free(netbuf);
}

void fs_net_mbuf_init(fs_net_mbuf_t *netbuf, unsigned head_size)
{
	netbuf->head_size = head_size;
	fs_mbuf_init(&netbuf->mbuf, 0);
	netbuf->head = FS_MBUF_ALLOC(&netbuf->mbuf, netbuf->head_size);
}

void fs_net_mbuf_reset(fs_net_mbuf_t *netbuf)
{
	fs_mbuf_reset(&netbuf->mbuf);
	netbuf->head = FS_MBUF_ALLOC(&netbuf->mbuf, netbuf->head_size);
}


void fs_mbuf_enq_span(fs_mbuf_t *mbuf, void *data, unsigned len)
{
	fs_mbuf_blk_t *blk = mbuf->blk_enq;
	unsigned capacity = FS_MBUF_BLK_CAP(blk);

	if (capacity < len) {
		memcpy(blk->tail, data, capacity);
		FS_MBUF_ADVANCE(mbuf, blk, capacity);

		len -= capacity;
		data += capacity;
		blk = fs_mbuf_add_blk(mbuf, len);
	}
	memcpy(blk->tail, data, len);
	FS_MBUF_ADVANCE(mbuf, blk, len);
}



void *fs_mbuf_enq(fs_mbuf_t *mbuf, void *data, unsigned len)
{
	void *p = FS_MBUF_ALLOC(mbuf, len);
	if (data)
		memcpy(p, data, len);
	return p;
}

unsigned fs_mbuf_deq(fs_mbuf_t *mbuf, void *ret, unsigned len)
{
	fs_mbuf_blk_t *blk = mbuf->blk_deq;
	unsigned slen = len;

	do {
		unsigned payload = blk->tail - blk->head;
		unsigned min = payload < len ? payload : len;
		if (min > 0) {
			if (ret != NULL)
				memcpy(ret, blk->head, payload);
			blk->head += min;
			mbuf->data_size -= min;
			len -= min;
		}

	//} while (len > 0 && NULL != (blk = mbuf->blk_deq = STAILQ_NEXT(blk, next)));
	} while (len > 0 && NULL != (blk = mbuf->blk_deq = blk->next));

	return slen - len;
}



#define ELEMENT_ALIGN  4
#define MIN_ELEMENT_CNT 1 
#define MAX_ELEMENT_CNT	1024 
#define MAX_PAGE_SIZE (1024 * 16)

static void free_array_blk_init(fs_free_array_t *farr, fs_mbuf_blk_t *blk)
{
	char *element, *end;
	fs_free_array_entry_t *entry;

	for (element = blk->buf, end = element + farr->element_size ; end <= blk->end; ) {
		entry = (fs_free_array_entry_t*)element;
		entry->next = farr->free;
		farr->free = entry;
		farr->element_total++;

		element = end;
		end = element + farr->element_size;
	}
}

void fs_free_array_init(fs_free_array_t *farr, char *name, unsigned elsize, unsigned elcnt_page)
{
	unsigned page_size;

	if (elsize < sizeof(fs_free_array_entry_t))
		elsize = sizeof(fs_free_array_entry_t);
	elsize = ELEMENT_ALIGN * ((elsize + ELEMENT_ALIGN - 1) / ELEMENT_ALIGN);

	if (elcnt_page < MIN_ELEMENT_CNT) 
		elcnt_page = MIN_ELEMENT_CNT;
	else if (elcnt_page > MAX_ELEMENT_CNT)
		elcnt_page = MAX_ELEMENT_CNT;

	page_size = elcnt_page * elsize;
	/*
	if (page_size > MAX_PAGE_SIZE) 
		page_size = MAX_PAGE_SIZE; 
	*/

	farr->name = strdup(name);
	farr->element_size = elsize;
	farr->element_total = 0;
	farr->element_alloc = 0;
	farr->free = NULL;

	fs_mbuf_init(&farr->mbuf, page_size);
	free_array_blk_init(farr, farr->mbuf.blk_enq);
}

void fs_free_array_destruct(fs_free_array_t *farr)
{
	free(farr->name);
	fs_mbuf_destruct(&farr->mbuf);
}

void *fs_free_array_alloc(fs_free_array_t *farr)
{
	 void *ret;

	if (farr->free == NULL) {
		fs_mbuf_add_blk(&farr->mbuf, farr->mbuf.hint_size);
		free_array_blk_init(farr, farr->mbuf.blk_enq);
	}

	ret = farr->free;
	farr->free = farr->free->next;
	farr->element_alloc++;

	memset(ret, 0, farr->element_size);
	//fprintf(stderr, "mem pool %s calloc %p\n", farr->name, ret);
	return ret;
}

void fs_free_array_free(fs_free_array_t *farr, void *element)
{
	fs_free_array_entry_t *entry = element;
	entry->next = farr->free;
	farr->free = entry;
	farr->element_alloc--;
	//fprintf(stderr, "mem pool %s free %p\n", farr->name, element);
}

