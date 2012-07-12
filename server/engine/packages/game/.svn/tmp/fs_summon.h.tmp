#ifndef _FS_SUMMON_H_
#define _FS_SUMMON_H_

#include "fs_char_ob.h"
#include "fs_common.h"

enum {
	T_CHAR_NULL,
	T_CHAR_SUMMON,
	T_CHAR_BABY,
};

typedef struct fs_summon_object_s {
	// summon需要定制的东西
	int type;                  /*召唤兽，孩子，时间宠等*/
} fs_summon_object_t;

// 初始化summon module
void fs_sys_init_summon_module(int max, int startid, int endid);
// 新建一个召唤兽
int fs_new_summon(char *path);
// 生成一个召唤兽
int fs_generate_summon(mapping_t *m);
// 销毁一个召唤兽
void fs_destroy_summon(int summon);

int is_summon_online(int summid);

svalue_t *fs_get_summon_var(int summid, svalue_t *key, int saved);
#endif
