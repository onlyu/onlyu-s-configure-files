// 脏数据管理头文件 
// ----------------------------------
// Create By Prophet 2010/05/14
// ----------------------------------

#ifndef __DIRTY_H__
#define __DIRTY_H__

#include "lpc.h"

#include <sys/queue.h>

struct array_s;
struct mapping_s;

#define DIRTY_SET (0)
#define DIRTY_ADD (1)
#define DIRTY_DEL (2)

typedef struct dirty_key_s
{
	TAILQ_ENTRY(dirty_key_s) entry;
	union {
		svalue_t *map_key;
		int arr_index;
		svalue_t del;
	} key;
	unsigned char dirty_op;
} dirty_key_t;

typedef union self_key_u {
	svalue_t *map_key;
	int arr_index;
} self_key_t;

struct dirty_manage_s;

//管理dirty key
typedef struct dirty_node_s
{
	TAILQ_ENTRY(dirty_node_s) entry;
	TAILQ_HEAD(dirty_key_head_s, dirty_key_s) dirty_key_list;
	unsigned key_cnt;
	struct dirty_manage_s *mng;
} dirty_node_t;


//dirty树的根节点管理
typedef struct dirty_root_s
{
	TAILQ_HEAD(dirty_node_head_s, dirty_node_s) dirty_node_list;
	unsigned node_cnt;
} dirty_root_t;

//dirty的管理
typedef struct dirty_manage_s
{
	svalue_t *root;
	svalue_t *parent;
	svalue_t *self;
	self_key_t self_key;
	dirty_node_t *dirty_node;
	dirty_root_t dirty_root[0]; //根才有这个节点
} dirty_manage_t;

#define get_manage(sv) ( (sv)->type == T_MAPPING ? (sv)->u.map->dirty_mng : \
			((sv)->type == T_ARRAY ? (sv)->u.arr->dirty_mng : NULL))

#define IS_DIRTY_ROOT(mng) ((mng)->self == (mng)->root && (mng)->root != NULL)

void begin_dirty_manage_map(svalue_t *svmap, svalue_t *parent, self_key_t *self_key);
void begin_dirty_manage_array(svalue_t *svarr, svalue_t *parent, self_key_t *self_key);
void free_dirty_arr(struct array_s *arr);
void free_dirty_map(struct mapping_s *map);
void set_dirty_map(struct mapping_s *map, svalue_t *key, unsigned char op);
void set_dirty_arr(struct array_s *arr, int idx, unsigned char op);
void clear_dirty(svalue_t *value);

void dirty_mem_pool_setup();
void dirty_mem_pool_stat();
#endif // __DIRTY_H__

