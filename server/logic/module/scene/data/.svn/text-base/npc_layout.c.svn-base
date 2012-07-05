#include <macros.h>
// -------------------  Auto Generate Begin --------------------

mapping data = ([ 
 
	1:([ 
	  "name":"天外村村长",  
	  "scene":1001,  
	  "path":"char/npc/template/option.c",  
	  "y":320,  
	  "x":1640,  
	  "type":"普通",  
	  "icon":46002, ]),
	2:([ 
	  "name":"铁匠",  
	  "scene":1001,  
	  "path":"char/npc/template/option.c",  
	  "y":320,  
	  "x":1340,  
	  "type":"普通",  
	  "icon":46003, ]),
	3:([ 
	  "name":"大娘",  
	  "scene":1001,  
	  "path":"char/npc/template/option.c",  
	  "y":320,  
	  "x":1940,  
	  "type":"普通",  
	  "icon":46004, ]),
	4:([ 
	  "name":"传送点",  
	  "scene":1001,  
	  "path":"char/npc/template/transport.c",  
	  "y":440,  
	  "x":2350,  
	  "type":"传送",  
	  "icon":46001, ]),
	5:([ 
	  "name":"灵石",  
	  "title":"古老的东海灵石",  
	  "scene":1601,  
	  "path":"char/npc/template/option.c",  
	  "y":334,  
	  "x":2050,  
	  "type":"普通",  
	  "icon":46005, ]),
	6:([ 
	  "name":"大野木",  
	  "title":"巨野镇镇长",  
	  "scene":1501,  
	  "path":"char/npc/template/option.c",  
	  "y":328,  
	  "x":1640,  
	  "type":"普通",  
	  "icon":46006, ]),
	7:([ 
	  "name":"赤土",  
	  "scene":1501,  
	  "path":"char/npc/template/option.c",  
	  "y":352,  
	  "x":960,  
	  "type":"普通",  
	  "icon":46007, ]),
	8:([ 
	  "name":"黄土",  
	  "scene":1501,  
	  "path":"char/npc/template/option.c",  
	  "y":296,  
	  "x":2048,  
	  "type":"普通",  
	  "icon":46008, ]),
	9:([ 
	  "name":"传送点",  
	  "scene":1501,  
	  "path":"char/npc/template/transport.c",  
	  "y":440,  
	  "x":2350,  
	  "type":"普通",  
	  "icon":46001, ]),
	10:([ 
	  "name":"济天下",  
	  "scene":1201,  
	  "path":"char/npc/template/option.c",  
	  "y":328,  
	  "x":1640,  
	  "type":"普通",  
	  "icon":46010, ]),
	11:([ 
	  "name":"司天监",  
	  "scene":1201,  
	  "path":"char/npc/template/option.c",  
	  "y":352,  
	  "x":960,  
	  "type":"普通",  
	  "icon":46011, ]),
	12:([ 
	  "name":"传送点",  
	  "scene":1201,  
	  "path":"char/npc/template/transport.c",  
	  "y":440,  
	  "x":2350,  
	  "type":"传送",  
	  "icon":46001, ]),
	13:([ 
	  "name":"老渔民",  
	  "scene":1601,  
	  "path":"char/npc/template/option.c",  
	  "y":334,  
	  "x":1062,  
	  "type":"普通",  
	  "icon":46012, ]),
	14:([ 
	  "name":"敏儿",  
	  "scene":1601,  
	  "path":"char/npc/template/option.c",  
	  "y":334,  
	  "x":1318,  
	  "type":"普通",  
	  "icon":46013, ]),
	15:([ 
	  "name":"夕颜",  
	  "scene":1601,  
	  "path":"char/npc/template/option.c",  
	  "y":334,  
	  "x":1534,  
	  "type":"普通",  
	  "icon":46014, ]),
	16:([ 
	  "name":"传送点",  
	  "scene":1601,  
	  "path":"char/npc/template/transport.c",  
	  "y":463,  
	  "x":2435,  
	  "type":"传送",  
	  "icon":46001, ]),
	17:([ 
	  "name":"大娘杂货",  
	  "scene":1001,  
	  "path":"char/npc/template/shop.c",  
	  "y":300,  
	  "x":1000,  
	  "type":"功能",  
	  "icon":46004, ]),
 
]);

mapping get_data()
{
	return data;
}
// from tmp/data/世界NPC.xls
// -------------------  Auto Generate End   --------------------

static mapping name2id = ([]);

mapping getNpcInfo(int uniqId)
{
        return data[uniqId];
}

/*mapping getNpcInfoByName(string name)
{
        int uniqId = name2id[name];
        return getNpcInfo(uniqId);
}*/

string getNpcName(int uniqId)
{
        if( data[uniqId] ) return data[uniqId]["name"];
        return "";
}

int getNpcIcon(int uniqId)
{
        if( data[uniqId] ) return data[uniqId]["icon"];
        return 0;
}

string getNpcPath(int uniqId)
{
        if( data[uniqId] ) return data[uniqId]["path"];
        return "";
}

// 建立NPC名字和npc uniqId的映射
static void buildNameCache()
{
        foreach(int uniqId, mapping mpInfo in data) {
                string name = mpInfo["name"];
                if( !undefinedp(name2id[name]) ) {
                        debug_message(sprintf("multiply npc name %s, %d override %d", name, uniqId, name2id[name]));
                }

                name2id[name] = uniqId;
        }

        debug_message("build npc name cache finish!");
}

void create()
{
        //buildNameCache();
}
