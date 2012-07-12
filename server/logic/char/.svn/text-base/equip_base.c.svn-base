
#include <item.h>
#include <item_key.h>
#include <equip.h>
#include <common_key.h>

inherit ITEM_BASE;

mapping getData()
{
	debug_message("请继承getData()" + base_name(this_object()));                                                                                                                                      
	return ([]); 
}

int getLocate()
{
	mapping data = getData();
	return data[K_EQUIP_LOCATE];
}


// 获取强化最大数量
int getQianghuaValue()
{
	mapping data = getData();
	return data[K_EQUIP_QIANGHUA];
}

int getEquipColor()
{
	mapping data = getData();
	return data[K_EQUIP_COLOR];
}

int getEquipGrade()
{
	mapping data = getData();
	return data[K_EQUIP_GRADE];
}

int getEquipType()
{
	mapping data = getData();
	return data[K_EQUIP_TYPE];
}

// 获取强化数值
mapping getQianghuaAttr()
{
	mapping data = getData();
	mapping res = ([]);

	foreach (mixed _key in ({ 
						   FT_DEFENCE,
                           FT_QIANGFA,
                           FT_KANGFA,
                           FT_ATTACK,
                           FT_SP,
                           FT_HP, }) ) {
		if (!undefinedp(data[_key])) {
			res[_key] = data[_key];
		}
	}
	return res;
}

int isEquip()
{
	return 1;
}
