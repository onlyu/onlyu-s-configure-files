#include <item_key.h>
#include <macros.h>

int destroyItem(int item)
{
	int owner;
	object oUser;

	owner = GetItemTemp(item, KT_ITEM_OWNER);
	if (owner)
	{
		oUser = get_user(owner);
		if ( !oUser )
			debug_message( sprintf("ERROR!物品%d[%O]属主不在线？请检查代码。", item, GetItemSave(item, K_GLOBAL_KEY) ) );	
		else
			oUser->RemoveItem(item);
	}

	destroy_item(item);
	return 1;
}	

int destroySummon(int summonId)
{
	return "module/summon/util"->destroySummon(summonId);
}

int isUserExist(int uid)
{
	if( file_size( sprintf("user/%02d/u%d.py", uid%100, uid) ) > 0 )
		return 1;

	if ( file_size( sprintf("inactive_user/%02d/u%d.py", uid%100, uid) ) > 0 )
		return 1;

	return 0;
}

int getHostNum()
{
	return "rc/config"->GetConfig("HOST_ID");
}

string getEncoding()
{
	return "rc/config"->GetConfig("ENCODING");
}

string cash2Str( int cash )
{
	int gold, silver, coin;

	gold = cash / 10000;
	silver = (cash % 10000) / 100;
	coin = cash % 100;

	return (gold?gold+"":"") + (silver?silver+"":"") + (coin?coin+"":"");
}

// 文字版
string cash2StrWord( int cash )
{
	int gold, silver, coin;

	gold = cash / 10000;
	silver = (cash % 10000) / 100;
	coin = cash % 100;

	return (gold?gold+"金":"") + (silver?silver+"银":"") + (coin?coin+"铜":"");
}

mapping getOrSetMapping(mapping mp, mixed key)
{
	mapping v = mp[key];
	if (undefinedp(v)) {
		v = ([]);
		mp[key] = v;
	}
	return v;
}

mapping getOrSetMappingDepth(mapping root, mixed *keys)
{
	mapping m = root;
	foreach (mixed key in keys) {
		m = getOrSetMapping(m, key);
	}
	return m;
}

mixed getmappingdepth(mapping root, mixed *keys)
{
	mapping m = root;
	foreach (mixed key in keys) {
		m = m[key];
		if (undefinedp(m)) {
			return 0;
		}
	}
	return m;
}

mapping mpUnit = ([
			0:"零",
			1:"一",
			2:"二",
			3:"三",
			4:"四",
			5:"五",
			6:"六",
			7:"七",
			8:"八",
			9:"九",
		]);

string unit2Str(int unit)
{
	return mpUnit[unit];
}

mapping mpPlace = ([
			// 32位
			10:(["normal":"", "em":"",]),
			100:(["normal":"十", "em":"十",]),
			1000:(["normal":"百", "em":"百",]),
			10000:(["normal":"千", "em":"千",]),
			100000:(["normal":"万", "em":"万",]),
			1000000:(["normal":"十万", "em":"十",]),
			10000000:(["normal":"百万", "em":"百",]),
			100000000:(["normal":"千万", "em":"千",]),
			1000000000:(["normal":"亿", "em":"亿",]),
			10000000000:(["normal":"十亿", "em":"十",]),
		]);

string place2Str(int place, int em_num)
{
	int divider = 10;
	while(place / divider) {
		divider = divider * 10;
	}
	return em_num ? mpPlace[divider]["em"] : mpPlace[divider]["normal"];
}

// 将阿拉伯数字转化为中文数字，如：1 --> 一
string arabicNum2ChineseNum(int num)
{
	int rest = num;
	int minor = 0;
	string ret = "";
	if(num == 0) {
		ret = "零";
	}
	else if(num < 0) {
		rest = -rest;
		minor = 1;
	}
	else {}

	// 1401009之类的情况
	int display_zero = 0;
	int divider = 10;
	int unit = rest % divider;
	int em = 1;
	int place = 0;
	while(rest) {
		place = num % divider;
		if(unit || display_zero) {
			if(unit) {
				ret = place2Str(place, em) + ret;
				em = 0;
			}
			// 在此考虑大于10小于20的表现方式
			if(num < 10 || num >= 20) {
				ret = unit2Str(unit) + ret;
			}
		}
		if(unit) {
			display_zero = 1;
			em = 1;
		}
		else {
			display_zero = 0;
			if(place / 1000000000 == 0 || place / 100000 == 0) {
				em = 0;
			}
			else {
				em = 1;
			}
		}
		rest = num / divider;
		divider = divider * 10;
		unit = rest % 10;
	}
	if(minor) {
		ret = "负" + ret;
	}
	return ret;	
}

//dbvar不能直接 = ([])
void clearDBVar(mapping dbvar)
{
	foreach(mixed key in keys(dbvar)) {
		map_delete(dbvar, key);
	}
}


int sortFunc(int key1, int key2, mapping mpData, int order)
{
	int value1 = mpData[key1];
	int value2 = mpData[key2];
	if (order >= 0) {
		return (value1 >= value2) ? 1 : -1;
	} else {
		return (value1 < value2) ? 1 : -1;
	}
	return 0;
}

int *sortMap(mapping mpData, int order)
{
	return sort_array(keys(mpData), (: sortFunc :), mpData, order);
}
