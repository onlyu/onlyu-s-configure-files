// -------------------  Auto Generate Begin --------------------


#include <item.h>
#include <item_key.h>
#include <macros.h>
#include <equip.h>
#include <common_key.h>
#include <baoshi.h>

inherit EQUIP_TUPU_BASE;


mapping data = ([
 
	  K_NEED_H_LEV_CAILIAO:({"CL20504", }),
	  K_NEED_TUPU:"EQ02504",
	  K_NEED_L_LEV_CAILIAO:({"CL10501", 100, }),
	  K_NEED_CASH:200,

 
]);

mapping getData()
{
	return data;
}

 
void InitItem(int itemid) 
{
	SetItemSave(itemid, K_ITEM_TYPE, "TP00504");

	::InitItem(itemid);
}
 

// from tmp/data/物品表.xls
// -------------------  Auto Generate End   --------------------
