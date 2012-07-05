// -------------------  Auto Generate Begin --------------------


#include <item.h>
#include <item_key.h>
#include <macros.h>
#include <equip.h>
#include <common_key.h>
#include <baoshi.h>

inherit EQUIP_TUPU_BASE;


mapping data = ([
 
	  K_NEED_H_LEV_CAILIAO:({"CL20901", }),
	  K_NEED_TUPU:"EQ02801",
	  K_NEED_L_LEV_CAILIAO:({"CL10901", 100, }),
	  K_NEED_CASH:1600,

 
]);

mapping getData()
{
	return data;
}

 
void InitItem(int itemid) 
{
	SetItemSave(itemid, K_ITEM_TYPE, "TP00901");

	::InitItem(itemid);
}
 

// from tmp/data/物品表.xls
// -------------------  Auto Generate End   --------------------
