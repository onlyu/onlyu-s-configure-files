// -------------------  Auto Generate Begin --------------------


#include <item.h>
#include <item_key.h>
#include <macros.h>
#include <equip.h>
#include <common_key.h>
#include <baoshi.h>

inherit EQUIP_BASE;


mapping data = ([
 
	  K_EQUIP_QIANGHUA:6,
	  K_EQUIP_GRADE:30,
	  K_EQUIP_COLOR:EQUIP_COLOR_PURPLE,
	  K_EQUIP_LOCATE:EQUIP_LOCATE_CLOTHES,
	  K_EQUIP_TYPE:EQUIP_TYPE_FANG,
	FT_KANGFA:59, 

 
]);

mapping getData()
{
	return data;
}

 
void InitItem(int itemid) 
{
	SetItemSave(itemid, K_ITEM_TYPE, "EQ02306");

	SetItemTemp(itemid, KT_ITEM_NAME, "麻布帽");
	SetItemTemp(itemid, KT_ITEM_MAXAMOUNT, 1);
	::InitItem(itemid);
}
 

// from tmp/data/物品表.xls
// -------------------  Auto Generate End   --------------------
