#ifndef _FS_npc_H_
#define _FS_npc_H_

#include "fs_char_ob.h"
#include "fs_common.h"
#include "fs_prop_key.h"

// NPC的AOI区域
#if 0
typedef struct fs_aoi_area_s {
	int r;                                    /*半径, 以后可能添加不同形状*/
} fs_aoi_area_t;
#endif

typedef struct fs_npc_object_s {
} fs_npc_object_t;


// 初始化npc module
void fs_sys_init_npc_module(int max, int startid, int endid);
// 新建一个NPC
int fs_new_npc(char *path);
// 销毁一个NPC
void fs_destroy_npc(int npcid);
// 取NPC身上的变量
svalue_t * get_npc_prop(int npcid, svalue_t *key);
// 获取LPC ob
object_t *get_npc_lpcob(int npcid);
#endif
