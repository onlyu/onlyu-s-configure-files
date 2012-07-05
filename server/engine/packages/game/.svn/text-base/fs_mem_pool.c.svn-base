// ==========================================
// Purpose  : 内存池管理
// Created  : 2010-04-30
// By       : Mouse
// History  : 2010-04-30  Mouse create
//            2010-05-25  增加内存块可扩展功能
// ==========================================
//
// 注意: 链表用整形数组连接，所以结点数量也就不能超过整形int能表达的数量
//		 2 ** 31 = 2147483648, 足够了!
// 

#include <stdlib.h>
#include <stdio.h>
#include "fs_mem_pool.h"
#include "fs_log.h"

#define INC_BASE_NUM         3              /*表示当内存不够时，每次分配原来的1/3数量*/

static int malloc_memory_size = 0;

inline int _fs_free_node_from_block(fs_memory_block_t *mbp, void *node)
{
	int ent;

	if (!mbp) return 0;

	if (!mbp->buff) return 0;

	ent = (node - mbp->buff) / mbp->ent_size;

	if (ent < 0 || ent > mbp->ent_num) {
		// 不属于本块的指针
		log_error(MOD_MEM_POOL, "free error!");
		return 0;
	}


	if (mbp->nodes[ent] != M_ALLOCATED) {
		log_error(MOD_MEM_POOL, "not allocated!");
		return 1;
	}

	mbp->nodes[mbp->tail] = ent;   
	mbp->nodes[ent] = M_EOM;
	mbp->tail = ent;
	mbp->used_num --;
	return 1;
}

inline void *_fs_malloc_node_from_block(fs_memory_block_t *mbp)
{
	int offset;
	int ent;

	if (!mbp) return NULL;

	// 循环链表碰头
	if (mbp->head == mbp->tail) {
		return NULL;
	}

	ent = mbp->head;

	mbp->head = mbp->nodes[ent];
	mbp->nodes[ent] = M_ALLOCATED;
	mbp->used_num ++;

	offset = mbp->ent_size * ent;

	return mbp->buff + offset;
}

inline fs_memory_block_t *fs_create_memory_block(int ent_size, int ent_num)
{
	int i;
	fs_memory_block_t *mbp;

	mbp = (fs_memory_block_t *) malloc(sizeof(struct fs_memory_block_s));

	mbp->ent_size = ent_size;
	mbp->ent_num = ent_num;
	mbp->used_num = 0;

	// ent_num+1: 多分配一个空间,使得末指针永远指向可使用的内存
	// 当head和tail指向同一块空间时,说明其他的ent_num个空间已分配完.
	mbp->buff = (char *) malloc(ent_size * (ent_num + 1));

	if (!mbp->buff) {
		return NULL;
	}

	// 最后一个为空
	mbp->nodes = malloc(sizeof(int) * (ent_num + 1));

	if (!mbp->nodes) {
		return NULL;
	}

	for (i = 0; i <= ent_num; i++ ) {
		mbp->nodes[i] = i+1;
	}
	mbp->nodes[ent_num] = M_EOM; 
	mbp->head = 0;
	mbp->tail = ent_num;
	mbp->blk_size = ent_size * (ent_num + 1) + sizeof(int) * (ent_num + 1);

	malloc_memory_size += mbp->blk_size;

	// log_info(MOD_MEM_POOL, "create memory block: size:%d, ent:%d, total: %d", ent_size, ent_num, malloc_memory_size);
	return mbp;
}


fs_memory_pool_t *fs_create_memory_pool(int ent_size, int ent_num)
{
	fs_memory_pool_t *mmp;
	fs_memory_block_t *mbp;

	mmp = (fs_memory_pool_t *) malloc(sizeof(struct fs_memory_pool_s));
	mbp = fs_create_memory_block(ent_size, ent_num);
	
	mbp->next = NULL;

	mmp->ent_num = ent_num;
	mmp->ent_size = ent_size;
	mmp->head = mbp;
	mmp->tail = mbp;
	mmp->pool_size = mbp->blk_size;

	// log_info(MOD_MEM_POOL, "create memory pool: size:%d, ent:%d, real:", ent_size, ent_num);
	return mmp;
}

void* fs_malloc_node(fs_memory_pool_t *mmp)
{
	fs_memory_block_t *mbp;
	void *node;

	mbp = mmp->head;
	while (mbp) {
		if ( (node = _fs_malloc_node_from_block(mbp)) ) {
			return node;
		}
		mbp = mbp->next;
	}


	// 遍历无果，增加新块
	mbp = fs_create_memory_block(mmp->ent_size, mmp->ent_num / 3 + 1);

	if (!mbp) {
		fprintf(stderr, "addr: 0x%ld, can't malloc memory block\n", (long) &mmp);
		return NULL;
	}


	mbp->next = NULL;
	mmp->tail->next = mbp;
	mmp->tail = mbp;
	mmp->pool_size += mbp->blk_size;

	node = _fs_malloc_node_from_block(mbp);

	// fprintf(stderr, "malloc addr: 0x%ld, 0x%ld\n", (long) &mmp, (long) &node);

	if (!node) {
		// TODO, warning
		fprintf(stderr, "malloc error!!!!!!!!\n");
	}
	return node;
}

int fs_free_node(fs_memory_pool_t *mmp, void *node)
{
	fs_memory_block_t *mbp;

	mbp = mmp->head;
	while (mbp) {
		if ( _fs_free_node_from_block(mbp, node) ) {
			return 1;
		}
		mbp = mbp->next;
	}
	return 0;
}


void fs_dump_mem_pool_desc(fs_memory_pool_t *mmp, char *desc)
{
	fs_memory_block_t *mbp;

	mbp = mmp->head;

	printf( "memory pool(%s): \n", desc );

	while (mbp) {
		printf( "----ent_size: %d, ent_num: %d, used_num: %d, size: %d\n", mbp->ent_size, mbp->ent_num, mbp->used_num, mbp->blk_size);
		mbp = mbp->next;
	}
	printf("----size : %d\n", mmp->pool_size);
	printf("----total : %d\n", malloc_memory_size);
	fflush(stdout);
}


// ==========================================
// test

/*
typedef struct fs_mm_test_s {
	int x, y, z;
} fs_mm_test_t;

#define TEST_MALLOC_NUM  16000

#define TEST_NUM 128

int main()
{
	fs_memory_pool_t *mmp;
	fs_mm_test_t *mt[TEST_MALLOC_NUM];
	int i;

	mmp = fs_create_memory_pool(sizeof(fs_mm_test_t), TEST_NUM);

	fs_dump_mem_pool_desc(mmp, "create");
	
	i = 0;
	while ( i < TEST_MALLOC_NUM ) {
		mt[i] = fs_malloc_node(mmp);
		i++;
	}

	fs_dump_mem_pool_desc(mmp, "malloc");

	i = 0;
	while ( i < TEST_MALLOC_NUM ) {
		fs_free_node(mmp, mt[i]);
		i++;
	}

	fs_dump_mem_pool_desc(mmp, "free");

	return 0;
}
*/
