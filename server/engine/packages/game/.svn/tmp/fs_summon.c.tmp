#include "fs_summon.h"
#include "fs_hlist.h"
#include "fs_mem_pool.h"
#include "fs_common.h"
#include "fs_lpc_op.h"
#include "fs_log.h"

void fs_sys_init_summon_module(int max, int startid, int endid)
{
	fs_init_char_ob(T_CHAR_BEAST, max, sizeof(fs_summon_object_t), startid, endid);
}

// 新建召唤兽
int fs_new_summon(char *path)
{
	fs_char_ob_t * cob;
	fs_lpc_object_t * oob;
	fs_summon_object_t * sob;

	cob = fs_new_char_ob(T_CHAR_BEAST, path, NULL);

	if (cob) {
		oob = CHAR_OB_ORIG_OB(cob);
		sob = (fs_summon_object_t *) CHAR_OB_ENTRY(cob);
		sob->type = T_CHAR_SUMMON;
		/* 物品初始化接口 */
		call_object_func(oob->ob, "InitSummon", "i", CHAR_OB_ID(cob));
		return CHAR_OB_ID(cob);
	}
	return 0;
}

// 根据信息生成召唤兽
// type : 物品type
// m    : 存盘数据
//
// 调用函数不需要free_maping(m)
int fs_restore_summon(char *path, mapping_t *m)
{
	fs_char_ob_t * cob;
	fs_lpc_object_t * oob;

	cob = fs_new_char_ob(T_CHAR_BEAST, path, m);

	if (cob) {
		/* 物品初始化接口 */
		oob = CHAR_OB_ORIG_OB(cob);
		call_object_func(oob->ob, "InitSummon", "i", CHAR_OB_ID(cob));
		return CHAR_OB_ID(cob);
	}
	return 0;
}

void fs_destroy_summon(int summon)
{
	fs_free_char_ob(T_CHAR_BEAST, summon);
}


mapping_t *fs_get_summon_map(int summid, int saved)
{
	fs_char_ob_t * cob;
	cob = fs_find_char_ob(T_CHAR_BEAST, summid);
	if (!cob) {
		return NULL;
	}
	if (saved) {
		return cob->save_map;
	} else {
		if( !cob->temp_map )
		{
			cob->temp_map = allocate_mapping(0);
		}

		return cob->temp_map;
	}
}

svalue_t *fs_get_summon_var(int summid, svalue_t *key, int saved)
{
	mapping_t *data_map;
	data_map = fs_get_summon_map(summid, saved);
	svalue_t *v;

	if (!data_map) {
		return NULL;
	}

	v = find_in_mapping(data_map, key);
	if (IS_NULL(v) || IS_LPC_UNDEF(v)) {
		return NULL;
	}
	return v;
}

int is_summon_online(int summid)
{
	fs_char_ob_t *cob;
	cob = fs_find_char_ob(T_CHAR_BEAST, summid);
	if (cob) {
		return 1;
	}
	return 0;
}


//========================================================
// efun

#define  SET_PERM_VAR { \
	SET_ALL_ARGS \
	fs_set_save_map(T_CHAR_BEAST, ARGS(0)->u.number, ARGS(1), ARGS(2)); \
	POP_ALL_ARGS \
}

#define GET_PERM_VAR { \
	SET_ALL_ARGS \
	svalue_t *v = fs_get_save_map(T_CHAR_BEAST, ARGS(0)->u.number, ARGS(1)); \
	POP_ALL_ARGS \
	push_svalue(v); \
}

#define  SET_TEMP_VAR { \
	SET_ALL_ARGS \
	fs_set_temp_map(T_CHAR_BEAST, ARGS(0)->u.number, ARGS(1), ARGS(2)); \
	POP_ALL_ARGS \
}

#define GET_TEMP_VAR { \
	SET_ALL_ARGS \
	svalue_t *v = fs_get_temp_map(T_CHAR_BEAST, ARGS(0)->u.number, ARGS(1)); \
	POP_ALL_ARGS \
	push_svalue(v); \
}

#define  SET_VAR { \
	SET_ALL_ARGS \
	fs_set_map(T_CHAR_BEAST, ARGS(0)->u.number, ARGS(1), ARGS(2)); \
	POP_ALL_ARGS \
}

#define GET_VAR { \
	SET_ALL_ARGS \
	svalue_t *v = fs_get_save_map(T_CHAR_BEAST, ARGS(0)->u.number, ARGS(1)); \
	if (IS_NULL(v) || IS_LPC_UNDEF(v)) { \
		v = fs_get_temp_map(T_CHAR_BEAST, ARGS(0)->u.number, ARGS(1)); \
	} \
	POP_ALL_ARGS \
	push_svalue(v); \
}

void f_new_summon(void)
{
	SET_ALL_ARGS
	int summon = fs_new_summon(ARGS(0)->u.string);
	POP_ALL_ARGS

	push_number(summon);
}

void f_restore_summon(void)
{
	SET_ALL_ARGS
	int summon = fs_restore_summon(ARGS(0)->u.string, ARGS(1)->u.map);
	POP_ALL_ARGS

	push_number(summon);
}

void f_SetSummon(void)
{
	SET_VAR
}

void f_GetSummon(void)
{
	GET_VAR
}

void f_SetSummonTemp(void)
{
	SET_TEMP_VAR
}

void f_GetSummonTemp(void)
{
	GET_TEMP_VAR
}


void f_SetSummonSave(void)
{
	SET_PERM_VAR
}

void f_GetSummonSave(void)
{
	GET_PERM_VAR
}

void f_destroy_summon(void)
{
	SET_ALL_ARGS
	fs_destroy_summon(ARGS(0)->u.number);
	POP_ALL_ARGS
}

void f_is_summon_online(void)
{
	SET_ALL_ARGS
	int ret = is_summon_online(ARGS(0)->u.number);
	POP_ALL_ARGS
	push_number(ret);
}


void f_get_summon_map(void)
{
	SET_ALL_ARGS
	mapping_t *m = fs_get_summon_map(ARGS(0)->u.number, ARGS(1)->u.number);
	POP_ALL_ARGS
	if (!m) {
		push_svalue(&const0u);
		return;
	}
	push_mapping(m);
}
