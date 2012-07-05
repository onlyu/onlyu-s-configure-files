#include "fs_item.h"
#include "fs_hlist.h"
#include "fs_mem_pool.h"
#include "fs_common.h"
#include "fs_lpc_op.h"
#include "fs_log.h"
#include "fs_char_ob.h"

void fs_sys_init_item_module(int max, int startid, int endid)
{
	fs_init_char_ob(T_CHAR_ITEM, max, sizeof(fs_item_object_t), startid, endid);
}

// 新建物品
int fs_new_item(char *path)
{
	fs_char_ob_t * cob;
	fs_lpc_object_t * oob;

	cob = fs_new_char_ob(T_CHAR_ITEM, path, NULL);

	if (cob) {
		oob = CHAR_OB_ORIG_OB(cob);

		/* 物品初始化接口 */
		call_object_func(oob->ob, "InitItem", "i", CHAR_OB_ID(cob));

		return CHAR_OB_ID(cob);
	}
	return 0;
}

// 根据信息生成物品
// type : 物品type
// m    : 存盘数据
//
// 调用函数不需要free_maping(m)
int fs_restore_item(char *path, mapping_t *m)
{
	fs_char_ob_t * cob;
	fs_lpc_object_t * oob;

	cob = fs_new_char_ob(T_CHAR_ITEM, path, m);

	if (cob) {
		/* 物品初始化接口 */
		oob = CHAR_OB_ORIG_OB(cob);
		call_object_func(oob->ob, "InitItem", "i", CHAR_OB_ID(cob));
		return CHAR_OB_ID(cob);
	}
	return 0;
}

void fs_destroy_item(int itemid)
{
	fs_free_char_ob(T_CHAR_ITEM, itemid);
}

mapping_t *fs_get_item_map(int itemid, int saved)
{
	fs_char_ob_t * cob;
	cob = fs_find_char_ob(T_CHAR_ITEM, itemid);
	if (!cob) {
		return NULL;
	}
	if (saved) {
		return cob->save_map;
	} else {
		return cob->temp_map;
	}
}

inline int is_item_online(int itemid)
{
	if(fs_find_char_ob(T_CHAR_ITEM, itemid)) {
		return 1;
	}
	return 0;
}

//========================================================
// efun

void f_new_item(void)
{
	SET_ALL_ARGS
	int item = fs_new_item(ARGS(0)->u.string);
	POP_ALL_ARGS

	push_number(item);
}

void f_restore_item(void)
{
	SET_ALL_ARGS
	int item = fs_restore_item(ARGS(0)->u.string, ARGS(1)->u.map);
	POP_ALL_ARGS

	push_number(item);
}

//=============================================
//永久变量

#define  SET_PERM_VAR { \
	SET_ALL_ARGS \
	fs_set_save_map(T_CHAR_ITEM, ARGS(0)->u.number, ARGS(1), ARGS(2)); \
	POP_ALL_ARGS \
}

#define GET_PERM_VAR { \
	SET_ALL_ARGS \
	svalue_t *v = fs_get_save_map(T_CHAR_ITEM, ARGS(0)->u.number, ARGS(1)); \
	POP_ALL_ARGS \
	push_svalue(v); \
}

#define  SET_TEMP_VAR { \
	SET_ALL_ARGS \
	fs_set_temp_map(T_CHAR_ITEM, ARGS(0)->u.number, ARGS(1), ARGS(2)); \
	POP_ALL_ARGS \
}

#define GET_TEMP_VAR { \
	SET_ALL_ARGS \
	svalue_t *v = fs_get_temp_map(T_CHAR_ITEM, ARGS(0)->u.number, ARGS(1)); \
	POP_ALL_ARGS \
	push_svalue(v); \
}

#define  SET_VAR { \
	SET_ALL_ARGS \
	fs_set_map(T_CHAR_ITEM, ARGS(0)->u.number, ARGS(1), ARGS(2)); \
	POP_ALL_ARGS \
}

#define GET_VAR { \
	SET_ALL_ARGS \
	svalue_t *v = fs_get_save_map(T_CHAR_ITEM, ARGS(0)->u.number, ARGS(1)); \
	if (IS_NULL(v) || IS_LPC_UNDEF(v)) { \
		v = fs_get_temp_map(T_CHAR_ITEM, ARGS(0)->u.number, ARGS(1)); \
	} \
	POP_ALL_ARGS \
	push_svalue(v); \
}


void f_SetItemSave(void)
{
	SET_PERM_VAR
}

void f_GetItemSave(void)
{
	GET_PERM_VAR
}


// =================================
// temp value
void f_SetItemTemp(void)
{
	SET_TEMP_VAR
}

void f_GetItemTemp(void)
{
	GET_TEMP_VAR
}

// =================================
// temp value
void f_SetItem(void)
{
	SET_VAR
}

void f_GetItem(void)
{
	GET_VAR
}

// ==============================================
// gblkey
void f_destroy_item(void)
{
	SET_ALL_ARGS
	fs_destroy_item(ARGS(0)->u.number);
	POP_ALL_ARGS
}

void f_is_item_online(void)
{
	SET_ALL_ARGS
	int ret = is_item_online(ARGS(0)->u.number);
	POP_ALL_ARGS
	push_number(ret);
}

// test
void f_get_item_map(void)
{
	SET_ALL_ARGS
	mapping_t *m = fs_get_item_map(ARGS(0)->u.number, ARGS(1)->u.number);
	POP_ALL_ARGS

	if (!m) {
		push_svalue(&const0u);
		return;
	}
	push_mapping(m);
}

