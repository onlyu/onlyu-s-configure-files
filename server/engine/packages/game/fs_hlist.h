#ifndef _OBJECT_H_
#define _OBJECT_H_


#include "hash.h"
#include <assert.h>

#define MAX_HLIST_NODE_NUM        (1024 * 1024)

enum {
	T_HT_KEY_NULL = 0,
	T_HT_KEY_INT,
	T_HT_KEY_ATOM,
};

typedef struct fs_ht_key_s {
	int type;
	int val;
} fs_ht_key_t;

// 节点类
typedef struct fs_ht_node_s{
	fs_ht_key_t hkey;
	void* ob;
	struct fs_ht_node_s *next;

	struct fs_ht_node_s *lnext;
	struct fs_ht_node_s **lprev;
}fs_ht_node_t;

typedef struct fs_obj_hash_s{
	fs_ht_node_t *head;
	fs_ht_node_t *tail;
	fs_ht_node_t ** node_hash;
	int hashsize;      // table hash size
	int num;
	int blru;
}fs_obj_hash_t;

// 创建一个hash表
fs_obj_hash_t *fs_ht_create_hash_table(int tblsize);

// 释放一个hash表
void fs_ht_free_hash_table(fs_obj_hash_t *obh);

// 将一个hash表打印出来
void fs_ht_dump_hash_table(fs_obj_hash_t *obh);

void *fs_ht_find_key(fs_obj_hash_t *obh, int kt, int kv);
int fs_ht_insert_key(fs_obj_hash_t *obh, int kt, int kv, void *ob);
void fs_ht_remove_key(fs_obj_hash_t *obh, int kt, int kv);
void fs_ht_remove_tail_node(fs_obj_hash_t *obh);
void fs_ht_set_lru_hlist(fs_obj_hash_t *obh);
void fs_dump_hlist_node();

#define HT_SET_KEY_INT(k, i) do {\
	(k)->type = T_HT_KEY_INT; \
	(k)->val = i; \
} while(0)

#define HT_SET_KEY_STR(k, i) do {\
	(k)->type = T_HT_KEY_ATOM; \
	(k)->val = SYS_GET_STR_ATOM(i); \
} while(0)

#define FS_HT_FIND_ATOM(obh, k) fs_ht_find_key(obh, T_HLIST_KEY_ATOM, k)
#define FS_HT_INSERT_ATOM(obh, k, ob) fs_ht_insert_key(obh, T_HLIST_KEY_ATOM, k, ob)
#define FS_HT_REMOVE_ATOM(obh, k) fs_ht_remove_key(obh, T_HLIST_KEY_ATOM, k)

#define FS_HT_FIND_INT(obh, k) fs_ht_find_key(obh, T_HT_KEY_INT, k)
#define FS_HT_INSERT_INT(obh, k, ob) fs_ht_insert_key(obh, T_HT_KEY_INT, k, ob)
#define FS_HT_REMOVE_INT(obh, k) fs_ht_remove_key(obh, T_HT_KEY_INT, k)

#define FS_HT_FIND(obh, k) fs_ht_find_key(obh, k->type, k->val)
#define FS_HT_INSERT(obh, k, ob) fs_ht_insert_key(obh, k->type, k->val, ob)
#define FS_HT_REMOVE(obh, k) fs_ht_remove_key(obh, k->type, k->val)

#define FS_HT_COUNT(obh) (obh->num)

#define FOR_EACH_HTABLE_OB(obh, ob) \
	if ( obh && obh->head) \
	for ( ob = obh->head; ob; ob = ob->lnext)

#endif

