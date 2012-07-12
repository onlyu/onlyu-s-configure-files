// Purpose  : 物品，召唤兽等char类型的管理机制
// Created  : 2010-05-24
// By       : Mouse
// History  : 2010-05-24  Mouse create
//
// 说明 : 
//       1. 本模块主要做了可动态更新item的功能. 
//       2. 将一个对象中需要保存的变量存储下来.
//       3. 将物品, 召唤兽, NPC统一一个模块管理(2010-06-18)
//        
// =======================================================================
// new char ob 管理
#include "fs_char_ob.h"
#include "fs_lpc_op.h"
#include "fs_log.h"
#include "fs_common.h"
#include "contrib.h"

fs_char_ob_manager_t gpCharObManager[T_CHAR_MAX];
#define IS_TYPE_VALID(t) (t < T_CHAR_MAX && t >= 0)

void fs_init_char_ob(int type, int hsize, int entsize, int startid, int endid)
{
	fs_char_ob_manager_t *obm;

	if (!IS_TYPE_VALID(type)) {
		log_error(MOD_CHAR_OB, "char type[%d] not exist!", type);
		return;
	}

	obm = &gpCharObManager[type];

	obm->ppool = fs_create_memory_pool(entsize + sizeof(fs_char_ob_t), hsize);
	obm->phtbl = fs_ht_create_hash_table(hsize);
	obm->startid = startid;
	obm->endid = endid;
	obm->curob = NULL;
	obm->binit = 1;
}

void fs_dump_char_ob()
{
	fs_char_ob_manager_t *obm;

	obm = &gpCharObManager[T_CHAR_ITEM];
	fs_dump_mem_pool_desc(obm->ppool, "item");

	obm = &gpCharObManager[T_CHAR_BEAST];
	fs_dump_mem_pool_desc(obm->ppool, "summon");

	obm = &gpCharObManager[T_CHAR_NPC];
	fs_dump_mem_pool_desc(obm->ppool, "npc");
}

fs_char_ob_t *fs_new_char_ob(int type, char *path, mapping_t *m)
{
	fs_char_ob_t * cob;
	fs_lpc_object_t *oob;
	fs_char_ob_manager_t *obm; 

	oob = fs_find_lpc_object(path);
	if (!oob) {
		log_error(MOD_CHAR_OB, "char path[%d %s] not exist!", type, path);
		return NULL;
	}

	if (!IS_TYPE_VALID(type)) {
		log_error(MOD_CHAR_OB, "char type[%d] not exist!", type);
		return NULL;
	}
	obm = &gpCharObManager[type];

	cob = (fs_char_ob_t *) fs_malloc_node(obm->ppool);
	if (!cob) {
		return NULL;
	}
	cob->id = ++obm->startid;
	cob->oob = oob;
	cob->save_map = NULL;
	cob->temp_map = NULL;

	FS_HT_INSERT_INT(obm->phtbl, cob->id, cob);

	if (m) {
		cob->save_map = m;
		add_ref(m, "fs_new_char_ob");
		// TODO:
	} else {
		svalue_t *v;
		v = call_object_func(oob->ob, "SaveMap", "");
		if ( v && IS_LPC_MAP(v) ) {
			cob->save_map = deep_copy_mapping(LPC_MAP_VALUE(v));
		} else {
			cob->save_map = allocate_mapping(0);
		}
	}
	return cob;
}

inline fs_char_ob_t *fs_find_char_ob(int type, int id)
{
	fs_char_ob_manager_t *obm; 
	if (!IS_TYPE_VALID(type)) {
		log_error(MOD_CHAR_OB, "char type[%d,%d] can't find!", type, id);
		return NULL;
	}
	obm = &gpCharObManager[type];

	if (obm->curob && (obm->curob->id == id)) {
		return obm->curob;
	}
	obm->curob = FS_HT_FIND_INT(obm->phtbl, id);
	return obm->curob;
}

void fs_free_char_ob(int type, int id)
{
	fs_char_ob_manager_t *obm; 
	fs_char_ob_t *cob;
	if (!IS_TYPE_VALID(type)) {
		log_error(MOD_CHAR_OB, "char type[%d,%d] can't free!", type, id);
		return;
	}
	obm = &gpCharObManager[type];
	if ((cob = FS_HT_FIND_INT(obm->phtbl, id))) {
		if (cob->save_map) {
			free_mapping(cob->save_map);
		}
		if (cob->temp_map) {
			free_mapping(cob->temp_map);
		}
		FS_HT_REMOVE_INT(obm->phtbl, id);
		fs_free_node(obm->ppool, cob);
		if (obm->curob && (obm->curob->id == id)) {
			obm->curob = NULL;
		}
	}
}

#define LPC_VAL_STR_LEN    64
inline void _lpc_svalue_2_str(svalue_t *v, char *buf)
{
	memset(buf, 0, sizeof(buf));
	switch(v->type) 
	{
		case T_NUMBER:
			sprintf(buf, "%d", v->u.number);
			break;
		case T_STRING:
			sprintf(buf, "%s", v->u.string);
		default:
			break;
	}
}

static svalue_t * find_in_save_map_free(fs_char_ob_t *cob, svalue_t *key)
{
	svalue_t *nval;
	fs_lpc_object_t *oob;

	if (!cob->save_map) {
		return NULL;
	}

	oob = cob->oob;
	nval = find_in_mapping(cob->save_map, key);

	if (IS_NULL(nval) || IS_LPC_UNDEF(nval)) {
		if (oob && oob->save_map) {
			nval = find_in_mapping(oob->save_map, key);
			if (IS_LPC_UNDEF(nval)) {
				return NULL;
			}
			nval = find_for_insert(cob->save_map, key, 1);
		} else {
			return NULL;
		}
	}
	else 
	{
		set_dirty_map(cob->save_map, nval-1, DIRTY_SET);
		free_svalue(nval, "find_in_save_map_free");
	}
	return nval;
}

// 设置
void fs_set_temp_map(int type, int id, svalue_t *key, svalue_t *val)
{
	svalue_t *nval;
	fs_char_ob_t *cob;

	cob = fs_find_char_ob(type, id);

	if (!cob) {
		return;
	}
	if (!cob->temp_map) {
		cob->temp_map = allocate_mapping(0);
	}
	nval = find_for_insert(cob->temp_map, key, 1);

	deep_copy_svalue(val, nval);

	// assign_svalue_no_free(nval, val);
}

void fs_set_save_map(int type, int id, svalue_t *key, svalue_t *val)
{
	svalue_t *nval;
	fs_char_ob_t *cob;
	fs_lpc_object_t *oob;
	char buf[LPC_VAL_STR_LEN];

	cob = fs_find_char_ob(type, id);

	if (!cob) {
		return;
	}

	oob = cob->oob;
	nval = find_in_save_map_free(cob, key);

	if (!nval) {
		_lpc_svalue_2_str(key, buf);
		log_error( MOD_ITEM, "[%s]'s saved key should be inited by \"SaveMap\" or \"BaseSaveMap\" func, [%s]", oob->ob ? oob->ob->name : "null", buf);
		return;
	}

	/* 如果为空，或为定义，说明没有初始化 */
	if (IS_NULL(nval)) {
		_lpc_svalue_2_str(key, buf);
		log_error( MOD_ITEM, "[%s]'s saved key should be inited by \"SaveMap\" or \"BaseSaveMap\" func, [%s]", oob->ob ? oob->ob->name : "null", buf);
		return;
	}

	deep_copy_svalue(val, nval);
	/* 赋值 */
	// assign_svalue_no_free(nval, val);
}

void fs_set_map(int type, int id, svalue_t *key, svalue_t *val)
{
	svalue_t *nval;
	fs_char_ob_t *cob;

	cob = fs_find_char_ob(type, id);

	if (!cob) {
		return;
	}

	nval = find_in_save_map_free(cob, key);

	// 如果save找不到，那么找temp
	if (!nval || IS_NULL(nval)) {
		if (!cob->temp_map) {
			cob->temp_map = allocate_mapping(0);
		}
		nval = find_for_insert(cob->temp_map, key, 1);
	}
	/* 赋值 */
	assign_svalue_no_free(nval, val);
}

svalue_t *fs_get_temp_map(int type, int id, svalue_t *key)
{
	fs_char_ob_t *cob;
	svalue_t *val = &const0u;
	
	if (!(cob = fs_find_char_ob(type, id))) {
		return val;
	}

	if (cob->temp_map) {
		val = find_in_mapping(cob->temp_map, key);
	}
	return val;
}

svalue_t *fs_get_save_map(int type, int id, svalue_t *key)
{
	fs_char_ob_t *cob;
	svalue_t *val = &const0u;

	if (!(cob = fs_find_char_ob(type, id))) {
		return val;
	}
	if (cob->save_map) {
		val = find_in_mapping(cob->save_map, key);
	}
	return val;
}
