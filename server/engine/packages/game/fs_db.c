
#include "fs_main.h"
#include "fs_db.h"
#include "fs_data_save.h"
#include "lpc_args_op.h"
#include "fs_marshal.h"
#include "fs_hlist.h"
#include "fs_log.h"
#include "fs_comm.h"

#define SAVE_ALL 0
#define SAVE_DIRTY 1

#define LOAD_ASYNC 0
#define LOAD_SYNC 1

extern int is_gamed_start;
extern fs_net_connection_t *dbd_conn;
extern fs_iobuf_t *dbd_io;
extern fs_obj_hash_t *online_user_tbl;



/*
void fs_dat_object_update(object_t *obj)
{
	fs_dat_t *dat;

	for (dat = dat_module; dat != NULL; dat = dat->hh.next) {
		if (0 == strcmp(obj->name, dat->dat_obj->name)) {
			fs_obj_save_vars_t *save_vars = fs_obj_save_vars_renew(dat->save_vars, obj, dat_var_list);
			if (NULL == save_vars) {
				fprintf(stderr, "update dat object %s save vars error!\n", obj->name);
				return;
			}
			dat->save_vars = save_vars;
			free_prog(dat->dat_obj->prog, 1);
			dat->dat_obj->prog = obj->prog;
			reference_prog(obj->prog, "dat object update");
			fprintf(stderr, "update dat object %s ok!\n", obj->name);
			return;
		}
	}
}

*/

int init_user_save_var(object_t *user_obj)
{
	return 0;
}

/////////////////////////////////////////

static buffer_t *mbuf_to_buffer(fs_mbuf_t *mbuf)
{
	unsigned start = 0;
	buffer_t *buf = allocate_buffer(mbuf->data_size);

	fs_mbuf_blk_t *blk;
	for (blk = mbuf->head; blk != NULL; blk = blk->next) {
		unsigned len = FS_MBUF_BLK_DATA_LEN(blk);
		write_buffer(buf, start, blk->head, len);
		start += len;
	}

	return buf;
}

void f_fs_enable_dirty_object()
{
	SET_ALL_ARGS 
	object_t *obj = ARGS(0)->u.ob;
	assert( 0 == db_object_enable_dirty(obj));
	POP_ALL_ARGS
	push_number(0);
}

void f_fs_marshal_dirty_object()
{
	fs_dbi_array_t arr;
	fs_dbi_array_construct(&arr);

	SET_ALL_ARGS 
	object_t *obj = ARGS(0)->u.ob;
	db_object_marshal_dirty(obj, &arr);
	POP_ALL_ARGS

	buffer_t *buf = mbuf_to_buffer(&arr.mbuf);
	fs_dbi_array_destruct(&arr);
	push_refed_buffer(buf);
}

void f_fs_merge_dirty_object()
{
	SET_ALL_ARGS 
	buffer_t *buf = ARGS(1)->u.buf;

	fs_dbi_array_iter_t iter;
	FS_DBI_ARRAY_ITER_FIRST(&iter, (char*)buf->item, buf->size);
	merge_dirty_db_object_to_map(ARGS(0), &iter);

	POP_ALL_ARGS
}

void f_fs_enable_dirty_map()
{
	SET_ALL_ARGS 
	begin_dirty_manage_map(ARGS(0), NULL, NULL);
	POP_ALL_ARGS
	push_number(0);
}

void f_fs_marshal_dirty_map()
{
	fs_dbi_array_t arr;
	fs_dbi_array_construct(&arr);

	SET_ALL_ARGS 
	fs_marshal_dirty_map(ARGS(0), &arr);
	POP_ALL_ARGS

	buffer_t *buf = mbuf_to_buffer(&arr.mbuf);
	fs_dbi_array_destruct(&arr);
	push_refed_buffer(buf);
}

void f_fs_merge_dirty_map()
{
}

void f_fs_marshal_map()
{
	fs_dbi_array_t arr;
	fs_dbi_array_construct(&arr);

	SET_ALL_ARGS 
	fs_marshal(ARGS(0), &arr);
	POP_ALL_ARGS

	buffer_t *buf = mbuf_to_buffer(&arr.mbuf);
	fs_dbi_array_destruct(&arr);
	push_refed_buffer(buf);
}

//////////////////////////////////////////////////////////////
static void log_destructed_obj(const char * what, svalue_t *k, object_t *obj)
{
	if (k == NULL ) {
		log_error(MOD_GAMED, "error db object has been destructed:%s,NULL,%s", what, obj->name);
		return;
	} 

	switch (k->type) {
	case T_NUMBER:
		log_error(MOD_GAMED, "error db object has been destructed:%s,%d,%s", what, k->u.number, obj->name);
		break;
	case T_STRING:
		log_error(MOD_GAMED, "error db object has been destructed:%s,%s,%s", what, k->u.string, obj->name);
		break;
	}
}

//int fs_db_object_new(int|string, object);
void f_fs_db_object_new(void)
{
	int ret = 0;
	SET_ALL_ARGS 
	if (is_gamed_start) {//编译-b时跳过这里
		svalue_t *k = ARGS(0);
		object_t *obj = ARGS(1)->u.ob;
		if (obj->flags & O_DESTRUCTED) {
			ret = -1;
			log_destructed_obj("new", k, obj);
		} else {
			fs_db_object_send(dbd_conn, FS_GD_CMD_DB_OBJ_NEW, k, obj);
		}
	}
	POP_ALL_ARGS
	push_number(ret);
}

//int fs_db_object_unload(int|string);
void f_fs_db_object_unload(void)
{
	SET_ALL_ARGS 
	if (is_gamed_start) {//编译-b时跳过这里
		svalue_t *k = ARGS(0);
		fs_db_object_unload(dbd_conn, k);
	}
	POP_ALL_ARGS
	push_number(0);
}

//int fs_db_object_save(int|string, object, int);
void f_fs_db_object_save(void)
{

	int ret = 0;
	SET_ALL_ARGS 
	if (is_gamed_start) {//编译-b时跳过这里
		svalue_t *k = ARGS(0);
		object_t *obj = ARGS(1)->u.ob;
		if (obj->flags & O_DESTRUCTED) {
			ret = -1;
			log_destructed_obj("save", k, obj);
		} else {
			int method = ARGS(2)->u.number == SAVE_DIRTY ? FS_GD_CMD_DB_OBJ_DATA_DIRTY: FS_GD_CMD_DB_OBJ_DATA;
			fs_db_object_send(dbd_conn, method, k, obj);
		}
	}
	POP_ALL_ARGS
	push_number(ret);
}

//int fs_db_object_rename(int|string(old), object, string(new));
void f_fs_db_object_rename(void)
{

	int ret = 0;
	SET_ALL_ARGS 
	if (is_gamed_start) {//编译-b时跳过这里
		svalue_t *k = ARGS(0);
		object_t *obj = ARGS(1)->u.ob;
		svalue_t *newk = ARGS(2);
		if (obj->flags & O_DESTRUCTED) {
			ret = -1;
			log_destructed_obj("rename", k, obj);
		} else {
			fs_db_object_rename(dbd_conn, k, obj, newk);
		}
	}
	POP_ALL_ARGS
	push_number(ret);
}

void f_fs_save_all_user()
{
	SET_ALL_ARGS 
	int method = ARGS(0)->u.number == SAVE_DIRTY ? FS_GD_CMD_DB_OBJ_DATA_DIRTY: FS_GD_CMD_DB_OBJ_DATA;
	POP_ALL_ARGS

	fs_user_t *user;
	fs_db_object_index_t *index = NULL;
	fs_ht_node_t *hnode;
	svalue_t key = const0;

	setproctitle("-gamed %s", "saving user");
	TIME_TAG_BEGIN(1, save_all_user)
	FOR_EACH_HTABLE_OB(online_user_tbl, hnode) {
		user = (fs_user_t *) hnode->ob;
		if (user->usernum > 0) {
			key.u.number = user->usernum;
			if (user->user_obj == NULL) {
				continue;
			}
			if (user->user_obj->flags & O_DESTRUCTED) {
				log_destructed_obj("save user", &key, user->user_obj);
				continue;
			}
			if (index == NULL) {
				index = fs_db_object_index_find_or_add(user->user_obj);
			}
			fs_db_object_save(dbd_conn, method, &key, user->user_obj, index);
		} else {
			fprintf(stderr, "error skip saving bad user uid=%d\n", user->usernum);
		}
	}
	TIME_TAG_END(1, save_all_user, "count=%d",  FS_HT_COUNT(online_user_tbl))
	restore_gamed_proc_title();

	push_number(0);
}

void f_fs_save_all_dat()
{
	SET_ALL_ARGS 
	mapping_t *dats = ARGS(0)->u.map;
	int method = ARGS(1)->u.number == SAVE_DIRTY ? FS_GD_CMD_DB_OBJ_DATA_DIRTY: FS_GD_CMD_DB_OBJ_DATA;
	svalue_t *k, *obj;
	int i;
	fs_db_object_index_t *index = NULL;
	mapping_node_t *elt, **table;

	setproctitle("-gamed %s", "saving dat");
	TIME_TAG_BEGIN(1, save_all_dat)

	for (i = 0, table = dats->table; i <= dats->table_size; i++) {
		for (elt = table[i]; elt; elt = elt->next) {
			k = elt->values;
			obj = elt->values + 1;
			if (k->type == T_STRING && obj->type == T_OBJECT) {
				if (obj->u.ob->flags & O_DESTRUCTED) {
					log_destructed_obj("save dat", k, obj->u.ob);
					continue;
				}
				index = fs_db_object_index_find_or_add(obj->u.ob);
				fs_db_object_save(dbd_conn, method, k, obj->u.ob, index);
			} else {
				fprintf(stderr, "error skip saving bad type dat.%s:%s\n", 
					type_name(k->type), type_name(obj->type));
				if (k->type == T_STRING && obj->type == T_NUMBER) {
					fprintf(stderr, "error skip saving bad type dat.%s:%d\n", 
						k->u.string, obj->u.number);
				}
			}
		}
	}
	TIME_TAG_END(1, save_all_dat, "count=%d", dats->count)

	restore_gamed_proc_title();

	POP_ALL_ARGS
	push_number(0);
}

//int fs_db_sync(int)
void f_fs_db_sync()
{
	SET_ALL_ARGS 
	if (is_gamed_start) {//编译-b时跳过这里
		int flag = ARGS(0)->u.number;
		fs_gd_header_t header;
		fs_dbi_array_t arr;

		fs_dbi_array_construct(&arr);
		fs_dbi_array_push_int(&arr, flag);
		FS_GD_SET_HEADER(&header, FS_GD_CMD_SYNC, FS_NULL_UID, 0);
		fs_gd_send_cmd_mbuf(dbd_conn, &header, &arr.mbuf);
		fs_dbi_array_destruct(&arr);
	}
	POP_ALL_ARGS
	push_number(0);
}

void f_fs_db_gamed_exit()
{
	SET_ALL_ARGS 
	if (is_gamed_start) {//编译-b时跳过这里
		fs_gd_header_t header;
		fs_dbi_array_t arr;
		fs_dbi_array_construct(&arr);
		FS_GD_SET_HEADER(&header, FS_GD_CMD_GAMED_EXIT, FS_NULL_UID, 0);
		fs_gd_send_cmd_mbuf(dbd_conn, &header, &arr.mbuf);
		fs_dbi_array_destruct(&arr);
	}
	POP_ALL_ARGS
	push_number(0);
}


//int fs_db_object_load(int|string, int, int);
void f_fs_db_object_load()
{

	int ret = -1;
	SET_ALL_ARGS 
	if (is_gamed_start) {//编译-b时跳过这里
		svalue_t *k = ARGS(0);
		int sync = ARGS(1)->u.number;
		int sid = ARGS(2)->u.number;
		ret = fs_db_object_load(dbd_conn, k, sync, sid);
	}
	POP_ALL_ARGS
	push_number(ret);
}

void f_fs_dirty_mem_pool_stat()
{
	SET_ALL_ARGS 
	dirty_mem_pool_stat();
	POP_ALL_ARGS
}


//int fs_db_object_restore(object, buffer);
void f_fs_db_object_restore()
{
	SET_ALL_ARGS 
	if (is_gamed_start) {//编译-b时跳过这里
		int sid;	
		object_t *obj = ARGS(0)->u.ob;
		buffer_t *buf = ARGS(1)->u.buf;

		db_key_t key;
		fs_dbi_array_iter_t iter;
		fs_gd_header_t *header = (void*)buf->item;

		if (header->stat != FS_GD_STAT_OK) {
			goto error;
		}
		if (obj->flags & O_DESTRUCTED) {
			log_destructed_obj("restore", NULL, obj);
			goto error;
		}

		FS_DBI_ARRAY_ITER_FIRST(&iter, buf->item + sizeof(fs_gd_header_t), buf->size - sizeof(fs_gd_header_t));
		db_key_ref(header, &iter, &key);
		sid = iter.tv->number.n;
		FS_DBI_ARRAY_ITER_NEXT(&iter);
		db_object_unmarshal(obj, &iter);
	}
	POP_ALL_ARGS
	push_number(0);
	return;

error:
	POP_ALL_ARGS
	push_number(-1);
}

int fs_db_object_recv(fs_gd_header_t *header, void *data, size_t len)
{
	fs_dbi_array_iter_t iter;
	db_key_t key;
	int stat = header->stat;
	int sid;

	//原封不动把数据先扔到脚本，restore的时候脚本再扔回来,多了一次内存的cp，但不是效率的关键.
	buffer_t *buf = allocate_buffer(sizeof(fs_gd_header_t) + len);
	write_buffer(buf, 0, (void*)header, sizeof(fs_gd_header_t));
	write_buffer(buf, sizeof(fs_gd_header_t), data, len);

	FS_DBI_ARRAY_ITER_FIRST(&iter, data, len);
	db_key_ref(header, &iter, &key);
	sid = iter.tv->number.n;
	FS_DBI_ARRAY_ITER_NEXT(&iter);

	//fprintf(stderr, "engine fs dat respone.savename:%s, len:%u, bufsize:%u\n", save_name, (int)len, buf->size);
	if (DB_KEY_IS_INT(&key)) {
		call_script_func("rc/db", "on_db_object_load", "iibi", stat, DB_KEY_INT(&key), buf, sid); 
	} else {
		call_script_func("rc/db", "on_db_object_load", "isbi", stat, DB_KEY_STR(&key), buf, sid); 
	}

	free_buffer(buf);

	return header->stat;
}


