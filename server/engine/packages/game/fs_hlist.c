// ==========================================
// Purpose  : 哈希链表管理
// Created  : 2010-04-30
// By       : Mouse
// History  : 2010-04-30  Mouse create
// ==========================================
//
// hashlist的管理:
// 1.通过id,能够很快的找到对应的对象
// 2.双向链表遍历
// 3.需要改善地方: 
// 	1) hash表象采用结点,浪费空间
// 	2) hash表的hash方法,字串和int是两种不同的hash算法
// ==========================================

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include "fs_mem_pool.h"
#include "fs_hlist.h"
#include "hash.h"

extern void debug_message (char *, ...);

static int inited = 0;
static fs_memory_pool_t *gpHlistNodePool;					/*场景描述表*/

// 修改32位时需要更改hash算法
#define POINTER_INT int64_t

#define COMBINE_INT(kt, kv) (((POINTER_INT)kt << 32) | (POINTER_INT)(kv))
// int
#define MAP_POINTER_HASH(x) (((POINTER_INT)(x) >> 4) ^ ((POINTER_INT)(x) & 0xFFFFFFF))

#define KEYHASH(kt, kv, m) (MAP_POINTER_HASH(COMBINE_INT(kt,kv)) & (m->hashsize -1))

#define _equal_hkey(k, t, v) (k->type== t && k->val == v)

inline static void fs_init_hlist_pool()
{
	gpHlistNodePool = fs_create_memory_pool(sizeof(fs_ht_node_t), MAX_HLIST_NODE_NUM);
	inited = 1;
}

inline static fs_ht_node_t *NEW_HT_NODE()
{
	if (!inited)  {
		fs_init_hlist_pool();
	}
	return fs_malloc_node(gpHlistNodePool);
}

inline static void FREE_HT_NODE(fs_ht_node_t *node)
{
	fs_free_node(gpHlistNodePool, node);
}

void fs_dump_hlist_node()
{
	fs_dump_mem_pool_desc(gpHlistNodePool, "hash table node");
}

inline static void _insert_list_node(fs_obj_hash_t *obh, fs_ht_node_t *node)
{
	// insert to list
	if (!obh->head) {
		node->lprev = &(obh->head);

		node->lnext = NULL;
		obh->head = node;
		obh->tail = node;

	} else {
		node->lnext = obh->head;
		node->lprev = &(obh->head);

		obh->head->lprev = &(node->lnext);
		obh->head = node;
	}
}

inline static void _remove_list_node(fs_obj_hash_t *obh, fs_ht_node_t *node)
{
	// 如果非结尾点, 即lnext不为空
	if (obh->tail != node) {
		*(node->lprev) = node->lnext;
		node->lnext->lprev = node->lprev;
	} else {
		*(node->lprev) = NULL;
		// 如果是头节点
		if (node->lprev == &(obh->head)) {
			obh->tail = NULL;
		} else {
			obh->tail = (fs_ht_node_t *)((void *)(node->lprev) - (void *)(&((fs_ht_node_t *)0)->lnext));
		}
	}
}


// 根据key找到对象
inline fs_ht_node_t * fs_ht_find_node(fs_obj_hash_t *obh, int h, int kt, int kv)
{
	fs_ht_node_t* p = obh->node_hash[h];
	fs_ht_key_t *pk;

	while (p!=NULL) {
		pk = &p->hkey;
		if (_equal_hkey(pk, kt, kv)) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}


// 插入对象ob
inline static int fs_ht_insert_node(fs_obj_hash_t *obh, int h, int kt, int kv, void *ob)
{
	if ( !obh || fs_ht_find_node(obh, h, kt, kv) != NULL ) {
		// TODO: warning
		return 0;
	}

	fs_ht_node_t* p = NEW_HT_NODE();
	assert(p!=NULL);
	p->hkey.type = kt;
	p->hkey.val = kv;
	p->next = obh->node_hash[h] ;

	p->ob = ob ;
	obh->node_hash[h] = p ;
	obh->num ++;

	_insert_list_node(obh, p);

	return 1;
}

// 删除对象节点
inline static void fs_ht_remove_node(fs_obj_hash_t *obh, int h, int kt, int kv)
{
	fs_ht_node_t *pre, *cur;
	fs_ht_key_t *pk;

	cur = obh->node_hash[h];
	pre = cur ;
	while(cur!=NULL){
		pk = &cur->hkey;
		if (_equal_hkey(pk, kt, kv)) {
			if ( cur == obh->node_hash[h] ) {
				obh->node_hash[h] = cur->next ;
			}else {
				pre->next = cur->next ;
			}

			obh->num--;
			_remove_list_node(obh, cur);
			FREE_HT_NODE(cur);
			break ;
		}else {
			pre = cur ;
			cur = cur->next;
		}
	}
}

// 创建一个hash表
fs_obj_hash_t *fs_ht_create_hash_table(int tblsize)
{
	int x, obj_hash_size;
	fs_obj_hash_t *obh;

	/* ensure that otable_size is a power of 2 */
	obj_hash_size = 1 ;
	for (; obj_hash_size < tblsize; obj_hash_size *= 2 ) ;

	obh = (fs_obj_hash_t *) malloc(sizeof(fs_obj_hash_t));
	obh->hashsize = obj_hash_size;
	obh->node_hash = (fs_ht_node_t**) malloc(obj_hash_size * sizeof(fs_ht_node_t *));
	if (!obh->node_hash) {
		debug_message( "\nerror ----------------, ht can't malloc");
	}

	obh->head = NULL;
	obh->tail = NULL;
	obh->blru = 0;
	obh->num = 0;
	for (x = 0; x < obj_hash_size; x++) {
		obh->node_hash[x] = NULL;
	}

	return obh;
}

// 设置hlist为一个最近使用的链表, 即每访问1个元素，则将这个元素置于链表头
// 最后的结果就会是较少使用的置于链表尾
void fs_ht_set_lru_hlist(fs_obj_hash_t *obh)
{
	obh->blru = 1;
}

// 释放一个hash表
void fs_ht_free_hash_table(fs_obj_hash_t *obh)
{
	fs_ht_node_t* p ;
	fs_ht_node_t* cur;
	int i = 0 ;
	for (i=0; i<obh->hashsize; ++i){
		p = obh->node_hash[i] ;
		while (p!=NULL){
			cur = p ;
			p = p->next; 
			FREE_HT_NODE(cur);
		}
	}
	free(obh);
	free(obh->node_hash);
}

// 将一个hash表打印出来
void fs_ht_dump_hash_table(fs_obj_hash_t *obh)
{
	fs_ht_node_t* p ;
	fs_ht_node_t* cur;
	int i = 0 ;
	debug_message("\n ========================\n");
	debug_message("\n hash tbl: %d\n", obh->hashsize);
	for (i=0; i<obh->hashsize; ++i){
		p = obh->node_hash[i] ;
		if (p) debug_message("\n---%d :", i);
		while (p!=NULL){
			cur = p ;
			p = p->next; 
			debug_message( "\n------(%d,%d)", cur->hkey.type, cur->hkey.val);
		}
	}
	debug_message("\n ========================\n");
}

void *fs_ht_find_key(fs_obj_hash_t *obh, int kt, int kv)
{
	fs_ht_node_t *n = fs_ht_find_node(obh, KEYHASH(kt, kv, obh), kt, kv);

	if (n) {
		if (obh->blru) {
			_remove_list_node(obh, n);
			_insert_list_node(obh, n);
		}
		return n->ob;
	}

	return NULL;
}

int fs_ht_insert_key(fs_obj_hash_t *obh, int kt, int kv, void *ob)
{
	return fs_ht_insert_node(obh, KEYHASH(kt, kv, obh), kt, kv, ob);
}

void fs_ht_remove_key(fs_obj_hash_t *obh, int kt, int kv)
{
	return fs_ht_remove_node(obh, KEYHASH(kt, kv, obh), kt, kv);
}

// ===================================================
// 以下方法是用于类lru最近访问的hash优化
//
// 1. 当hash表数量过大时,可以移除hash链表中最后的元素
// 2. 1的前提是保证每次访问hash表之后, 将节点移动到头部

void *fs_ht_tail_node_ob(fs_obj_hash_t *obh)
{
	if (obh->tail) {
		return obh->tail->ob;
	}
	return NULL;
}

// 移除最后的一个元素
void fs_ht_remove_tail_node(fs_obj_hash_t *obh)
{
	int kt, kv;
	if (obh->tail) {
		kt = obh->tail->hkey.type;
		kv = obh->tail->hkey.val;
		fs_ht_remove_node(obh, KEYHASH(kt, kv, obh), kt, kv);
	}
}

// 将hash表里面的一个结点移动到头
// 所以不用动hash表
void fs_ht_node_move_2_head(fs_obj_hash_t *obh, fs_ht_node_t *node)
{

}
