#include "fs_npc.h"
#include "fs_hlist.h"
#include "fs_mem_pool.h"
#include "fs_common.h"
#include "fs_lpc_op.h"
#include "fs_log.h"
#include "fs_char_ob.h"
#include "fs_rpc_c.h"

void fs_sys_init_npc_module(int max, int startid, int endid)
{
	fs_init_char_ob(T_CHAR_NPC, max, sizeof(fs_npc_object_t), startid, endid);
}

inline void _init_npc(fs_npc_object_t *nob, int npcid)
{
	/*
	nob->to_x = 0;
	nob->to_y = 0;
	nob->to_z = 0;
	*/
}

// 新建物品
int fs_new_npc(char *path)
{
	fs_char_ob_t *cob;
	fs_lpc_object_t *oob;

	cob = fs_new_char_ob(T_CHAR_NPC, path, NULL);

	if (cob) {
		oob = CHAR_OB_ORIG_OB(cob);
		// 初始化
		_init_npc((fs_npc_object_t *) CHAR_OB_ENTRY(cob), CHAR_OB_ID(cob));
		/* 物品初始化接口 */
		call_object_func(oob->ob, "initNpc", "i", CHAR_OB_ID(cob));

		return CHAR_OB_ID(cob);
	}
	return 0;
}

// 根据信息生成物品
// type : 物品type
// m    : 存盘数据
//
// 调用函数不需要free_maping(m)
int fs_restore_npc(char *path, mapping_t *m)
{
	fs_char_ob_t * cob;
	fs_lpc_object_t * oob;

	cob = fs_new_char_ob(T_CHAR_NPC, path, m);

	if (cob) {
		/* 物品初始化接口 */
		oob = CHAR_OB_ORIG_OB(cob);
		_init_npc((fs_npc_object_t *) CHAR_OB_ENTRY(cob), CHAR_OB_ID(cob));
		call_object_func(oob->ob, "initNpc", "i", CHAR_OB_ID(cob));
		return CHAR_OB_ID(cob);
	}
	return 0;
}

void fs_destroy_npc(int npcid)
{
	fs_free_char_ob(T_CHAR_NPC, npcid);
}

inline fs_npc_object_t *fs_npc_object(int npcid)
{
	fs_char_ob_t *cob;

	cob = fs_find_char_ob(T_CHAR_NPC, npcid);
	if (cob) {
		return (fs_npc_object_t *) CHAR_OB_ENTRY(cob);
	}
	return NULL;
}

int is_npc_online(int npcid)
{
	fs_char_ob_t *cob;
	cob = fs_find_char_ob(T_CHAR_NPC, npcid);
	if (cob) {
		return 1;
	}
	return 0;
}

object_t *get_npc_lpcob(int npcid)
{
	fs_char_ob_t *cob;

	cob = fs_find_char_ob(T_CHAR_NPC, npcid);
	if (cob) {
		return ((fs_lpc_object_t *) CHAR_OB_ORIG_OB(cob))->ob;
	}
	return NULL;

}

// 取NPC身上的变量
svalue_t * get_npc_prop(int npcid, svalue_t *key)
{
	return fs_get_temp_map(T_CHAR_NPC, npcid, key);
}

mapping_t *fs_get_npc_map(int npcid, int saved)
{
	fs_char_ob_t * cob;
	cob = fs_find_char_ob(T_CHAR_NPC, npcid);
	if (!cob) {
		return NULL;
	}
	if (saved) {
		return cob->save_map;
	} else {
		return cob->temp_map;
	}
}

void fs_set_npc_map(int npcid, mapping_t *m, int saved)
{
	fs_char_ob_t * cob;
	cob = fs_find_char_ob(T_CHAR_NPC, npcid);
	if (!cob) {
		return;
	}
	if (saved) {
		if (cob->save_map) {
			free_mapping(cob->save_map);
		}
		cob->save_map = m;
		add_ref(m, "set npc map");
	} else {
		if (cob->temp_map) {
			free_mapping(cob->temp_map);
		}
		cob->temp_map = m;
		add_ref(m, "set npc map");
	}
}

//========================================================
// efun

#define  SET_PERM_VAR { \
	SET_ALL_ARGS \
	fs_set_save_map(T_CHAR_NPC, ARGS(0)->u.number, ARGS(1), ARGS(2)); \
	POP_ALL_ARGS \
}

#define GET_PERM_VAR { \
	SET_ALL_ARGS \
	svalue_t *v = fs_get_save_map(T_CHAR_NPC, ARGS(0)->u.number, ARGS(1)); \
	POP_ALL_ARGS \
	push_svalue(v); \
}

#define  SET_TEMP_VAR { \
	SET_ALL_ARGS \
	fs_set_temp_map(T_CHAR_NPC, ARGS(0)->u.number, ARGS(1), ARGS(2)); \
	POP_ALL_ARGS \
}

#define GET_TEMP_VAR { \
	SET_ALL_ARGS \
	svalue_t *v = fs_get_temp_map(T_CHAR_NPC, ARGS(0)->u.number, ARGS(1)); \
	POP_ALL_ARGS \
	push_svalue(v); \
}

void f_new_npc(void)
{
	SET_ALL_ARGS
	int npcid = fs_new_npc(ARGS(0)->u.string);
	POP_ALL_ARGS

	push_number(npcid);
}

void f_restore_npc(void)
{
	SET_ALL_ARGS
	int npc = fs_restore_npc(ARGS(0)->u.string, ARGS(1)->u.map);
	POP_ALL_ARGS

	push_number(npc);
}

void f_SetNpc(void)
{
	SET_TEMP_VAR
}

void f_GetNpc(void)
{
	GET_TEMP_VAR
}

void f_SetNpcSave(void)
{
	SET_PERM_VAR
}

void f_GetNpcSave(void)
{
	GET_PERM_VAR
}


void f_destroy_npc(void)
{
	SET_ALL_ARGS
	fs_destroy_npc(ARGS(0)->u.number);
	POP_ALL_ARGS
}

// ========================================================
// NPC aoi事件

void f_is_npc_online(void)
{
	SET_ALL_ARGS
	int ret = is_npc_online(ARGS(0)->u.number);
	POP_ALL_ARGS
	push_number(ret);
}

// test
void f_get_npc_map(void)
{
	SET_ALL_ARGS
	mapping_t *m = fs_get_npc_map(ARGS(0)->u.number, ARGS(1)->u.number);
	POP_ALL_ARGS

	if (!m) {
		push_svalue(&const0u);
		return;
	}
	push_mapping(m);
}


void f_set_npc_map(void)
{
	SET_ALL_ARGS
	fs_set_npc_map(ARGS(0)->u.number, ARGS(1)->u.map, ARGS(2)->u.number);
	POP_ALL_ARGS
}

void f_get_npc_object(void)
{
	SET_ALL_ARGS
	fs_char_ob_t *cob;
	cob = fs_find_char_ob(T_CHAR_NPC, ARGS(0)->u.number);
	POP_ALL_ARGS
	if (cob && cob->oob && cob->oob->ob) {
		push_object(cob->oob->ob);
	} else {
		push_svalue(&const0u);
	}
}

