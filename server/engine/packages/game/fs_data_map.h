#ifndef  _FS_DATA_MAP_H
#define  _FS_DATA_MAP_H

#include "lpc_incl.h"
#include "fs_hlist.h"
#include "stack.h"

enum {
	T_VALUE_NULL = 0,
	T_VALUE_INT,
	T_VALUE_STR,
	T_VALUE_DMAP
};

typedef int fs_bucket_node_t;

typedef struct fs_bnode_value_s {
	// value
	unsigned char type;
	union {
		int num;
		char *str;
		fs_bucket_node_t bn;
	}u ;

} fs_bnode_value_t;

typedef int (*fs_bucket_hook_func_t)(fs_ht_key_t *, fs_bnode_value_t *, void *); 

typedef struct fs_bucket_s {
	fs_obj_hash_t *ohash;
	unsigned char dirty;
} fs_bucket_t;

typedef struct fs_bucket_manager_s {
	fs_bucket_t **all_buckets;         /*所有分配的bucket*/
	ST_I *idle_stack;                       /*空闲栈*/
	int idle_max;                           /*空闲的最大数*/
} fs_bucket_manager_t;

void i_set_bucket_value(fs_bucket_node_t, fs_ht_key_t *, int);
void c_set_bucket_value(fs_bucket_node_t, fs_ht_key_t *, char *);
fs_bucket_node_t append_bucket_node(fs_bucket_node_t, fs_ht_key_t *);
fs_bnode_value_t *get_bucket_value(fs_bucket_node_t, fs_ht_key_t *);
int get_bucket_svalue(fs_bucket_node_t, svalue_t *, svalue_t *);
int set_bucket_svalue(fs_bucket_node_t, svalue_t *, svalue_t *);
fs_bucket_node_t fs_new_bucket(int);
void fs_free_bucket(fs_bucket_node_t);
void remove_bucket_key(fs_bucket_node_t, fs_ht_key_t *);
int bucket_serial(fs_bucket_node_t, char *, int);
int bucket_unserial(fs_bucket_node_t, char *);
void foreach_bucket_node(fs_bucket_node_t, fs_bucket_hook_func_t, void *);
void set_lru_bucket(fs_bucket_node_t);
int bucket_is_dirty(fs_bucket_node_t);
void clear_bucket_dirty(fs_bucket_node_t node);
void fs_sys_init_bucket_manager();
void fs_sys_release_bucket_manager();

#define BM_ASSERT_TYPE_VALUE(v, t, e, r)  ((!v || v->type != t) ? e : r)

#define BM_IS_TYPE_VALUE(v, t) (v && v->type == t)

#define BM_INT_VALUE(x) (x->u.num)
#define BM_STR_VALUE(x) (x->u.str)
#define BM_DMAP_VALUE(x) (x->u.bn)

#endif
