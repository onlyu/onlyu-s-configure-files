// -------------------  Auto Generate Begin --------------------


#include <item.h>
#include <item_key.h>
#include <macros.h>
#include <equip.h>
#include <common_key.h>
#include <baoshi.h>

inherit EQUIP_BAOSHI_BASE;


mapping data = ([
 
	  K_BAOSHI_GRADE:6,
	  K_BAOSHI_TYPE:ZHILI,
	  K_BAOSHI_ATTR:([ZHILI_RATE:8, ]),

 
]);

mapping getData()
{
	return data;
}

 
void InitItem(int itemid) 
{
	SetItemSave(itemid, K_ITEM_TYPE, "BS20006");

	SetItemTemp(itemid, KT_ITEM_NAME, "6级智力宝石");
	SetItemTemp(itemid, KT_ITEM_MAXAMOUNT, 100);
	::InitItem(itemid);
}
 

// from tmp/data/物品表.xls
// -------------------  Auto Generate End   --------------------
