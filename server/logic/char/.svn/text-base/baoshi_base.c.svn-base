#include <item.h>
#include <item_key.h>
#include <equip.h>
#include <common_key.h>
#include <baoshi.h>

inherit ITEM_BASE;

mapping getData()
{
        debug_message("请继承getData()" + base_name(this_object()));                                                                                                                                      
        return ([]); 
}

int getBaoshiGrade()
{
        mapping data = getData();
        return data[K_BAOSHI_GRADE];
}

int getBaoshiAttr()
{
        mapping data = getData();
        return data[K_BAOSHI_ATTR];
}

int isBaoshi()
{
        return 1;
}
