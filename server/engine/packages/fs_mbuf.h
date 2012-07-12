
#ifndef _FS_MBUF_H_
#define _FS_MBUF_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//#include <sys/queue.h>

typedef struct fs_mbuf_s fs_mbuf_t;

typedef struct fs_mbuf_blk_s {
	//STAILQ_ENTRY(fs_mbuf_blk_s) next;
	struct fs_mbuf_blk_s *next;
	fs_mbuf_t *mbuf;
	unsigned id;
	unsigned size;
	char *buf;
	char *head;
	char *tail;
	char *end;
} fs_mbuf_blk_t;


//STAILQ_HEAD(fs_mbuf_blk_list_s, fs_mbuf_blk_s);
struct fs_mbuf_s {
	//struct fs_mbuf_blk_list_s blk_list;
	fs_mbuf_blk_t *head;
	fs_mbuf_blk_t *tail;
	
	fs_mbuf_blk_t *blk_deq;
	fs_mbuf_blk_t *blk_enq;
	int blk_count;
	unsigned data_size;
	unsigned hint_size;
	unsigned alloc_size;
};

void fs_mbuf_init(fs_mbuf_t *mbuf, unsigned blk_size);
void fs_mbuf_destruct(fs_mbuf_t *mbuf);
fs_mbuf_blk_t *fs_mbuf_add_blk(fs_mbuf_t *mbuf, unsigned size);
void *fs_mbuf_enq(fs_mbuf_t *mbuf, void *data, unsigned len);
void fs_mbuf_enq_span(fs_mbuf_t *mbuf, void *data, unsigned len);
unsigned fs_mbuf_deq(fs_mbuf_t *mbuf, void *ret, unsigned len);
void fs_mbuf_reset(fs_mbuf_t *mbuf);

struct bufferevent;

#define FS_MBUF_ADVANCE(mbuf, blk, len)  do {\
	(mbuf)->data_size += (len);	\
	(blk)->tail += (len);	\
} while (0)

#define FS_MBUF_BLK_CAP(blk) ((blk)->end - (blk)->tail)
#define FS_MBUF_BLK_DATA_LEN(blk) ((blk)->tail - (blk)->head)

inline static void *FS_MBUF_ALLOC(fs_mbuf_t *mbuf, unsigned len)  
{
	//printf("mbuf alloc try from id:%d,cap:%ld,need:%d,blks:%d\n", 
		//mbuf->blk_enq->id, FS_MBUF_BLK_CAP(mbuf->blk_enq), len, mbuf->blk_count);
	for (; FS_MBUF_BLK_CAP(mbuf->blk_enq) < len; ) {
		mbuf->blk_enq = mbuf->blk_enq->next;
		if (mbuf->blk_enq == NULL) {
			fs_mbuf_add_blk(mbuf, len);
			break;
		}
		assert(FS_MBUF_BLK_DATA_LEN(mbuf->blk_enq) == 0);
	}

	//printf("mbuf alloc from id:%d,cap:%ld,need:%d,blks:%d\n",
		//mbuf->blk_enq->id, FS_MBUF_BLK_CAP(mbuf->blk_enq), len, mbuf->blk_count);
	FS_MBUF_ADVANCE(mbuf, mbuf->blk_enq, len);
	return mbuf->blk_enq->tail - len;
}

#define FS_MBUF_ENQ(mbuf, data, len) do { \
	void *p = FS_MBUF_ALLOC(mbuf, len);\
	memcpy(p, data, len);\
} while(0)

#define FS_MBUF_ENQ_TYPE(mbuf, data, type) do { \
	type *p = FS_MBUF_ALLOC(mbuf, sizeof(type));\
	*p = *(data);\
} while(0)



typedef struct fs_inbuf_s {
	char *buf;
	unsigned off;
	unsigned size;
} fs_inbuf_t;

#define FS_INBUF_INIT(in, data, len) do {\
	(in)->buf = (data);\
	(in)->size = (len);\
	(in)->off = 0;	\
} while (0)

#define FS_INBUF_POS(in) ((void*)((in)->buf + (in)->off))
#define FS_INBUF_LEN(in) ((in)->size - (in)->off)
#define FS_INBUF_ADV(in, len) ((in)->off += (len))
#define FS_INBUF_CHECK(in, len)  do {if (FS_INBUF_LEN(in) < (len)) return -1; } while (0)

#define FS_INBUF_GET_NO_CHECK(in, retv, len) do { \
	memcpy((retv), FS_INBUF_POS((in)), (len));\
	FS_INBUF_ADV((in), (len));	\
} while(0)

#define FS_INBUF_GET_TYPE(in, retv, type) do {\
	FS_INBUF_CHECK((in), sizeof(type));\
	*(type*)(retv) = *(type*)FS_INBUF_POS((in));\
	FS_INBUF_ADV((in), sizeof(type)); \
} while(0)



typedef struct fs_free_array_entry_s {
	struct fs_free_array_entry_s *next;
} fs_free_array_entry_t;

typedef struct fs_free_array_s {
	fs_free_array_entry_t *free;
	char *name;
	unsigned element_size;
	unsigned element_alloc;
	unsigned element_total;
	fs_mbuf_t mbuf;
} fs_free_array_t;


void fs_free_array_init(fs_free_array_t *farr, char *name, unsigned elsize, unsigned elcnt_page);
void fs_free_array_destruct(fs_free_array_t *farr);
void *fs_free_array_alloc(fs_free_array_t *farr);
void fs_free_array_free(fs_free_array_t *farr, void *element);


typedef struct fs_net_mbuf_s {
	void *head;
	unsigned head_size;
	fs_mbuf_t mbuf;
} fs_net_mbuf_t;

fs_net_mbuf_t * fs_net_mbuf_new(unsigned head_size);
void fs_net_mbuf_free(fs_net_mbuf_t *netbuf);
void fs_net_mbuf_init(fs_net_mbuf_t *netbuf, unsigned head_size);
void fs_net_mbuf_reset(fs_net_mbuf_t *netbuf);
#define FS_NET_MBUF_PAYLOAD_LEN(netbuf) ((netbuf)->mbuf.data_size - (netbuf)->head_size)

#endif /*_FS_MBUF_H_ */

