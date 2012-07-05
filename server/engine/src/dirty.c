// 脏数据管理 
// ----------------------------------

#include "dirty.h"
#include "lpc.h"
#include "lpc_incl.h"
#include "object.h"
#include "malloc_incl.h"

#include <assert.h>

//lpc 虚拟机依赖外部的packages，不干净了。
#include "packages/fs_mbuf.h"

static fs_free_array_t pool_key;
static fs_free_array_t pool_node;
static fs_free_array_t pool_manage;
static fs_free_array_t pool_manage_root;

#define PAGE_ELEMENT_CNT 8 //为了测试，先别设置太大了

#define MEM_POOL_ALLOC(which) fs_free_array_alloc(&(which))
#define MEM_POOL_FREE(which, ptr) fs_free_array_free(&(which), (ptr))

//#define DIRTY_DUMP_DEBUG 1

void dirty_mem_pool_setup()
{
	fs_free_array_init(&pool_key, "dirty_key", sizeof(dirty_key_t), PAGE_ELEMENT_CNT);
	fs_free_array_init(&pool_node, "dirty_node", sizeof(dirty_node_t), PAGE_ELEMENT_CNT);
	fs_free_array_init(&pool_manage, "dirty_manage", sizeof(dirty_manage_t), PAGE_ELEMENT_CNT);
	fs_free_array_init(&pool_manage_root, "dirty_manage_root", sizeof(dirty_manage_t) + sizeof(dirty_root_t), PAGE_ELEMENT_CNT);
}

void dirty_mem_pool_clear()
{
	//todo 
}

void dirty_mem_pool_stat()
{
	fprintf(stderr, "dirty key total_size:%d,element_size:%d,total:%d,alloc:%d\n", 
		pool_key.mbuf.alloc_size, pool_key.element_size, 
		pool_key.element_total, pool_key.element_alloc);
	fprintf(stderr, "dirty node total_size:%d,element_size:%d,total:%d,alloc:%d\n", 
		pool_node.mbuf.alloc_size, pool_node.element_size, 
		pool_node.element_total, pool_node.element_alloc);
	fprintf(stderr, "dirty manage total_size:%d,element_size:%d,total:%d,alloc:%d\n", 
		pool_manage.mbuf.alloc_size, pool_manage.element_size,
		pool_manage.element_total, pool_manage.element_alloc);
	fprintf(stderr, "dirty manage_root total_size:%d,element_size:%d,total:%d,alloc:%d\n", 
		pool_manage_root.mbuf.alloc_size, pool_manage_root.element_size,
		pool_manage_root.element_total, pool_manage_root.element_alloc);
}

static void free_dirty_map_recurse(mapping_t *map);
static void free_dirty_arr_recurse(array_t *arr);
static void clear_dirty_map_recurse(mapping_t *map);
static void clear_dirty_arr_recurse(array_t *arr);
static void clear_dirty_map(mapping_t *map);
static void clear_dirty_arr(array_t *arr);
static void assert_attach_dirty_arr_recurse(svalue_t *svarr);
static void assert_attach_dirty_map_recurse(svalue_t *svmap);

inline static void free_map_dirty_key(dirty_key_t *dk)
{
	if (dk->dirty_op == DIRTY_DEL) {
		free_svalue(&dk->key.del, "dirty node release");
	}
	MEM_POOL_FREE(pool_key, dk);
}

inline static dirty_key_t *new_map_dirty_key(svalue_t *key, unsigned char op)
{
	dirty_key_t *dk = MEM_POOL_ALLOC(pool_key);
	dk->dirty_op = op;
	if (dk->dirty_op == DIRTY_DEL) {
		set_svalue_undefined(&dk->key.del);
		assign_svalue(&dk->key.del, key);
	} else {
		dk->key.map_key = key;
	}
	return dk;
}

inline static dirty_key_t *new_arr_dirty_key(int idx, unsigned char op)
{
	dirty_key_t *dk = MEM_POOL_ALLOC(pool_key);
	assert(dk->dirty_op != DIRTY_DEL);
	dk->dirty_op = op;
	dk->key.arr_index = idx;
	return dk;
}

inline static void free_arr_dirty_key(dirty_key_t *dk)
{
	MEM_POOL_FREE(pool_key, dk);
}

inline static void overwrite_map_dirty_key(dirty_key_t *dk, svalue_t *key, unsigned char op)
{
	if (dk->dirty_op == DIRTY_DEL) {
		free_svalue(&dk->key.del, "dirty node release");
	}
	dk->dirty_op = op;
	if (dk->dirty_op == DIRTY_DEL) {
		assign_svalue_no_free(&dk->key.del, key);
	} else {
		dk->key.map_key = key;
	}
}

inline static void overwrite_arr_dirty_key(dirty_key_t *dk, int idx, unsigned char op)
{
	dk->dirty_op = op;
	dk->key.arr_index = idx;
}

inline static void dirty_root_init(dirty_root_t *dirty_root, svalue_t *root)
{
	TAILQ_INIT(&dirty_root->dirty_node_list);
	dirty_root->node_cnt = 0;
}

inline static void dirty_root_add(dirty_root_t *dirty_root, dirty_node_t *dirty_node)
{
	//fprintf(stderr, "add dirty node to list:%p\n", dirty_node);
	TAILQ_INSERT_TAIL(&dirty_root->dirty_node_list, dirty_node, entry);
	dirty_root->node_cnt++;
}

inline static void dirty_root_remove(dirty_root_t *dirty_root, dirty_node_t *dirty_node)
{
	//fprintf(stderr, "remove dirty node from list:%p\n", dirty_node);
	TAILQ_REMOVE(&dirty_root->dirty_node_list, dirty_node, entry);
	dirty_root->node_cnt--;
}

static dirty_node_t *new_dirty_node(dirty_manage_t *mng)
{
	dirty_node_t *dirty_node = MEM_POOL_ALLOC(pool_node);
	//fprintf(stderr, "new dirty node:%p\n", dirty_node);

	dirty_node->key_cnt = 0;
	TAILQ_INIT(&dirty_node->dirty_key_list);

	dirty_manage_t *root_mng = get_manage(mng->root);
	dirty_root_add(root_mng->dirty_root, dirty_node);

	dirty_node->mng = mng;
	mng->dirty_node = dirty_node;

	return dirty_node;
}

inline static void destroy_dirty_node(dirty_manage_t *mng)
{
	dirty_root_t *dirty_root  = get_manage(mng->root)->dirty_root;
	dirty_root_remove(dirty_root, mng->dirty_node);

	mng->dirty_node->mng = NULL;
	//fprintf(stderr, "free dirty node:%p\n", mng->dirty_node);
	MEM_POOL_FREE(pool_node, mng->dirty_node);
	mng->dirty_node = NULL;
}

static void free_dirty_node(dirty_manage_t *mng)
{
	dirty_node_t *dirty_node = mng->dirty_node;
	dirty_key_t *dk, *next;

	switch (mng->self->type) {
		case T_MAPPING:
			TAILQ_FOREACH_SAFE(dk, &dirty_node->dirty_key_list, entry, next) {
				free_map_dirty_key(dk);
			}
			break;
		case T_ARRAY:
			TAILQ_FOREACH_SAFE(dk, &dirty_node->dirty_key_list, entry, next) {
				free_arr_dirty_key(dk);
			}
			break;
		default:
			assert(0);
	}

	destroy_dirty_node(mng);
}

static void dump_dirty_info(dirty_manage_t *dirty_mng, dirty_key_t *dkey, svalue_t *value, char *msg) 
{
	svalue_t *root = dirty_mng->root;
	svalue_t *parent = dirty_mng->parent;
	svalue_t *self = dirty_mng->self;

	if (dkey) {
		if (self->type == T_ARRAY) {
			fprintf(stderr, "%s arr dirty_key=%d\n\n", msg, dkey->key.arr_index);
		} else {
			fprintf(stderr, "%s map dirty_key=%s\n\n", msg, save_variable(dkey->key.map_key));
		}
	}

	fprintf(stderr, "%s dirty_value svalue=%p,p=%p,content=%s\n\n", msg, value, value->u.refed, save_variable(value));
	fprintf(stderr, "%s dbvar svalue=%p,p=%p,content=%s\n\n", msg, root, root->u.refed, save_variable(root));
	if (parent) {
		fprintf(stderr, "%s parent svalue=%p,p=%p,content=%s\n", msg, parent, parent->u.refed, save_variable(parent));
		if (parent->type == T_ARRAY) {
			int idx = dirty_mng->self_key.arr_index;
			svalue_t *self_value = &parent->u.arr->item[idx];
			fprintf(stderr, "%s arr self_key=%d\n\n", msg, idx);
			fprintf(stderr, "%s arr self_value=%p,p=%p\n\n", msg, self_value, self_value->u.refed);
		} else {
			svalue_t *self_key = dirty_mng->self_key.map_key;
			svalue_t *self_value = self_key + 1;
			fprintf(stderr, "%s map self_key=%p,p=%p,%s\n\n", 
				msg, self_key, self_key->u.refed, save_variable(self_key));
			fprintf(stderr, "%s map self_value=%p,p=%p,%s\n\n", 
				msg, self_value, self_value->u.refed, save_variable(self_value));
		}
	}
	fprintf(stderr, "%s self svalue=%p,p=%p,content=%s\n\n", msg, self, self->u.refed, save_variable(self));
	fprintf(stderr, "%s null array p=%p\n\n", msg, &the_null_array);
}

void assert_attach(dirty_manage_t *dirty_mng, dirty_key_t *dkey, svalue_t *value)
{
	#ifdef DIRTY_DUMP_DEBUG
	if (value->type == T_MAPPING || value->type == T_ARRAY) {
		dump_dirty_info(dirty_mng, dkey, value, "try assert attach");
	}
	#endif

	switch(value->type) {
		case T_MAPPING:
			assert_attach_dirty_map_recurse(value);
			break;
		case T_ARRAY:
			assert_attach_dirty_arr_recurse(value);
			break;
	}
}

void assert_detach(int op, svalue_t *value)
{
	if (op != DIRTY_ADD) {
		switch (value->type) {
			case T_MAPPING:
				#ifdef DIRTY_DUMP_DEBUG
				fprintf(stderr, "detach map.op=%d,svalue:%p,p:%p,content:%s\n", 
					op, value, value->u.refed, save_variable(value));
				#endif
				break;
			case T_ARRAY:
				#ifdef DIRTY_DUMP_DEBUG
				fprintf(stderr, "detach map.op=%d,svalue:%p,p:%p,content:%s\n", 
					op, value, value->u.refed, save_variable(value));
				#endif
				break;
		}
	} 
}

//todo: Add or Set 入dirty map的value，要检测其是否已经在别的dirty map中，然后再挂入dirty map.
inline static void attach_node(svalue_t *value, svalue_t *parent, self_key_t *self_key)
{
	switch((value)->type) {
		case T_MAPPING:
			begin_dirty_manage_map(value, parent, self_key);
			break;
		case T_ARRAY:
			begin_dirty_manage_array(value, parent, self_key);
			break;
	}
}

//延迟处理，在清理脏数据的key的时候才把新增进来的mapping启动脏数据管理
static void clear_dirty_node(dirty_manage_t *mng)
{
	dirty_key_t *dk, *next;
	dirty_node_t *dirty_node = mng->dirty_node;
	svalue_t *node = mng->self;

	assert(dirty_node);
	switch (node->type) {
		case T_MAPPING:
			TAILQ_FOREACH_SAFE(dk, &dirty_node->dirty_key_list, entry, next) {
				switch (dk->dirty_op) {
					case DIRTY_ADD:
					case DIRTY_SET: {
						svalue_t *k = dk->key.map_key;
						svalue_t *v = k + 1;	
						self_key_t self_key;
						self_key.map_key = k;
						#ifdef DIRTY_MAP_CHECK
						assert_attach(mng, dk, v);
						#endif
						attach_node(v, node, &self_key);
						break;
					}
				}
				free_map_dirty_key(dk);
			}
			break;
		case T_ARRAY:
			TAILQ_FOREACH_SAFE(dk, &dirty_node->dirty_key_list, entry, next) {
				switch (dk->dirty_op) {
					case DIRTY_ADD:
					case DIRTY_SET: {
						int idx = dk->key.arr_index;
						svalue_t *v = &node->u.arr->item[idx];
						self_key_t self_key;
						self_key.arr_index = idx;
						#ifdef DIRTY_MAP_CHECK
						assert_attach(mng, dk, v);
						#endif
						attach_node(v, node, &self_key);
						break;
					}
				}
				free_arr_dirty_key(dk);
			}
			break;
		default:
			assert(0);
	}

	destroy_dirty_node(mng);
}

static dirty_key_t *dirty_node_insert_map_key(dirty_node_t* dirty_node, svalue_t* key, unsigned char op)
{
	dirty_key_t *dk = new_map_dirty_key(key, op);
	TAILQ_INSERT_TAIL(&dirty_node->dirty_key_list, dk, entry);
	dirty_node->key_cnt++;
	return dk;
} 

static dirty_key_t *dirty_node_insert_arr_key(dirty_node_t* dirty_node, int idx, unsigned char op)
{
	dirty_key_t *dk = new_arr_dirty_key(idx, op);
	TAILQ_INSERT_TAIL(&dirty_node->dirty_key_list, dk, entry);
	dirty_node->key_cnt++;
	return dk;
} 

dirty_key_t *dirty_node_find_map_key(dirty_node_t *dirty_node, svalue_t *key)
{
	dirty_key_t *dk;

	TAILQ_FOREACH(dk, &dirty_node->dirty_key_list, entry) {
		if (dk->dirty_op == DIRTY_DEL) {
			if (sameval(&dk->key.del, key)) {
				return dk;
			}
		} else {
			if (dk->key.map_key == key) {
				return dk;
			} 
		}
	}

	return NULL;
}

dirty_key_t *dirty_node_find_arr_key(dirty_node_t *dirty_node, int idx)
{
	dirty_key_t *dk;
	TAILQ_FOREACH(dk, &dirty_node->dirty_key_list, entry) {
		if (dk->key.arr_index == idx) {
			return dk;
		} 
	}

	return NULL;
}



inline static void dirty_manage_init(dirty_manage_t *mng, svalue_t *self, svalue_t *root, svalue_t *parent, self_key_t*self_key)
{
	mng->root = root;
	mng->parent = parent;
	mng->self = self;

	if (self_key) {
		mng->self_key = *self_key;
	} else { //root
		memset(&mng->self_key, 0, sizeof(self_key_t));
	}
	mng->dirty_node = NULL;
}

static void free_dirty_manage(dirty_manage_t *mng)
{
	int is_root = IS_DIRTY_ROOT(mng);
	if (mng->dirty_node) {
		free_dirty_node(mng);
	}
	mng->root = NULL;
	mng->parent = NULL;
	mng->self = NULL;

	if (is_root) {
		MEM_POOL_FREE(pool_manage_root, mng);
	} else {
		MEM_POOL_FREE(pool_manage, mng);
	}
}

static dirty_manage_t *new_dirty_manage(svalue_t *self, svalue_t *parent, self_key_t *self_key)
{
	dirty_manage_t *mng;

	if (parent == NULL) { //the root 
		mng = MEM_POOL_ALLOC(pool_manage_root);
		dirty_manage_init(mng, self, self, NULL, NULL);
		dirty_root_init(mng->dirty_root, self);
	} else {
		mng = MEM_POOL_ALLOC(pool_manage);
		svalue_t *root = get_manage(parent)->root;
		dirty_manage_init(mng, self, root, parent, self_key);
	}

	return mng;
}

void clear_dirty_manage(dirty_manage_t *mng)
{
	if (mng != NULL) {
		if (mng->dirty_node) {
			clear_dirty_node(mng);
		}
		//need not to handle dirty_root
		//dirty_root is affected by dirty_node of manage
	}
}


inline static void detach_node(int op, svalue_t *value)
{
	//todo check the added content if dirty
	//op == DIRTY_ADD has to delay, we do not know what will be added.
	//此刻不知道ADD或者SET进来的数据是什么，要延迟做检查处理。
	#ifdef DIRTY_MAP_CHECK
	assert_detach(op, value);
	#endif
	if (op != DIRTY_ADD) {
		switch (value->type) {
			case T_MAPPING:
				free_dirty_map_recurse(value->u.map);
				break;
			case T_ARRAY:
				free_dirty_arr_recurse(value->u.arr);
				break;
		}
	} 
}

inline static void map_accept_dirty_key(mapping_t *map, svalue_t *key, unsigned char op)
{
	dirty_manage_t *mng = map->dirty_mng;
	//put in the diry_node list
	if (mng->dirty_node == NULL) {
		new_dirty_node(mng); 
	}


	//has the same key ?
	dirty_key_t *dk = dirty_node_find_map_key(mng->dirty_node, key);
	if (dk == NULL) {
		dk = dirty_node_insert_map_key(mng->dirty_node, key, op);
		detach_node(op, key + 1);
	} else {
		detach_node(op, key + 1);
		overwrite_map_dirty_key(dk, key, op);
	}
}

inline static void arr_accept_dirty_key(array_t *arr, int idx, unsigned char op)
{
	dirty_manage_t *mng = arr->dirty_mng;
	assert(op == DIRTY_SET);

	//put in the diry_node list
	if (mng->dirty_node == NULL) {
		new_dirty_node(mng); 
	}

	//has the same key ?
	dirty_key_t *dk = dirty_node_find_arr_key(mng->dirty_node, idx);
	if (dk == NULL) {
		dk = dirty_node_insert_arr_key(mng->dirty_node, idx, op);
		detach_node(op, &arr->item[idx]);
	} else {
		detach_node(op, &arr->item[idx]);
		overwrite_arr_dirty_key(dk, idx, op);
	}
}

void set_dirty_map(mapping_t *map, svalue_t *key, unsigned char op)
{
	if (map->dirty_mng) {
		map_accept_dirty_key(map, key, op);
	}
}

void set_dirty_arr(array_t *arr, int idx, unsigned char op)
{
	if (arr->dirty_mng) {
		arr_accept_dirty_key(arr, idx, op);
	}
}

static void clear_dirty_map(mapping_t *map)
{
	if (map->dirty_mng) {
		clear_dirty_manage(map->dirty_mng);
	}
}

void free_dirty_map(mapping_t *map)
{
	if (map->dirty_mng) {
		free_dirty_manage(map->dirty_mng);
		map->dirty_mng = NULL;
	}
}

static void clear_dirty_arr(array_t *arr)
{
	if (arr->dirty_mng) {
		clear_dirty_manage(arr->dirty_mng);
	}
}

void free_dirty_arr(array_t *arr)
{
	if (arr->dirty_mng) {
		free_dirty_manage(arr->dirty_mng);
		arr->dirty_mng = NULL;
	}
}

static void assert_attach_dirty_map_recurse(svalue_t *svmap)
{
	int table_size, i;
	mapping_node_t *elt, **table;
	svalue_t *value;
	mapping_t *map = svmap->u.map;

	assert(svmap->type == T_MAPPING);

	if (map->dirty_mng) {
		dump_dirty_info(map->dirty_mng, NULL, svmap, "assert fail map from");
	} else {
		#ifdef DIRTY_DUMP_DEBUG
		fprintf(stderr, "assert ok attach map svalue:%p,p:%p,content:%s\n", 
			svmap, svmap->u.refed, save_variable(svmap));
		#endif
	}
	assert(map->dirty_mng == NULL);

	for (i = 0, table = map->table, table_size = map->table_size; i <= table_size; i++) {
		for (elt = table[i]; elt; elt = elt->next) {
			value = elt->values + 1;
			switch (value->type) {
				case T_MAPPING:
					assert_attach_dirty_map_recurse(value);
					break;
				case T_ARRAY:
					assert_attach_dirty_arr_recurse(value);
					break;
			}
		}
	}
}

static void clear_dirty_map_recurse(mapping_t *map)
{
	int table_size, i;
	mapping_node_t *elt, **table;
	svalue_t *value;

	clear_dirty_map(map);
	for (i = 0, table = map->table, table_size = map->table_size; i <= table_size; i++) {
		for (elt = table[i]; elt; elt = elt->next) {
			value = elt->values + 1;
			switch (value->type) {
				case T_MAPPING:
					clear_dirty_map_recurse(value->u.map);
					break;
				case T_ARRAY:
					clear_dirty_arr_recurse(value->u.arr);
					break;
			}
		}
	}
}

static void free_dirty_map_recurse(mapping_t *map)
{
	int table_size, i;
	mapping_node_t *elt, **table;
	svalue_t *value;

	for (i = 0, table = map->table, table_size = map->table_size; i <= table_size; i++) {
		for (elt = table[i]; elt; elt = elt->next) {
			value = elt->values + 1;
			switch (value->type) {
				case T_MAPPING:
					free_dirty_map_recurse(value->u.map);
					break;
				case T_ARRAY:
					free_dirty_arr_recurse(value->u.arr);
					break;
			}
		}
	}

	//root要最后清除,但这样就不能尾递归了
	free_dirty_map(map);
}


void begin_dirty_manage_map(svalue_t *svmap, svalue_t *parent, self_key_t *key)
{
	int table_size, i;
	mapping_node_t *elt,  **table;
	svalue_t *value;
	mapping_t *map;

	if (svmap->type != T_MAPPING) {
		return;
	}

	map = svmap->u.map;
	if (map->dirty_mng) {
		return ;
	}

	map->dirty_mng = new_dirty_manage(svmap, parent, key);
	for (i = 0, table = map->table, table_size = map->table_size; i <= table_size; i++) {
		for (elt = table[i]; elt; elt = elt->next) {
			self_key_t self_key;
			self_key.map_key = elt->values;
			value = elt->values + 1;
			switch (value->type) {
				case T_MAPPING:
					begin_dirty_manage_map(value, svmap, &self_key);
					break;
				case T_ARRAY:
					begin_dirty_manage_array(value, svmap, &self_key);
					break;
			}
		}
	}
}

void begin_dirty_manage_array(svalue_t *svarr, svalue_t *parent, self_key_t *self_key)
{
	int i;
	array_t *arr;

	if (svarr->type != T_ARRAY) {
		return;
	}

	arr = svarr->u.arr;
	if (arr->dirty_mng) {
		return ;
	}

	arr->dirty_mng = new_dirty_manage(svarr, parent, self_key);
	for (i = 0; i < arr->size; i++) {
		svalue_t *item = &arr->item[i];
		self_key_t self_key;
		self_key.arr_index = i;
		switch (item->type) {
			case T_MAPPING:
				begin_dirty_manage_map(item, svarr, &self_key);
				break;
			case T_ARRAY:
				begin_dirty_manage_array(item, svarr, &self_key);
				break;
		}
	}
}

static void assert_attach_dirty_arr_recurse(svalue_t *svarr)
{
	int i;
	svalue_t *item;
	array_t *arr = svarr->u.arr;

	assert(svarr->type == T_ARRAY);

	if (svarr->u.arr == &the_null_array) { //空数组是共享的全局变量，不能做检测
			return;
	}

	if (arr->dirty_mng) {
		dump_dirty_info(arr->dirty_mng, NULL, svarr, "assert fail arr from");
	} else {
		#ifdef DIRTY_DUMP_DEBUG
		fprintf(stderr, "assert ok attach arr svalue:%p,p:%p,content:%s\n", svarr, svarr->u.refed,save_variable(svarr));
		#endif
	}
	assert(arr->dirty_mng == NULL);

	for (i = 0; i < arr->size; i++) {
		item = &arr->item[i];
		switch (item->type) {
			case T_MAPPING:
				assert_attach_dirty_map_recurse(item);
				break;
			case T_ARRAY:
				assert_attach_dirty_arr_recurse(item);
				break;
		}
	}
}

static void clear_dirty_arr_recurse(array_t *arr)
{
	int i;
	svalue_t *item;

	clear_dirty_arr(arr);
	for (i = 0; i < arr->size; i++) {
		item = &arr->item[i];
		switch (item->type) {
			case T_MAPPING:
				clear_dirty_map_recurse(item->u.map);
				break;
			case T_ARRAY:
				clear_dirty_arr_recurse(item->u.arr);
				break;
		}
	}
}

static void free_dirty_arr_recurse(array_t *arr)
{
	int i;
	svalue_t *item;

	for (i = 0; i < arr->size; i++) {
		item = &arr->item[i];
		switch (item->type) {
			case T_MAPPING:
				free_dirty_map_recurse(item->u.map);
				break;
			case T_ARRAY:
				free_dirty_arr_recurse(item->u.arr);
				break;
		}
	}

	//root要最后清除,但这样就不能尾递归了
	free_dirty_arr(arr);
}

//从根开始清理脏数据
void clear_dirty(svalue_t *value)
{
	dirty_manage_t *mng = get_manage(value);
	if (mng != NULL) {
		assert(IS_DIRTY_ROOT(mng));
		svalue_t *node;
		dirty_node_t *dirty_node, *next;
		dirty_root_t *dirty_root = mng->dirty_root;

		TAILQ_FOREACH_SAFE(dirty_node, &dirty_root->dirty_node_list, entry, next) {
			//fprintf(stderr, "root clear dirty node:%p\n", dirty_node);
			node = dirty_node->mng->self;
			switch (node->type) {
				case T_MAPPING: {
					clear_dirty_map(node->u.map);
					break;
				}
				case T_ARRAY: {
					clear_dirty_arr(node->u.arr);
					break;
				}
			}
		}
	}
}

