// Purpose  : 数据映射表，类似map
// Created  : 2010-04-30
// By       : Mouse
// History  : 2010-04-30  Mouse create
//            2010-05-27  将数据映射表改成容器.
//
// 说明: 
//		1. 主要应用于Item, Summon等需要存储的模块管理中
//		2. key都是采用系统原子串id

#include "fs_data_map.h"
#include "fs_atom.h"
#include "fs_hlist.h"
#include "fs_lpc_op.h"
#include "fs_common.h"
#include "fs_log.h"

extern void debug_message (char *, ...);

#define INIT_IDLE_BUCKET_MAX          1024                     /*初始化bucket的数目*/
#define INC_IDLE_NUM                  1024                     /*每次增加的空闲bucket数目*/
#define DEFAULT_BUCKET_HSIZE          16                       /*默认新建出来的Bucket的哈希项, 2的n次*/
#define DEFAULT_SCRIPT_BUCKET_HSIZE   32                       /*默认脚本新建出来的Bucket的哈希项, 2的n次*/

#define NODE_NOT_VALID(n) (node > gpBucketManager->idle_max || node <= 0)

fs_bnode_value_t constNullValue;

fs_bucket_manager_t *gpBucketManager;
int bInited = 0;

inline void set_bucket_dirty(fs_bucket_node_t node);

static inline fs_bnode_value_t * NEW_DATA_VALUE()
{
	fs_bnode_value_t * v = (fs_bnode_value_t *) MALLOC(sizeof(fs_bnode_value_t));
	memset(v, 0, sizeof(fs_bnode_value_t));
	v->type = T_VALUE_NULL;
	return v;
}

static inline void FREE_DATA_VALUE(fs_bnode_value_t * v)
{
	FREE(v);
}

static inline fs_bucket_t * NEW_BUCKET()
{
	return (fs_bucket_t *) MALLOC(sizeof(fs_bucket_t));
}

static inline void  FREE_BUCKET(fs_bucket_t *bucket)
{
	FREE(bucket);
}

static inline fs_bucket_t *_trans_node_2_struct(fs_bucket_node_t node)
{
	if (NODE_NOT_VALID(node)) return NULL;

	return gpBucketManager->all_buckets[node - 1];
}

// 增加空闲的bucket
static inline void _malloc_idle_buckets(int num)
{
	int cur = gpBucketManager->idle_max;
	int max;

	if (!cur) {
		max = num;
		gpBucketManager->all_buckets = (fs_bucket_t **) MALLOC(sizeof(fs_bucket_t *) * max );
	} else {
		max = cur + num;
		gpBucketManager->all_buckets = (fs_bucket_t **) REALLOC(gpBucketManager->all_buckets, sizeof(fs_bucket_t *) * max);
	}

	while (cur < max) {
		gpBucketManager->all_buckets[cur] = NULL;

		cur++;
		gpBucketManager->idle_stack->push(gpBucketManager->idle_stack, cur);	
	}
	gpBucketManager->idle_max = max;
}

// 新建一个容器结点, 返回结点ID
static inline fs_bucket_node_t _new_bucket_node(int hsize)
{
	int ret, idle;
	fs_bucket_t *db;

	ret = gpBucketManager->idle_stack->pop(gpBucketManager->idle_stack, &idle);

	if (!ret) {
		debug_message( "FS_WARNING, bucket stack used out!" );
		_malloc_idle_buckets(INC_IDLE_NUM);
		ret = gpBucketManager->idle_stack->pop(gpBucketManager->idle_stack, &idle);
		if (!ret) {
			debug_message( "FS_ERROR, bucket stack can't malloc new" );
			return 0;
		}
	}

	if (_trans_node_2_struct(idle)) {
		debug_message("FS_ERROR, idle[%d] bucket isn't null!!", idle);
		return 0;
	}

	db = NEW_BUCKET();
	db->ohash = fs_ht_create_hash_table(hsize);
	db->dirty = 0;
	gpBucketManager->all_buckets[idle - 1] = db; 

	return idle;
}


static inline void _bucket_release_value(fs_bnode_value_t *data)
{
	fs_bucket_t *bucket;
	fs_ht_node_t *hnode;
	fs_bnode_value_t *vob;

	if (!data || data->type == T_VALUE_NULL || data->type == T_VALUE_INT) {
		return ;
	}
	switch (data->type)
	{
		case T_VALUE_STR:
			FREE(data->u.str);
			break;
		case T_VALUE_DMAP:
			bucket = _trans_node_2_struct(data->u.bn);
			if (bucket) {
				FOR_EACH_HTABLE_OB(bucket->ohash, hnode)
				{
					vob = (fs_bnode_value_t *) hnode->ob;
					_bucket_release_value(vob);
				}
			}
			fs_free_bucket(data->u.bn);
			break;
		default:
			break;
	}
}

static inline fs_bnode_value_t *_find_bucket_value(fs_bucket_node_t node, fs_ht_key_t *key, int doNew)
{
	fs_bnode_value_t *vob;

	fs_bucket_t *bucket = _trans_node_2_struct(node);

	if (!bucket) {
		return NULL;
	}

	vob = (fs_bnode_value_t *) FS_HT_FIND(bucket->ohash, key);
	if (!vob) {
		if (doNew) {
			vob = NEW_DATA_VALUE();
			FS_HT_INSERT(bucket->ohash, key, vob);
		} else {
			vob = NULL;
		}
	}
	return vob;
}

inline void set_bucket_dirty(fs_bucket_node_t node)
{
	fs_bucket_t *bucket;
	bucket = _trans_node_2_struct(node);
	if (bucket) bucket->dirty = 1;
}

void i_set_bucket_value(fs_bucket_node_t node , fs_ht_key_t *htkey, int v)
{
	fs_bnode_value_t *bv = _find_bucket_value(node, htkey, 1);

	if (!bv) return;

	_bucket_release_value(bv);

	set_bucket_dirty(node);

	bv->type = T_VALUE_INT;
	bv->u.num = v;
}

void c_set_bucket_value(fs_bucket_node_t node, fs_ht_key_t *htkey, char *v)
{
	int size;
	char *temp;
	fs_bnode_value_t *bv = _find_bucket_value(node, htkey, 1);

	if (!bv) return;

	_bucket_release_value(bv);

	set_bucket_dirty(node);

	bv->type = T_VALUE_STR;
	size = strlen(v) + 1;
	// TODO, 释放原来的
	temp = (char *) MALLOC(size);
	strncpy(temp, (char *)v, size);
	bv->u.str = temp;
}

// 如果key本来就是Bucket, 那么返回，否则创建
fs_bucket_node_t append_bucket_node(fs_bucket_node_t node, fs_ht_key_t *htkey)
{
	fs_bucket_node_t child;
	fs_bnode_value_t *bv = _find_bucket_value(node, htkey, 1);

	if (!bv) return 0;

	if (bv->type == T_VALUE_DMAP) {
		return bv->u.bn;
	}

	_bucket_release_value(bv);

	set_bucket_dirty(node);

	child = _new_bucket_node(DEFAULT_BUCKET_HSIZE);
	bv->type = T_VALUE_DMAP;
	bv->u.bn= child;
	return child;
}

fs_bnode_value_t *get_bucket_value(fs_bucket_node_t node, fs_ht_key_t *htkey)
{
	return _find_bucket_value(node, htkey, 0);
}

inline int parse_svalue_key(svalue_t *key, fs_ht_key_t *htkey)
{
	if (IS_LPC_STR(key)) {
		HT_SET_KEY_STR(htkey, LPC_STR_VALUE(key));
		return 1;
	} else if (IS_LPC_INT(key)) {
		HT_SET_KEY_INT(htkey, LPC_INT_VALUE(key));
		return 1;
	}
	return 0;
}


void clear_bucket_dirty(fs_bucket_node_t node)
{
	fs_bucket_t *bucket;
	bucket = _trans_node_2_struct(node);
	if (bucket) bucket->dirty = 0;
}

int bucket_is_dirty(fs_bucket_node_t node)
{
	fs_bucket_t *bucket;
	bucket = _trans_node_2_struct(node);
	return (bucket && bucket->dirty == 1);
}

void remove_bucket_key(fs_bucket_node_t node, fs_ht_key_t *htkey)
{
	fs_bucket_t *bucket;
	bucket = _trans_node_2_struct(node);

	if (bucket) {
		FS_HT_REMOVE(bucket->ohash, htkey);
	}
}


// 初始化bucket的管理
void fs_sys_init_bucket_manager()
{
	gpBucketManager = (fs_bucket_manager_t *)MALLOC(sizeof(fs_bucket_manager_t));
	gpBucketManager->all_buckets = NULL;
	gpBucketManager->idle_stack = NULL;
	gpBucketManager->idle_max = 0;

	if (init_stack_int(&gpBucketManager->idle_stack) != 1) {
		debug_message("bucket stack init error\n");
		exit(-1);
	}

	// init constNullValue
	constNullValue.type = T_VALUE_NULL;
	constNullValue.u.num = 0;

	_malloc_idle_buckets(INIT_IDLE_BUCKET_MAX);
}

void fs_sys_release_bucket_manager()
{
	int i;
	i = 0;
	while (i < gpBucketManager->idle_max) {
		fs_free_bucket(i+1);
		i++;
	}

	free_stack_int(gpBucketManager->idle_stack);
	FREE(gpBucketManager->all_buckets);
	FREE(gpBucketManager);
}

fs_bucket_node_t fs_new_bucket(int hsize)
{
	return _new_bucket_node(hsize);
}

void fs_free_bucket(fs_bucket_node_t node)
{
	fs_bucket_t *bucket;

	if (NODE_NOT_VALID(node)) return ;

	bucket = gpBucketManager->all_buckets[node - 1];

	if (bucket) {
		gpBucketManager->idle_stack->push(gpBucketManager->idle_stack, node);
		gpBucketManager->all_buckets[node - 1] = NULL;
		FREE_BUCKET(bucket);
	}
}
mapping_t * fs_bucket_2_map(fs_bucket_node_t node)
{
	fs_bucket_t *bucket;
	fs_bnode_value_t *vob;
	fs_ht_node_t *hnode;
	char *strkey;
	svalue_t *knode;
	mapping_t *map, *child_map;
	fs_ht_key_t *htkey;

	if (NODE_NOT_VALID(node)) {
		return NULL;
	}

	bucket = _trans_node_2_struct(node);

	if (bucket && bucket->ohash) {
		map = allocate_mapping(bucket->ohash->num);
		FOR_EACH_HTABLE_OB(bucket->ohash, hnode)
		{
			vob = (fs_bnode_value_t *) hnode->ob; 

			// TODO: 整形key和串型key的区分
			htkey = (fs_ht_key_t *)(&hnode->hkey);
			knode = NULL;

			switch (htkey->type) {
				case T_HT_KEY_INT: 
					knode = map_insert_int_key(map, htkey->val);
					break;
				case T_HT_KEY_ATOM:
					strkey = fs_atom_get_string_by_atom(htkey->val);
					if (strlen(strkey)) {
						knode = map_insert_str_key(map, strkey);
					}
					break;
				default:
					break;
			}
			if (!knode) {
				// warning  插入map失败
				continue;
			}

			switch(vob->type) {
				case T_VALUE_INT:
					set_svalue_number(knode, BM_INT_VALUE(vob));
					break;
				case T_VALUE_STR:
					set_svalue_shared_string(knode, BM_STR_VALUE(vob));
					break;
				case T_VALUE_DMAP:
					child_map = fs_bucket_2_map(BM_DMAP_VALUE(vob));
					if (child_map) {
						set_svalue_mapping(knode, child_map);
					}
					break;
				default:
					break;
			}
		}
		return map;
	}
	return NULL;
}

int bucket_serial(fs_bucket_node_t node, char *buf, int len)
{
	int size;
	svalue_t v;

	mapping_t *m = fs_bucket_2_map(node);

	if (!m) return 0;

	TO_LPC_MAP(v, m);
	size = svalue_save_size(&v) + 1;

	if (size >= len) {
		free_mapping(m);
		return 0;
	}

	char *save_variable_2(svalue_t *var, char *ret);

	save_variable_2(&v, buf); 

	free_mapping(m);
	return 1;
}

void fs_map_2_bucket(fs_bucket_node_t node, mapping_t *m)
{
	mapping_node_t *elt,  *nelt;
	svalue_t *key, *value;
	int childnode;
	fs_ht_key_t htkey;

	int j = (int)m->table_size;
	do
	{
		for (elt = m->table[j]; elt; elt = nelt)
		{
			nelt = elt->next;
			key = &elt->values[0];
			value = &elt->values[1];

			if (!parse_svalue_key(key, &htkey)) {
				// warning
				continue;
			}

			switch (value->type) {
				case T_NUMBER:
					i_set_bucket_value(node, &htkey, LPC_INT_VALUE(value));
					break;
				case T_STRING:
					c_set_bucket_value(node, &htkey, LPC_STR_VALUE(value));
					break;
				case T_MAPPING:
					childnode = append_bucket_node(node, &htkey);
					fs_map_2_bucket(childnode, LPC_MAP_VALUE(value));
					break;
				default:
					break;
			}

		}
	}
	while (j--);
}

int bucket_unserial(fs_bucket_node_t node, char *str)
{
	svalue_t v;
	svalue_t *pv = &v;

	if (strlen(str) <= 1) return 0;

	restore_variable(pv, str); 

	if (!IS_LPC_MAP(pv)) {
		return 0;
	}

	fs_map_2_bucket(node, LPC_MAP_VALUE(pv));
	return 1;
}

// 从一个lpc的array获得root根结点下面的节点
static inline fs_bucket_node_t find_bucket_node_by_path(fs_bucket_node_t root, svalue_t *pathkey, int doNew)
{
	int arr_size, i;
	fs_bucket_node_t node;
	fs_bnode_value_t *bv = NULL;
	svalue_t *item;
	char *ret_val;
	fs_ht_key_t htkey;

	if (!IS_LPC_ARR(pathkey)) {
		return 0;
	}

	arr_size = LPC_ARR_SIZE(pathkey);
	node = root;
	i = 0;
	ret_val = NULL;

	while (i < arr_size && node ) {
		if (i == (arr_size - 1)) {
			return node;
		}
		item = (svalue_t *) LPC_ARR_ITEM(pathkey, i);
		if (parse_svalue_key(item, &htkey)) {
			if (doNew) {
				bv = _find_bucket_value(node, &htkey, 0);
				if (!BM_IS_TYPE_VALUE(bv, T_VALUE_DMAP)) {
					node = append_bucket_node(node, &htkey);
				} else {
					node = BM_DMAP_VALUE(bv);
				}
			} 
		}
		i++;
	}
	return 0;
}

void foreach_bucket_node(fs_bucket_node_t node, fs_bucket_hook_func_t func, void *arg)
{
	fs_bucket_t *bucket;
	fs_bnode_value_t *vob;
	fs_ht_node_t *hnode;

	if (NODE_NOT_VALID(node)) {
		return ;
	}
	bucket = _trans_node_2_struct(node);

	if (bucket) {
		FOR_EACH_HTABLE_OB(bucket->ohash, hnode)
		{
			vob = (fs_bnode_value_t *) hnode->ob;
			if (func(&hnode->hkey, vob, arg)) {
				break;
			}
		}
	}
}

void set_lru_bucket(fs_bucket_node_t node)
{
	fs_bucket_t *bucket;

	bucket = _trans_node_2_struct(node);
	if (bucket) {
		fs_ht_set_lru_hlist(bucket->ohash);
	}
}

// =======================================================
// efun
//

int get_bucket_svalue(fs_bucket_node_t root, svalue_t *pathkey, svalue_t *retvalue)
{
	fs_bucket_node_t node;
	fs_bnode_value_t *bv = NULL;
	svalue_t *key;
	int ret;
	fs_ht_key_t htkey;

	if (IS_LPC_ARR(pathkey)) {

		if (!(node = find_bucket_node_by_path(root, pathkey, 1))) {
			return 0;
		}
		key = (svalue_t *) LPC_ARR_ITEM(pathkey, LPC_ARR_SIZE(pathkey) - 1);
	} else {
		key = pathkey;
		node = root;
	}

	if (parse_svalue_key(key, &htkey)) {
		bv = _find_bucket_value(node, &htkey, 0);
	}

	if (!bv) {
		// error
		return 0;
	}

	ret = 0;
	switch(bv->type) {
		case T_VALUE_INT:
			set_svalue_number(retvalue, BM_INT_VALUE(bv)); 
			ret = 1;
			break;
		case T_VALUE_STR:
			set_svalue_shared_string(retvalue, BM_STR_VALUE(bv));
			ret = 1;
			break;
		default:
			break;
	}
	return ret;
}

int set_bucket_svalue(fs_bucket_node_t root, svalue_t *pathkey, svalue_t *value)
{
	fs_bucket_node_t node;
	svalue_t *key;
	int ret;
	fs_ht_key_t htkey;

	if (IS_LPC_ARR(pathkey)) {

		if (!(node = find_bucket_node_by_path(root, pathkey, 1))) {
			return 0;
		}
		key = (svalue_t *) LPC_ARR_ITEM(pathkey, LPC_ARR_SIZE(pathkey) - 1);
	} else {
		key = pathkey;
		node = root;
	}

	ret = 0;
	if (parse_svalue_key(key, &htkey)) {

		switch(value->type) {
			case T_NUMBER:
				i_set_bucket_value(node, &htkey, LPC_INT_VALUE(value));
				ret = 1;
				break;
			case T_STRING:
				c_set_bucket_value(node, &htkey, LPC_STR_VALUE(value));
				ret = 1;
				break;
			default:
				break;
		}
	}
	return ret;
}

void f_New_Bucket(void)
{
	int ret = fs_new_bucket(DEFAULT_SCRIPT_BUCKET_HSIZE);

	push_number(ret);
}

void f_Free_Bucket(void)
{
	svalue_t *bucket = (sp--);
	
	fs_free_bucket(LPC_INT_VALUE(bucket));
}

void f_I_SetBucket(void)
{
	svalue_t *value = (sp--);
	svalue_t *pathkey = (sp--);
	svalue_t *bucket = (sp--);
	int root;

	root = LPC_INT_VALUE(bucket);

	set_bucket_svalue(root, pathkey, value);

	free_svalue(pathkey, "I_SetBucket");
	free_svalue(value, "I_SetBucket");
}

void f_C_SetBucket(void)
{
	svalue_t *value = (sp--);
	svalue_t *pathkey = (sp--);
	svalue_t *bucket = (sp--);
	int root;

	root = LPC_INT_VALUE(bucket);

	set_bucket_svalue(root, pathkey, value);

	free_svalue(pathkey, "C_SetBucket");
	free_svalue(value, "C_SetBucket");
}

void f_I_GetBucket(void)
{
	svalue_t *pathkey = (sp--);
	svalue_t *bucket = (sp--);
	int root;
	svalue_t value;
	svalue_t *pvalue = &value;
	int ret;

	root = LPC_INT_VALUE(bucket);

	ret = get_bucket_svalue(root, pathkey, pvalue);

	free_svalue(pathkey, "I_GetBucket");

	if (ret) {
		if (IS_LPC_INT(pvalue)) {
			push_svalue(pvalue); 
			return;
		} else {
			// warning;
		}
	}
	push_svalue(&const0u); 
}

void f_C_GetBucket(void)
{
	svalue_t *pathkey = (sp--);
	svalue_t *bucket = (sp--);
	svalue_t value;
	svalue_t *pvalue = &value;
	int ret, root;

	root = LPC_INT_VALUE(bucket);

	ret = get_bucket_svalue(root, pathkey, pvalue);

	free_svalue(pathkey, "C_GetBucket");

	if (ret) {
		if (IS_LPC_STR(pvalue)) {
			push_svalue(pvalue); 
			return;
		} else {
			// warning;
		}
	}
	push_svalue(&const0u); 
}


void f_Bucket2Map(void)
{
	svalue_t *bucket = (sp--);

	mapping_t *ret;

	ret = fs_bucket_2_map(LPC_INT_VALUE(bucket));

	if (!ret) {
		push_svalue(&const0u); 
		return;
	}
	push_refed_mapping(ret);
}

