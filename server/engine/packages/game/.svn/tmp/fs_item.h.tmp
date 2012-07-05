#ifndef _FS_ITEM_H_
#define _FS_ITEM_H_

#include "fs_char_ob.h"
#include "fs_common.h"
#include "fs_prop_key.h"

typedef struct fs_item_object_s {
	// item需要定制的东西, 现在没有！
} fs_item_object_t;

// 初始化item module
void fs_sys_init_item_module(int max, int startid, int endid);
// 新建一个物品
int fs_new_item(char *type);
// 生成一个物品
int fs_generate_item(mapping_t *m);
// 销毁一个物品
void fs_destroy_item(int itemid);

inline int is_item_online(int itemid);
#endif
