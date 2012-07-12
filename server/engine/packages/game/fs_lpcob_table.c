// Purpose  : 物品，召唤兽等char类型的对象管理机制
// Created  : 2010-05-24
// By       : Mouse
// History  : 2010-05-24  Mouse create
//
// 说明 : 
//       1. 本模块主要做了可动态更新item的功能. 
//       2. 将一个对象中需要保存的变量存储下来.
//        

#include "fs_lpcob_table.h"
#include "fs_atom.h"
#include "fs_lpc_op.h"
#include "fs_log.h"
#include "fs_common.h"
#include "contrib.h"

#include "fs_rpc_lpc.h"
#include "fs_user.h"
#include "fs_data_save.h"

// s->string
#define StrHash(s)     (whashstr((s), 20))
#define CHAR_OB_HASH_SIZE           2000

static int bInited = 0;
static fs_char_ob_htbl_t *gpCharObTbl;

// =======================================================================
// orig char ob 的管理
static fs_lpc_object_t* NEW_CHAR_OBJECT()
{
	fs_lpc_object_t * cob = calloc(1, sizeof(fs_lpc_object_t));
		//(fs_lpc_object_t*) MALLOC(sizeof(fs_lpc_object_t));
	//memset(cob, 0, sizeof(fs_lpc_object_t));
	return cob;
}

/*
   // TODO:
static void FREE_CHAR_OBJECT(fs_lpc_object_t *ob)
{
	FREE(ob);
}
*/

inline static void _init_lpcob_table(int hsize)
{
	int i;
	int hashsize = 1;
	for (; hashsize < hsize; hashsize *= 2) ;

	gpCharObTbl = (fs_char_ob_htbl_t *) malloc(sizeof(fs_char_ob_htbl_t));

	gpCharObTbl->htbl = (fs_lpc_object_t **)malloc(hashsize * sizeof(fs_lpc_object_t *));

	for (i = 0; i < hashsize; i++) {
		gpCharObTbl->htbl[i] = NULL;
	}
	gpCharObTbl->hsize = hashsize;
	gpCharObTbl->num = 0;
}

inline static fs_lpc_object_t* _find_lpc_object(object_t* ob, int h)
{
	fs_lpc_object_t *cur;
	int hash = h & (gpCharObTbl->hsize - 1);

	cur = gpCharObTbl->htbl[hash];

	while (cur) {
		if ( ob == cur->ob \
				|| (strlen(cur->ob->name) == strlen(ob->name) \
				&& !strcmp(cur->ob->name, ob->name)) ) {
			return cur;
		}

		if (cur->ob->flags & O_DESTRUCTED) {
			// 如果当前update的ob, 则在上面条件中已经return, 否则则有没有清除的ob
			log_error( MOD_CHAR_OB, "ob is destroy: %s", cur->ob->name);
		}
		cur = cur->next;
	}
	return NULL;
}

inline static void _insert_lpc_object(fs_lpc_object_t* cob, int h)
{
	int hash = h & (gpCharObTbl->hsize - 1);

	if (!gpCharObTbl->htbl[hash]) {
		gpCharObTbl->htbl[hash] = cob;
	} else {
		cob->next = gpCharObTbl->htbl[hash];
		gpCharObTbl->htbl[hash] = cob;
	}
}

inline static void _init_lpc_object(fs_lpc_object_t *cob, object_t* ob)
{
	svalue_t *v;

	cob->ob   = ob;

	/* 先清掉原来的数据 */
	if (cob->save_map) {
		free_mapping(cob->save_map);
		cob->save_map = NULL;
	}

	/* 初始化存盘变量 */
	v = call_object_func(ob, "BaseSaveMap", "");
	if ( v && IS_LPC_MAP(v) ) {
		cob->save_map = deep_copy_mapping(LPC_MAP_VALUE(v));
	}

	v = call_object_func(ob, "SaveMap", "");
	if ( v && IS_LPC_MAP(v) ) {
		if (cob->save_map) {
			cob->save_map = add_mapping(cob->save_map, LPC_MAP_VALUE(v));
		} else {
			cob->save_map = deep_copy_mapping(LPC_MAP_VALUE(v));
		}
	}
}

inline fs_lpc_object_t* fs_find_lpc_object(char *path)
{
	object_t *ob = NULL;
	fs_lpc_object_t *cob;
	int h;

	if (!bInited)
	{
		_init_lpcob_table(CHAR_OB_HASH_SIZE);
		bInited = 1;
	}

	ob = find_object(path);
	if (ob == NULL) {
		log_error( MOD_CHAR_OB, "can't find ob: %s", path );
		return NULL;
	}

	h = StrHash(ob->name);
	cob = _find_lpc_object(ob, h);

	if (!cob) {
		cob = NEW_CHAR_OBJECT();
		_init_lpc_object(cob, ob);
		_insert_lpc_object(cob, h);
	}
	return cob;
}

// 声明
void fs_update_lpc_object(object_t *ob)
{
	fs_lpc_object_t *cob;

	if (!bInited || !ob || (ob->flags & O_DESTRUCTED))
	{
		return;
	}

	fs_user_object_update( ob );
	//fs_dat_object_update(ob);
	fs_rpc_lpc_module_update(ob);
	fs_db_object_module_update(ob);

	cob = _find_lpc_object(ob, StrHash(ob->name));

	if (cob) {
		_init_lpc_object(cob, ob);
	}
}
// =======================================================================


void f_update_lpc_object(void)
{
	SET_ALL_ARGS
	fs_update_lpc_object(ARGS(0)->u.ob);
	POP_ALL_ARGS
}

