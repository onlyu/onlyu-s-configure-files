#ifndef _FS_OB_MANAGER_H_
#define _FS_OB_MANAGER_H_

#include "fs_hlist.h"
#include "fs_mem_pool.h"
#include "fs_lpcob_table.h"

enum {
	T_CHAR_ITEM,
	T_CHAR_BEAST,         /*包括召唤兽，孩子，时间宠等*/
	T_CHAR_NPC,
	T_CHAR_MAX,
};

typedef struct fs_char_ob_s {
	int id;
	fs_lpc_object_t *oob;
	mapping_t *save_map;
	mapping_t *temp_map;
} fs_char_ob_t;

typedef struct fs_char_ob_manager_s {
	fs_memory_pool_t *ppool;
	fs_obj_hash_t *phtbl;
	fs_char_ob_t *curob;
	int startid;
	int endid;
	int binit;
} fs_char_ob_manager_t;

#define CHAR_OB_ENTRY(ob) ((ob) + 1)
#define CHAR_OB_ID(ob) ((ob)->id)
#define CHAR_OB_ORIG_OB(ob) ((ob)->oob)

void fs_init_char_ob(int type, int hsize, int entsize, int startid, int endid);
fs_char_ob_t *fs_find_char_ob(int type, int id);
fs_char_ob_t *fs_new_char_ob(int type, char *path, mapping_t *m);
void fs_free_char_ob(int type, int id);

mapping_t *get_char_map(int type, int id, int saved);

void fs_set_temp_map(int type, int id, svalue_t *key, svalue_t *val);
void fs_set_save_map(int type, int id, svalue_t *key, svalue_t *val);
// 如果save不存在该值，那么设置为临时的
void fs_set_map(int type, int id, svalue_t *key, svalue_t *val);
svalue_t *fs_get_temp_map(int type, int id, svalue_t *key);
svalue_t *fs_get_save_map(int type, int id, svalue_t *key);
void fs_dump_char_ob();
#endif
