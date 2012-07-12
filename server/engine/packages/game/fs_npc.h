#ifndef _FS_npc_H_
#define _FS_npc_H_

#include "fs_char_ob.h"
#include "fs_common.h"
#include "fs_prop_key.h"

// NPC��AOI����
#if 0
typedef struct fs_aoi_area_s {
	int r;                                    /*�뾶, �Ժ������Ӳ�ͬ��״*/
} fs_aoi_area_t;
#endif

typedef struct fs_npc_object_s {
} fs_npc_object_t;


// ��ʼ��npc module
void fs_sys_init_npc_module(int max, int startid, int endid);
// �½�һ��NPC
int fs_new_npc(char *path);
// ����һ��NPC
void fs_destroy_npc(int npcid);
// ȡNPC���ϵı���
svalue_t * get_npc_prop(int npcid, svalue_t *key);
// ��ȡLPC ob
object_t *get_npc_lpcob(int npcid);
#endif
