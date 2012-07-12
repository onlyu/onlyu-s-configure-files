// -------------------  Auto Generate Begin --------------------


#include <item.h>
#include <item_key.h>
#include <macros.h>
#include <equip.h>
#include <common_key.h>
#include <baoshi.h>

inherit EQUIP_BASE;


mapping data = ([
 
	  K_EQUIP_QIANGHUA:38,
	  K_EQUIP_GRADE:70,
	  K_EQUIP_COLOR:EQUIP_COLOR_PURPLE,
	  K_EQUIP_LOCATE:EQUIP_LOCATE_WEAPON,
	FT_QIANGFA:1326, 
	  K_EQUIP_TYPE:EQUIP_TYPE_HUAN,

 
]);

mapping getData()
{
	return data;
}

 
void InitItem(int itemid) 
{
	SetItemSave(itemid, K_ITEM_TYPE, "EQ02703");

	SetItemTemp(itemid, KT_ITEM_NAME, "檀木扇");
	SetItemTemp(itemid, KT_ITEM_MAXAMOUNT, 1);
	::InitItem(itemid);
}
 

// from tmp/data/物品表.xls
// -------------------  Auto Generate End   --------------------
