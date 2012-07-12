#ifndef __MEM_POOL_H
#define __MEM_POOL_H

// 内存管理池 

#define M_ALLOCATED  -1
//end of memory
#define M_EOM        -2


typedef struct fs_memory_block_s {

	int head;        
	int tail;
	int *nodes;
	void * buff;
	int ent_num;               /*内存块中实体个数*/
	int ent_size;              /*内存块中实体大小*/
	int used_num;              /*已使用*/

	int blk_size;

	struct fs_memory_block_s *next;

}fs_memory_block_t;


typedef struct fs_memory_pool_s {

	fs_memory_block_t *head;
	fs_memory_block_t *tail;   

	int ent_size;              /*内存池中实体大小*/
	int ent_num;               /*内存块中实体个数*/

	int pool_size;
} fs_memory_pool_t;


void *fs_malloc_node(fs_memory_pool_t *mmp);
int fs_free_node(fs_memory_pool_t *mmp, void *node);
fs_memory_pool_t *fs_create_memory_pool(int ent_size, int ent_num);
void fs_dump_mem_pool_desc(fs_memory_pool_t *mmp, char *desc);

#endif

