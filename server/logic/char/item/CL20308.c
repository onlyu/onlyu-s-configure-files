// -------------------  Auto Generate Begin --------------------


#include <item.h>
#include <item_key.h>
#include <macros.h>
#include <equip.h>
#include <common_key.h>
#include <baoshi.h>

inherit EQUIP_CAILIAO_BASE;


mapping data = ([
 

 
]);

mapping getData()
{
	return data;
}

 
void InitItem(int itemid) 
{
	SetItemSave(itemid, K_ITEM_TYPE, "CL20308");

	SetItemTemp(itemid, KT_ITEM_NAME, "30精英鞋子材料");
	SetItemTemp(itemid, KT_ITEM_MAXAMOUNT, 100);
	::InitItem(itemid);
}
 

// from tmp/data/物品表.xls
// -------------------  Auto Generate End   --------------------
