
#include <macros.h>
#include <item_key.h>
#include <item.h>

static mapping mpType2ItemDesc = ([

//auto gen begin member

//auto gen end member
		]);

void loadItem(string type)
{
	int itemid = Import("UTIL")->CreateItem(type);
	object oItem = find_object(ITEM_PATH+type, 1);
	mpType2ItemDesc[type] = ([ 
			"name" : GetItemTemp(itemid, KT_ITEM_NAME), 
			"amount": GetItemTemp(itemid, KT_ITEM_MAXAMOUNT),
			"is_equip" : oItem->isEquip(),
			]);
	destroyItem(itemid);
}

string getNameByType(string type)
{
	if (!sizeof(type)) return "";

	if (undefinedp(mpType2ItemDesc[type])) {
		loadItem(type);
	}
	return mpType2ItemDesc[type]["name"];
}

string getClickableName(string type)
{
	return sprintf("#c(item%s)#G[%s]#n#c()", type, getNameByType(type));
}

int getMaxAmountByType(string type)
{
	if (!sizeof(type)) return 0;

	if (undefinedp(mpType2ItemDesc[type])) {
		loadItem(type);
	}
	return mpType2ItemDesc[type]["amount"];
}

int isEquip(string type)
{
	if (!sizeof(type)) return 0;

	if (undefinedp(mpType2ItemDesc[type])) {
		loadItem(type);
	}
	return mpType2ItemDesc[type]["is_equip"];
}

