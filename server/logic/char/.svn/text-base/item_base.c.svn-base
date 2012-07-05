
#include <item_key.h>
#include <macros.h>
#include <debug.h>
#include <common_key.h>

// Item Base
static mapping baseSaveMap = ([
			K_ITEM_TYPE: "",
			KT_ITEM_NAME: "",
			K_ITEM_LOCK: 0,
			K_ITEM_AMOUNT: 1,
			K_ITEM_TASK_OWNER: 0,
			K_GLOBAL_KEY:"",
			K_ITEM_TIMEOUT:0,

			// 装备================================
			K_ITEM_QIANGHUA:0,
			K_ITEM_XILIAN_ATTR:([]),
			K_ITEM_XILIAN_ATTR_REP : ([]),
			K_ITEM_BAOSHI : ([]),
			// 常规属性
			NAILI : 0, 
			LILIANG : 0, 
			ZHILI : 0, 
			MINJIE : 0, 
			// 精英属性
			FT_DODGE : 0, 
			FT_HIT : 0, 
			FT_BAO_JI : 0, 
			FT_KANG_BAO : 0, 
			FT_ZHAO_JIA : 0, 
			FT_PO_JI : 0, 
			FT_YC_HIT : 0, 
			FT_YC_DODGE : 0, 
			FT_FAN_JI : 0, 
			FT_LIANG_JI : 0, 
			FT_BI_SHA : 0, 
			FT_HURT_MORE : 0, 
			FT_HURT_LESS : 0, 
]);

static mapping BaseSaveMap()
{
	return baseSaveMap;
}

static mapping SaveMap()
{
	return ([
			K_ITEM_AMOUNT: 1,
			]);
}

void InitItem(int itemid) 
{
	// null
}

// 默认不能使用
int doUse(object user, int item, int target)
{
	return 0;
}

string itemTips(object user, mapping save_data, mapping temp_data)
{
	return "";
}

int isEquip()
{
	return 0;
}

int isBaoshi()
{
	return 0;
}

void create()
{
}
