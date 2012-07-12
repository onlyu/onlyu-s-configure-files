
#ifndef _FS_DATA_SAVE_H
#define _FS_DATA_SAVE_H

#include "fs_dbi.h"
#include "fs_comm.h"

#include "lpc.h"
#include "lpc_incl.h"

extern char *save_var_list;

#define DB_KEY_TYPE_INT 0
#define DB_KEY_TYPE_STR 1

typedef struct {
	unsigned type;
	unsigned len;
	void *value;
} db_key_t;

#define DB_KEY_INT(key) (*(fs_uid_t*)(key)->value)
#define DB_KEY_STR(key) ((char*)(key)->value)
#define DB_KEY_IS_INT(key) ((key)->type == DB_KEY_TYPE_INT)
#define DB_KEY_IS_STR(key) ((key)->type == DB_KEY_TYPE_STR)

#define DB_KEY_INT_REF(key, uidp) do {\
	(key)->type = DB_KEY_TYPE_INT;\
	(key)->value = (uidp);\
	(key)->len = sizeof(*(uidp));\
} while (0)

#define DB_KEY_STR_REF(key, str, sz) do {\
	(key)->type = DB_KEY_TYPE_STR;\
	(key)->value = (str);\
	(key)->len = (sz);\
} while (0)

#define DB_KEY_SVALUE_STR_REF(key, sv) DB_KEY_STR_REF(key, (sv)->u.string, SVALUE_STRLEN(sv))


#define DB_KEY_LOG(what, key, fmt, args...) 	\
do {	\
	char timefmt[128];\
	time_t seconds = time(NULL);\
	struct tm *tp = localtime(&seconds);\
	strftime(timefmt, sizeof(timefmt), "%F %T", tp);\
	if (DB_KEY_IS_INT(key)) {	\
		fprintf(stderr, "[%s] %s uid=%d "fmt"\n", timefmt, what, DB_KEY_INT(key), ##args);\
	} else {	\
		fprintf(stderr, "[%s] %s savename=%s "fmt"\n", timefmt, what, DB_KEY_STR(key), ##args);\
	}	\
} while(0)

void db_key_push(db_key_t *key, fs_dbi_array_t *arr);
void db_key_init(db_key_t *key, int type, void *value, unsigned len);
void db_key_free(db_key_t *key);
void db_key_ref(fs_gd_header_t *header, fs_dbi_array_iter_t *iter, db_key_t *ret);
void db_key_svalue_ref(db_key_t *key, svalue_t *v);


void fs_marshal_dirty_map(svalue_t *svmap, fs_dbi_array_t *array);
int db_object_unmarshal(object_t *obj, fs_dbi_array_iter_t *iter);

int db_map_unmarshal(svalue_t *svmap, fs_dbi_array_iter_t *iter);
int db_map_marshal_to(fs_net_connection_t *conn, fs_gd_header_t *header, db_key_t *key, svalue_t *svmap, int sid);

int fs_db_object_send(fs_net_connection_t *conn, fs_gd_cmd_t cmd, svalue_t *key, object_t *obj);
int fs_db_object_load(fs_net_connection_t *conn, svalue_t *k, int sync, int sid);
int fs_db_object_unload(fs_net_connection_t *conn, svalue_t *k);

int db_object_enable_dirty(object_t *obj);
int db_object_marshal_dirty(object_t *obj, fs_dbi_array_t *arr);
void merge_dirty_db_object_to_map(svalue_t *root, fs_dbi_array_iter_t *iter);

typedef struct {
	svalue_t svname;
	unsigned index;
} fs_db_object_pair_t;

typedef struct {
	program_t *prog;
	unsigned cnt;
	fs_db_object_pair_t *pairs;
} fs_db_object_index_t;

fs_db_object_index_t *fs_db_object_index_find_or_add(object_t *obj);
int fs_db_object_save(fs_net_connection_t *conn, fs_gd_cmd_t cmd, svalue_t *k, object_t *obj, fs_db_object_index_t *index);
int fs_db_object_rename(fs_net_connection_t *conn, svalue_t *k, object_t *obj, svalue_t *newk);

void fs_db_object_module_init();
void fs_db_object_module_exit();
void fs_db_object_module_update();

#endif /*_FS_DATA_SAVE_H*/
