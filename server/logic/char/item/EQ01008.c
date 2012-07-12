// -------------------  Auto Generate Begin --------------------


#include <item.h>
#include <item_key.h>
#include <macros.h>
#include <equip.h>
#include <common_key.h>
#include <baoshi.h>

inherit EQUIP_BASE;


mapping data = ([
 
	  K_EQUIP_QIANGHUA:2,
	FT_SP:1, 
	  K_EQUIP_TYPE:EQUIP_TYPE_FANG,
	  K_EQUIP_LOCATE:EQUIP_LOCATE_SHOES,
	  K_EQUIP_COLOR:EQUIP_COLOR_GREEN,

 
]);

mapping getData()
{
	return data;
}

 
void InitItem(int itemid) 
{
	SetItemSave(itemid, K_ITEM_TYPE, "EQ01008");

	SetItemTemp(itemid, KT_ITEM_NAME, "兽皮靴");
	SetItemTemp(itemid, KT_ITEM_MAXAMOUNT, 1);
	::InitItem(itemid);
}
 

// from tmp/data/物品表.xls
// -------------------  Auto Generate End   --------------------
