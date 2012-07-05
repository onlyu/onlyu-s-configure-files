
#ifndef __COMMON_H__
#define __COMMON_H__

#include "fs_atom.h"
#include "lpc_args_op.h"


// item取值范围 8亿
#define	ITEM_START_ID		0x50000000
#define	ITEM_END_ID			0x7FFFFFFF
#define ITEM_DIR            "char/item/"

// 战斗中的NPC ID数量

// summon 2.68亿
#define	SUMMON_START_ID		0x40000000
#define	SUMMON_END_ID		0x50000000
#define SUMMON_PATH         "char/summon/base_summon.c"

// npc 5.3亿
#define	NPC_START_ID		0x20000000
#define	NPC_END_ID		    0x40000000

#define MAX_ONLINE          8000
#define MAX_ITEM            (MAX_ONLINE * 150)
#define MAX_SUMMON          (MAX_ONLINE * 15)
#define MAX_NPC             (MAX_ONLINE * 30)
#define MAX_TEAM            MAX_ONLINE

#define IS_ITEM(x) (x >= ITEM_START_ID && x <= ITEM_END_ID)

/*从系统原子表查找或者生成某串对应的原子*/
#define SYS_GET_STR_ATOM(x)     fs_atom_get_string_atom(x)
/*从系统原子表找到某串对应的原子*/
#define SYS_FIND_STR_ATOM(x)    fs_atom_find_string_atom(x)

#define TEMP_VAR   1
#define PERM_VAR   0 

#define FS_GBL_KEY_LEN    12
#define FS_MAX_PATH_LEN   1024
#define FS_NAME_LEN       32
#define FS_TITLE_LEN      32

#endif
