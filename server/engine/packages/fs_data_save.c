
#include "fs_data_save.h"
#include "fs_lpc_op.h"
#include "lpc_args_op.h"
#include "malloc_incl.h"
#include "fs_ipc.h"
#include "fs_marshal.h"
#include "dirty.h"

#include <assert.h>

#define OBJ_FAST_NAME "fast_index"
 char *save_var_list = "_GDBArray";

static fs_apr_hash_t db_obj_base = {NULL, NULL};

extern int g_sync_dbd_fd;

inline static void get_dirty_path(dirty_manage_t *mng, fs_dbi_array_t *arr)
{
	void *dbi_arr;
	int c = 0;
	dbi_arr = fs_dbi_array_alloc(arr, sizeof(fs_dbi_arr_t));

	for ( ; mng->parent; mng = get_manage(mng->parent), c++) {
		switch(mng->parent->type) {
		case T_MAPPING: {
			svalue_t *key = mng->self_key.map_key;
			switch (key->type) {
				case T_NUMBER:
					fs_dbi_array_push_int(arr, key->u.number); 
					break;
				case T_REAL:
					fs_dbi_array_push_real(arr, key->u.real); 
					break;
				case T_STRING:
					fs_dbi_array_push_lstring(arr, key->u.string, SVALUE_STRLEN(key)); 
					break;
				default:
					assert(0);
			}
			break;
		}
		case T_ARRAY:
			fs_dbi_array_push_int(arr, mng->self_key.arr_index);
			break;
		default:
			assert(0);
		}
	}

	fs_dbi_array_set_array(arr, dbi_arr, c);
}


inline static void get_map_dirty_data(svalue_t *svmap, fs_dbi_array_t *arr)
{
	mapping_t *map = svmap->u.map;
	dirty_manage_t *mng = map->dirty_mng;
	dirty_node_t *dirty_node = mng->dirty_node;
	dirty_key_t *dk;

	assert(svmap->type == T_MAPPING);
	assert(mng->self == svmap); 
	//get the path: array
	get_dirty_path(mng, arr);

	fs_dbi_array_push_mapping(arr, dirty_node->key_cnt);
	//printf("map dirty key size:%d\n", dirty_node->key_cnt);

	TAILQ_FOREACH(dk, &dirty_node->dirty_key_list, entry) {
		switch (dk->dirty_op) {
			case DIRTY_SET:
			case DIRTY_ADD: {
				svalue_t *key = dk->key.map_key;
				svalue_t *value = key + 1;
				assert(value == find_in_mapping(map, key));
				//printf("add or set dirty map k:%s,v:%s\n", save_variable(key), save_variable(value));
				fs_marshal(key, arr);
				fs_marshal(value, arr);
				break;
			}
			case DIRTY_DEL:
				fs_marshal(&dk->key.del, arr);
				fs_marshal(&const0u, arr);
				//printf("del dirty map k:%s\n", save_variable(&dk->key.del));
				break;
		}
	}
}

inline static void get_arr_dirty_data(svalue_t *svarr, fs_dbi_array_t *arr)
{
	array_t *array = svarr->u.arr;
	dirty_manage_t *mng = array->dirty_mng;
	dirty_node_t *dirty_node = mng->dirty_node;
	dirty_key_t *dk;

	assert(svarr->type == T_ARRAY);
	//get the path: array
	get_dirty_path(mng, arr);

	fs_dbi_array_push_array(arr, dirty_node->key_cnt * 2);

	TAILQ_FOREACH(dk, &dirty_node->dirty_key_list, entry) {
		switch (dk->dirty_op) {
		case DIRTY_SET:
		case DIRTY_ADD: {
			int idx = dk->key.arr_index;
			svalue_t key;
			set_svalue_number(&key, idx);
			svalue_t *value = &array->item[idx];
			fs_marshal(&key, arr);
			fs_marshal(value, arr);
			//printf("get dirty arr k:%s,v:%s\n", save_variable(&key), save_variable(value));
			break;
		}
		case DIRTY_DEL:
			fs_marshal(&dk->key.del, arr);
			fs_marshal(&const0u, arr);
			//printf("del dirty arr k:%s\n", save_variable(&dk->key.del));
			break;
		}
	}
}


void fs_marshal_dirty_map(svalue_t *svmap, fs_dbi_array_t *array)
{

	mapping_t *map = svmap->u.map;
	dirty_manage_t *mng = map->dirty_mng;
	dirty_root_t *dirty_root = mng->dirty_root;
	dirty_node_t *dirty_node;
	svalue_t *node;

	assert(mng);
	assert(IS_DIRTY_ROOT(mng));
	assert(svmap->type == T_MAPPING);

	//printf("dirty map entry:%d\n", dirty_root->node_cnt);
	fs_dbi_array_push_mapping(array, dirty_root->node_cnt);
	TAILQ_FOREACH(dirty_node, &dirty_root->dirty_node_list, entry) {
		node = dirty_node->mng->self;
		switch (node->type) {
			case T_MAPPING: {
				get_map_dirty_data(node, array);
				break;
			}
			case T_ARRAY: {
				get_arr_dirty_data(node, array);
				break;
			}
			default:
				assert(0);
		}
	}
}

int db_object_enable_dirty(object_t *obj)
{
	svalue_t *var_list;
	array_t *list;
	int i;

	if (obj->flags & O_DESTRUCTED) {
		return -1;
	}

	var_list = find_object_variable(obj, save_var_list);
	assert(var_list->type == T_ARRAY);
	list = var_list->u.arr;

	for (i = 0; i < list->size; ++i) {
		svalue_t *ov = find_object_variable(obj, list->item[i].u.string);
		begin_dirty_manage_map(ov, NULL, NULL);
		//char *var_name = list->item[i].u.string;
		//fprintf(stderr, "enable dirty object.dirty map:%s\n", var_name);
	}

	return 0;
}

static void dump_dirty_data(svalue_t *sv)
{
	dirty_key_t *dk;
	dirty_node_t *dirty_node;
	dirty_manage_t *mng = get_manage(sv);

	if (sv->type != T_MAPPING) {
		// printf("*** is not mapping\n");
		return;
	}

	if (mng == NULL) {
		// printf("*** no dirty manager\n");
		return;
	}

	dirty_manage_t *root_mng = get_manage(mng->root);
	dirty_root_t *dirty_root = root_mng->dirty_root;
	// printf("*** dirty nodes:%d\n", dirty_root->node_cnt);
	TAILQ_FOREACH(dirty_node, &dirty_root->dirty_node_list, entry) {
		//printf("*** dirty node:%s\n", save_variable(&dirty_node->node));
	}


	dirty_node = mng->dirty_node;
	if (dirty_node == NULL) {
		//printf("*** no dirty keys\n");
		return;
	}

	//printf("*** dirty key count:%d\n", dirty_node->key_cnt);
	TAILQ_FOREACH(dk, &dirty_node->dirty_key_list, entry) {
		switch (dk->dirty_op) {
			case DIRTY_SET: {
				svalue_t *key = dk->key.map_key;
				svalue_t *value = key + 1;
				//assert(value == find_in_mapping(map, key));
				printf("*** set dirty k:%s,v:%s\n", save_variable(key), save_variable(value));
				break;
			}
			case DIRTY_ADD: {
				svalue_t *key = dk->key.map_key;
				svalue_t *value = key + 1;
				//assert(value == find_in_mapping(map, key));
				printf("*** add dirty k:%s,v:%s\n", save_variable(key), save_variable(value));
				break;
			}
			case DIRTY_DEL:
				printf("*** del dirty k:%s\n", save_variable(&dk->key.del));
				break;
		}
	}
}

void f_print_map_dirty()
{
        svalue_t *data = sp;
	dump_dirty_data(data);
        pop_stack();
}

/////////////////////////////////////////////////////////////////////

inline static svalue_t *get_directed_node(svalue_t *node, svalue_t *key)
{
	switch (node->type) {
	case T_MAPPING: {
		return find_in_mapping(node->u.map, key);
	}
	case T_ARRAY: {
		array_t *arr = node->u.arr;
		return &arr->item[key->u.number];
	}
	default:
		assert(0);
		return NULL;
	}
}

inline static svalue_t *get_node(svalue_t *root, fs_dbi_array_iter_t *iter)
{
	assert(iter->tv->common.type == FS_DBI_ARRAY);

	int i, cnt;
	svalue_t *keys, *key, *node;
	svalue_t v = const0u;

	cnt = iter->tv->arr.cnt;
	FS_DBI_ARRAY_ITER_NEXT(iter);

	keys = calloc(cnt, sizeof(v));
	for (i = 0; i < cnt; ++i) {
		fs_unmarshal(&v, iter);
		memcpy(keys + i, &v, sizeof(v));
	}
	
	
	for (i = 0, node = root; i < cnt; ++i) {
		key = &keys[cnt - i - 1];
		node = get_directed_node(node, key);
		assert(key->type == T_NUMBER || key->type == T_STRING);
		assert(node->type == T_MAPPING || node->type == T_ARRAY);
	}

	for (i = 0; i < cnt; ++i) {
		free_svalue(&keys[i], "free path");
	}

	free(keys);

	return node;
}

inline static void merge_dirty_map(mapping_t *map, fs_dbi_array_iter_t *iter)
{
	int i;
	svalue_t k = const0u, v = const0u, *lv = NULL;
	int pair = iter->tv->map.pair;

	FS_DBI_ARRAY_ITER_NEXT(iter); //skip the map header
	//printf("dirty key size:%d\n", pair);
	for(i = 0; i < pair; ++i) {
		fs_unmarshal(&k, iter); //must be string or number
		assert(k.type == T_NUMBER || k.type == T_REAL || k.type == T_STRING);
		fs_unmarshal(&v, iter); 
		//printf("merge dirty map k:%s,v:%s\n", save_variable(&k), save_variable(&v));
		if (IS_LPC_UNDEF(&v)) { //del 
			mapping_delete(map, &k);
		} else { //set or add
			lv = find_for_insert(map, &k, 0);
			assign_svalue(lv, &v);
			free_svalue(&v, "merge");
		}
		free_svalue(&k, "merge");
	}
}

inline static void merge_dirty_arr(array_t *arr, fs_dbi_array_iter_t *iter)
{
	int i, idx;
	svalue_t k = const0u, v = const0u;
	int pair = iter->tv->arr.cnt/2;

	FS_DBI_ARRAY_ITER_NEXT(iter); //skip the arr header
	for(i = 0; i < pair; ++i) {
		fs_unmarshal(&k, iter); //must be string or number
		assert(k.type == T_NUMBER);
		idx = k.u.number;
		assert(0 <= idx && idx < arr->size);
		fs_unmarshal(&v, iter); 
		assign_svalue(&arr->item[idx], &v);
		//printf("merge dirty arr k:%s,v:%s\n", save_variable(&k), save_variable(&arr->item[idx]));
		free_svalue(&k, "merge");
		free_svalue(&v, "merge");
	}
}

inline static void handle_dirty_map(svalue_t *root, fs_dbi_array_iter_t *iter)
{
	int pair, i;
	svalue_t *node;

	assert(iter->tv->common.type == FS_DBI_MAPPING);

	pair = iter->tv->map.pair;
	FS_DBI_ARRAY_ITER_NEXT(iter);
	
	//printf("handle dirty map. entry size:%d\n", pair);
	for(i = 0; i < pair; ++i) {
		//extract the path
		node = get_node(root, iter);
		switch (node->type) {
		case T_MAPPING: {
			merge_dirty_map(node->u.map, iter);
			break;
		}
		case T_ARRAY:
			merge_dirty_arr(node->u.arr, iter);
			break;
		default:
			//printf("error node type:%d\n", node->type);
			assert(0);
		}
	}
}

void merge_dirty_db_object_to_map(svalue_t *root, fs_dbi_array_iter_t *iter)
{
	int i, pair;
	svalue_t *mv;
	svalue_t key = const0u;

	//dirty object is mapping
	assert(root->type == T_MAPPING);
	assert(iter->tv->common.type == FS_DBI_MAPPING);

	pair = iter->tv->map.pair;
	FS_DBI_ARRAY_ITER_NEXT(iter);

	//printf("dirty map cnt:%d\n", pair);
	//printf("merge object to map beg:%s\n", save_variable(root));
	for (i = 0; i < pair; ++i) {
		fs_unmarshal(&key, iter);
		//the first level just support mapping, not array
		assert(iter->tv->common.type == FS_DBI_MAPPING);
		assert(key.type == T_STRING);
		mv = find_for_insert(root->u.map, &key, 0);
		if (IS_LPC_UNDEF(mv)) {
			mapping_t *m = allocate_mapping(1);
			set_svalue_mapping(mv, m);
		}
		//printf("dirty map beg.k:%s,v:%s\n", save_variable(&key), save_variable(mv));
		handle_dirty_map(mv, iter);
		//printf("dirty map end.k:%s,v:%s\n", save_variable(&key), save_variable(mv));
		free_svalue(&key, "free dirty key");
	}

	//printf("merge object to map end:%s\n", save_variable(root));
}


int db_object_unmarshal(object_t *obj, fs_dbi_array_iter_t *iter)
{
	int i;
	array_t *list;
	svalue_t *var_list;
	mapping_node_t *elt;
	mapping_t *map;
	svalue_t svmap = const0u;

	var_list = find_object_variable(obj, save_var_list);
	assert(var_list->type == T_ARRAY);
	list = var_list->u.arr;

	fs_unmarshal(&svmap, iter);
	assert(svmap.type == T_MAPPING);
	map = svmap.u.map;
	i = (int)map->table_size;

	if (obj->flags & O_DESTRUCTED) {
		return -1;
	}

	if (list->size != map->count) {
		//todo warning
		fprintf(stderr, "db object unmarshal unequal size.size:%d,count:%d\n", list->size, map->count);
	}

	//以数据为准而不以vars为准，但冗余key的数据会丢失.
	do {
		for (elt = map->table[i]; elt; elt = elt->next) {	
			//svalue_t *ov = find_object_variable(obj, list->item[i].u.string);
			//svalue_t *mv = find_in_mapping(map, &list->item[i]);
			svalue_t *mk = elt->values;
			svalue_t *mv = elt->values + 1;
			assert(mk->type == T_STRING);
			//todo : optimization
			svalue_t *v = find_object_variable(obj, mk->u.string);
			if(IS_LPC_UNDEF(v)) {
				//todo warning
				fprintf(stderr, "db object unmarshal miss key in vars.key:%s\n", mk->u.string);
			} else {
				assign_svalue(v, mv);
			}
		}
	} while(i--);

	free_svalue(&svmap, "free dat db");
	return 0;
}

inline static void db_object_marshal(object_t *obj, fs_dbi_array_t *arr)
{
	svalue_t *var_list;
	array_t *list;
	int i;

	var_list = find_object_variable(obj, save_var_list);
	assert(var_list->type == T_ARRAY);
	list = var_list->u.arr;

	fs_dbi_array_push_mapping(arr, list->size);
	for (i = 0; i < list->size; ++i) {
		svalue_t *ov = find_object_variable(obj, list->item[i].u.string);
		if (IS_LPC_UNDEF(ov)) {
			//todo log
			//todo mapping cnt
			assert(0);
			continue;
		}
		//todo log
		if(ov->type != T_MAPPING) {
			fprintf(stderr, "error object marshal key:%s,v:%s\n", save_variable(&list->item[i]), save_variable(ov));
			assert(0);
		}
		// key 
		fs_marshal(&list->item[i], arr);
		// value
		fs_marshal(ov, arr);
	}
}


int db_object_marshal_dirty(object_t *obj, fs_dbi_array_t *arr)
{
	svalue_t *var_list;
	array_t *list;
	int i;
	int c = 0;
	void *dbi_map;

	if (obj->flags & O_DESTRUCTED) {
		return 0;
	}

	var_list = find_object_variable(obj, save_var_list);
	assert(var_list->type == T_ARRAY);

	list = var_list->u.arr;

	dbi_map = fs_dbi_array_alloc(arr, sizeof(fs_dbi_mapping_t));
	for (i = 0; i < list->size; ++i) {
		svalue_t *ov = find_object_variable(obj, list->item[i].u.string);
		//svalue_t *ov = &obj->variables[indexs->item[i].u.number];
		//todo log
		if (ov->type != T_MAPPING) {
			fprintf(stderr, "marshal dirty error! %s db_var must be mapping in %s\n", 
				list->item[i].u.string, obj->prog->name);
			assert(ov->type == T_MAPPING);
		}
		dirty_manage_t *root_mng = get_manage(ov);
		if (root_mng != NULL) {
			if (root_mng->dirty_root->node_cnt > 0) {
				// key 
				fs_marshal(&list->item[i], arr);
				// value
				fs_marshal_dirty_map(ov, arr);
				clear_dirty(ov);
				c++;
			}
		} else {
			fprintf(stderr, "error! %s has not dirty manager\n", list->item[i].u.string);
		}
	}
	fs_dbi_array_set_mapping(arr, dbi_map, c);
	return c;
}


inline static int top_marshal_dirty(svalue_t *key, svalue_t *value, fs_dbi_array_t *arr)
{
	dirty_manage_t *root_mng = get_manage(value);
	if (root_mng != NULL) {
		if (root_mng->dirty_root->node_cnt > 0) {
			fs_marshal(key, arr);
			fs_marshal_dirty_map(value, arr);
			clear_dirty(value);
			return 1;
		}
	} 
	return 0;
}

inline static int top_marshal(svalue_t *key, svalue_t *value, fs_dbi_array_t *arr)
{
	fs_marshal(key, arr);
	fs_marshal(value, arr);
	return 1;
}

typedef int (*marshal_f_t)(svalue_t *, svalue_t*, fs_dbi_array_t*);

int db_object_marshal_iter(object_t *obj, fs_db_object_index_t *index, fs_dbi_array_t *arr, marshal_f_t handler)
{
	int i;
	int c = 0;
	void *dbi_map;
	fs_db_object_pair_t *pair;

	if (obj->flags & O_DESTRUCTED) {
		return 0;
	}

	dbi_map = fs_dbi_array_alloc(arr, sizeof(fs_dbi_mapping_t));
	for (i = 0; i < index->cnt; ++i) {
		pair = &index->pairs[i];
		svalue_t *ok = &pair->svname;
		svalue_t *ov = &obj->variables[pair->index];
		assert(ok->type == T_STRING);
		assert(ov->type == T_MAPPING);
		c += handler(ok, ov, arr);
	}
	fs_dbi_array_set_mapping(arr, dbi_map, c);
	return c;
}


////////////////////////////////////////

int db_map_unmarshal(svalue_t *svmap, fs_dbi_array_iter_t *iter)
{
	svalue_t map = const0u;

	fs_unmarshal(&map, iter);

	if (map.type != T_MAPPING) {
		free_svalue(&map, "db user recv");
		return -1;
	}

	assign_svalue(svmap, &map);
	free_svalue(&map, "db dat recv");
	return 0;
}

int db_map_marshal_to(fs_net_connection_t *conn, fs_gd_header_t *header, db_key_t *key, svalue_t *svmap, int sid)
{

	fs_dbi_array_t arr;
	fs_dbi_array_construct(&arr);

	db_key_push(key, &arr);
	fs_dbi_array_push_int(&arr, sid);

	if (svmap != NULL) {
		//todo check the return
		fs_marshal(svmap, &arr);
	}

	fs_gd_send_cmd_mbuf(conn, header, &arr.mbuf);
	fs_dbi_array_destruct(&arr);
	return 0;
}

static int pack_gd_header(fs_gd_header_t *header, svalue_t *k, fs_dbi_array_t *arr)
{
	if (k->type == T_NUMBER) {
		header->uid = k->u.number;
		header->flag = FS_GD_FLAG_INT_KEY;
		return 1;
	} else if (k->type == T_STRING) {
		header->uid = FS_NULL_UID;
		header->flag = FS_GD_FLAG_STR_KEY;
		fs_dbi_array_push_lstring(arr, k->u.string, SVALUE_STRLEN(k) + 1);
		return 1;
	}

	return 0;
}

int fs_db_object_recv(fs_gd_header_t *header, void *data, size_t len);

static int load_db_sync(fs_net_connection_t *conn, fs_gd_header_t *hdr, fs_mbuf_t *mbuf)
{
	if (0 == fs_gd_send_sync(conn, hdr, mbuf)) {
		struct evbuffer *inevb = fs_gd_recv_sync(g_sync_dbd_fd);
		if (inevb != NULL) {
			fs_gd_header_t *retheader = (void*)EVBUFFER_DATA(inevb);
			char *data = (void*)(retheader->payload_len == 0 ? NULL : retheader + 1);

			if (retheader->cmd == FS_GD_CMD_DB_OBJ_DATA) {
				int ret = fs_db_object_recv(retheader, data, retheader->payload_len);
				evbuffer_free(inevb);
				return ret;
			} else {
				fprintf(stderr, "dbd input unknow cmd:%d\n", retheader->cmd);
				evbuffer_free(inevb);
				return -1;
			}
		}
	}

	return -1;
}

int fs_db_object_send(fs_net_connection_t *conn, fs_gd_cmd_t cmd, svalue_t *k, object_t *obj)
{
	int ret = 0;
	fs_dbi_array_t arr;
	fs_gd_header_t header;

	if (obj->flags & O_DESTRUCTED) {
		fprintf(stderr, "error send desctructed obj. type=%d\n", k->type);
		goto error;
	}

	memset(&header, 0, sizeof(header));
	header.cmd = cmd;
	fs_dbi_array_construct(&arr);
	if (!pack_gd_header(&header, k, &arr)) {
		goto error;
	}

	switch (header.cmd) {
		case FS_GD_CMD_DB_OBJ_NEW: 
		case FS_GD_CMD_DB_OBJ_DATA:
			db_object_marshal(obj, &arr);
			fs_gd_send_cmd_mbuf(conn, &header, &arr.mbuf);
			break;
		case FS_GD_CMD_DB_OBJ_DATA_DIRTY: {
			int c = db_object_marshal_dirty(obj, &arr);
			if (c > 0) {
				fs_gd_send_cmd_mbuf(conn, &header, &arr.mbuf);
			}
			break;
		  }
		default:
			goto error;
	}

	fs_dbi_array_destruct(&arr);
	return ret;
error:
	fs_dbi_array_destruct(&arr);
	return -1;
}

int fs_db_object_load(fs_net_connection_t *conn, svalue_t *k, int sync, int sid)
{
	int ret = 0;
	fs_dbi_array_t arr;
	fs_gd_header_t header;

	memset(&header, 0, sizeof(header));
	fs_dbi_array_construct(&arr);

	if (!pack_gd_header(&header, k, &arr)) {
		goto error;
	}
	fs_dbi_array_push_int(&arr, sid);
	if (sync) {
		header.cmd =  FS_GD_CMD_DB_OBJ_LOAD_SYNC;
		ret = load_db_sync(conn, &header, &arr.mbuf);
	} else {
		header.cmd =  FS_GD_CMD_DB_OBJ_LOAD;
		fs_gd_send_cmd_mbuf(conn, &header, &arr.mbuf);
	}

	fs_dbi_array_destruct(&arr);
	return ret;
error:
	fs_dbi_array_destruct(&arr);
	return -1;
}

int fs_db_object_unload(fs_net_connection_t *conn, svalue_t *k)
{
	int ret = 0;
	fs_dbi_array_t arr;
	fs_gd_header_t header;

	memset(&header, 0, sizeof(header));
	header.cmd = FS_GD_CMD_DB_OBJ_UNLOAD;
	fs_dbi_array_construct(&arr);

	if (!pack_gd_header(&header, k, &arr)) {
		goto error;
	}
	fs_gd_send_cmd_mbuf(conn, &header, &arr.mbuf);
	fs_dbi_array_destruct(&arr);
	return ret;
error:
	fs_dbi_array_destruct(&arr);
	return -1;
}

int fs_db_object_save(fs_net_connection_t *conn, fs_gd_cmd_t cmd, svalue_t *k, object_t *obj, fs_db_object_index_t *index)
{
	fs_dbi_array_t arr;
	int c = 0;

	fs_gd_header_t header;
	memset(&header, 0, sizeof(header));
	header.cmd = cmd;
	fs_dbi_array_construct(&arr);

	if (obj->flags & O_DESTRUCTED) {
		fprintf(stderr, "error send desctructed obj. type=%d\n", k->type);
		goto error;
	}

	if (!pack_gd_header(&header, k, &arr)) {
		fprintf(stderr, "error save skip bad type key. type=%d\n", k->type);
		goto error;
	}


	if (cmd == FS_GD_CMD_DB_OBJ_DATA_DIRTY) {
		c = db_object_marshal_iter(obj, index, &arr, top_marshal_dirty);
	} else {//todo must be _OBJ_DATA
		c = db_object_marshal_iter(obj, index, &arr, top_marshal);
	}

	if (c > 0) {
		fs_gd_send_cmd_mbuf(conn, &header, &arr.mbuf);
	}

	fs_dbi_array_destruct(&arr);
	return 0;

error:
	fs_dbi_array_destruct(&arr);
	return -1;
}

int fs_db_object_rename(fs_net_connection_t *conn, svalue_t *k, object_t *obj, svalue_t *newk)
{
	fs_dbi_array_t arr;
	fs_gd_header_t header;
	memset(&header, 0, sizeof(header));
	header.cmd = FS_GD_CMD_RENAME;
	fs_dbi_array_construct(&arr);

	if (obj->flags & O_DESTRUCTED) {
		fprintf(stderr, "error send desctructed obj. type=%d\n", k->type);
		goto error;
	}

	if (!pack_gd_header(&header, k, &arr)) {
		fprintf(stderr, "error save skip bad type key. type=%d\n", k->type);
		goto error;
	}

	//目前不能rename整型的key即玩家对象，只能rename字符串的key即dat的对象
	if (newk->type == T_STRING) {
		fs_dbi_array_push_lstring(&arr, newk->u.string, SVALUE_STRLEN(newk) + 1);
	} else {
		goto error;
	}

	fs_gd_send_cmd_mbuf(conn, &header, &arr.mbuf);
	fs_dbi_array_destruct(&arr);
	return 0;
error:
	fs_dbi_array_destruct(&arr);
	return -1;
}

void db_key_init(db_key_t *key, int type, void *value, unsigned len)
{
	key->type = type;
	key->len = len;

	if (DB_KEY_IS_INT(key)) {
		key->value = malloc(key->len);
		memcpy(key->value, value, key->len);
	} else {
		key->value = malloc(key->len + 1);
		memcpy(key->value, value, key->len);
		DB_KEY_STR(key)[key->len] = '\0';
	}
}

void db_key_free(db_key_t *key)
{
	free(key->value);
}

void db_key_push(db_key_t *key, fs_dbi_array_t *arr)
{
	if (DB_KEY_IS_STR(key)) {
		//append the extra '\0'
		fs_dbi_array_push_lstring(arr, DB_KEY_STR(key), key->len + 1);
	}
}

void db_key_ref(fs_gd_header_t *header, fs_dbi_array_iter_t *iter, db_key_t *key)
{
	if (header->flag == FS_GD_FLAG_INT_KEY) {
		key->type = DB_KEY_TYPE_INT;
		key->len = sizeof(header->uid);
		key->value = &header->uid;
	} else {
		assert(header->flag == FS_GD_FLAG_STR_KEY);
		key->type = DB_KEY_TYPE_STR;
		key->value = iter->tv->string.str;
		key->len = iter->tv->string.len - 1;
		FS_DBI_ARRAY_ITER_NEXT(iter);
	}
}

void db_key_svalue_ref(db_key_t *key, svalue_t *v)
{
	if (v->type == T_NUMBER) {
		DB_KEY_INT_REF(key, &v->u.number);
	} else {
		DB_KEY_SVALUE_STR_REF(key, v);
	}
}


void fs_db_object_index_free(fs_db_object_index_t *index)
{
	int i;
	fs_db_object_pair_t *pair;

	for (i = 0; i < index->cnt; ++i) {
		pair = &index->pairs[i];
		free_svalue(&pair->svname, "db object index");
		
	}

	free(index->pairs);
	free_prog(index->prog, 1);
	free(index);
}

inline static fs_db_object_index_t *fs_db_object_index_find(fs_apr_hash_t *base, object_t *obj)
{
	program_t *prog = obj->prog;
	fs_db_object_index_t *index =apr_hash_get(base->hash, prog, sizeof(prog));
	return index;
}

static fs_db_object_index_t *fs_db_object_index_find_by_name(fs_apr_hash_t *base, object_t *obj)
{
	apr_hash_index_t *hi;
	void *index;
	fs_db_object_index_t *dbo_index;
	char *name = obj->prog->name;

	for (hi = apr_hash_first(NULL, base->hash); hi; hi = apr_hash_next(hi)) {
		apr_hash_this(hi, NULL, NULL, &index);
		dbo_index = index;
		if (!strcmp(name, dbo_index->prog->name)) {
			return dbo_index;
		}
	}

	return NULL;
}


static fs_db_object_index_t *fs_db_object_index_add(fs_apr_hash_t *base, object_t *obj)
{
	if (NULL == fs_db_object_index_find(base, obj)) {
		svalue_t *vars = find_object_variable(obj, save_var_list);
		if (vars->type != T_ARRAY) {
			fprintf(stderr, "error db object bad type variable list:%s\n", type_name(vars->type));
			return NULL;
		}

		int i;
		array_t *list = vars->u.arr;

		fs_db_object_pair_t *pairs = calloc(list->size, sizeof(fs_db_object_pair_t));
		fs_db_object_index_t *dbo_index = calloc(1, sizeof(fs_db_object_index_t));
		dbo_index->cnt = 0;
		dbo_index->pairs = pairs;
		dbo_index->prog = obj->prog;
		reference_prog(dbo_index->prog, "db object index");

		for (i = 0; i < list->size; ++i) {
			assert(list->item[i].type == T_STRING);
			unsigned short type;
			char *name = list->item[i].u.string;
			int index = find_global_variable(obj->prog, name, &type);
			if (index == -1) {
				//todo warning
				fprintf(stderr, "warning db object can not find variable:%s\n", name);
				continue;
			}
			assign_svalue_no_free(&pairs[dbo_index->cnt].svname, &list->item[i]);
			pairs[dbo_index->cnt++].index = index;
			//fprintf(stderr, "add db object index.module=%s,k=%s\n", obj->prog->name, name);
		}
		apr_hash_set(base->hash, dbo_index->prog, sizeof(dbo_index->prog), dbo_index);

		return dbo_index;
	}

	return NULL;
}

void fs_db_object_index_update(fs_apr_hash_t *base, object_t *obj)
{
	fs_db_object_index_t *dbo_index = fs_db_object_index_find_by_name(base, obj);
	if (dbo_index != NULL) {
		apr_hash_set(base->hash, dbo_index->prog, sizeof(dbo_index->prog), NULL);
		fs_db_object_index_free(dbo_index);
		fs_db_object_index_add(base, obj);
	}
}

static void fs_db_object_index_clear(fs_apr_hash_t *base)
{
	apr_hash_index_t *hi;
	void *index;
	fs_db_object_index_t *dbo_index;

	for (hi = apr_hash_first(NULL, base->hash); hi; hi = apr_hash_next(hi)) {
		apr_hash_this(hi, NULL, NULL, &index);
		dbo_index = index;
		fs_db_object_index_free(dbo_index);
	}

	apr_hash_clear(base->hash);
}

fs_db_object_index_t *fs_db_object_index_find_or_add(object_t *obj)
{
	fs_db_object_index_t *index = fs_db_object_index_find(&db_obj_base, obj);
	if (index == NULL) {
		index = fs_db_object_index_add(&db_obj_base ,obj);
	}

	assert(index != NULL);
	return index;
}


void fs_db_object_module_init()
{
	fs_apr_hash_construct(&db_obj_base);
}

void fs_db_object_module_exit()
{
	fs_db_object_index_clear(&db_obj_base);
	fs_apr_hash_destruct(&db_obj_base);
}

void fs_db_object_module_update(object_t *obj)
{
	fs_db_object_index_update(&db_obj_base, obj);
}

