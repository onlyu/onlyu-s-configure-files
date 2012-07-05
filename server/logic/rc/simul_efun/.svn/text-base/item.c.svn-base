// 添加一些物品efun
#include <item.h>
#include <item_key.h>


static mapping mpKind = ([
		/*
        "CL" : ITEM_PRODUCT, // 生产职业材料
        "SC" : ITEM_SHANGCHENG, // 商城

        "BF" : ITEM_BUFF, // buff物品
        "LS" : ITEM_LINGSHOU, // 灵兽物品   
        "DH" : ITEM_DH,  // 兑换物品
        "DT" : ITEM_EQUIP_DHT, // 兑换套装
        "YP" : ITEM_DRUG, // 药品
        "RW" : ITEM_MISSION, //任务
        //"GE" : ITEM_EQUIP,    //  绿色装备
        //"BE" : ITEM_EQUIP,    //  蓝色装备
        //"PE" : ITEM_EQUIP,    //  紫色装备
        //"GT: : ITEM_EQUIP,    //  绿色套装
        //"BT: : ITEM_EQUIP,    //  蓝色套装
        //"PT: : ITEM_EQUIP,    //  紫色套装
        "TS" : ITEM_TUSHI, // 图饰, 灵兽buff
        "TA" : ITEM_TUSHI, // 图饰, 灵兽buff
        "XZ" : ITEM_XIANGZI, // 箱子
        "QC" : ITEM_QIANGHUA, // 强化材料
		"ZH" : ITEM_ZAHUO,
		"FX" : ITEM_FLY_TOOL,
		"BS" : ITEM_ZAHUO,
		*/
        ]);


int GetTypeKind(string cType)
{
    return mpKind[cType[0..1]];
}

int GetItemKind(int itemid)
{
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	string strTmp = cType[0 .. 1];
    int iKind = mpKind[strTmp];

	if ( !iKind )
	{
		string cErrorMsg = sprintf("新类型%s，%s. tell Prophet!!!!!!!", cType, strTmp);
		// xassert(iKind, cErrorMsg);
		debug_message(cErrorMsg);
		return 0;
	}
	return mpKind[strTmp];
}

string GetItemCType( int itemid )
{
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	return cType;
}


int GetItemNeedGrade( int itemid )
{
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	return (ITEM_PATH + cType)->GetNeedGrade();
}

int GetItemAddHpOut( int itemid )
{	
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	return (ITEM_PATH + cType)->GetAddHpOut();
}

int GetItemAddMpOut( int itemid )
{	
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	return (ITEM_PATH + cType)->GetAddMpOut();
}

int GetItemAddHp( int itemid )
{	
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	return (ITEM_PATH + cType)->GetAddHp();
}

int GetItemAddMp( int itemid )
{	
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	return (ITEM_PATH + cType)->GetAddMp();
}

int GetItemLastOut( int itemid )
{
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	return (ITEM_PATH + cType)->GetLastBout();
}

int CanItemUseInDie( int itemid )
{
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	return (ITEM_PATH + cType)->CanUseInDie();
}

int GetShoulongGrow( int itemid )
{
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	return (ITEM_PATH + cType)->GetSummonGrow();
}

int CanItemTrade( int itemid )
{
    string cType = GetItemSave(itemid, K_ITEM_TYPE);
    return (ITEM_PATH + cType)->CanTrade();
}

int ItemDoUse( object owner, int itemid, int target )
{
	string cType;

	if ( !is_item_online(itemid) ) return 0 ;

	cType = GetItemSave(itemid, K_ITEM_TYPE);	

	return (ITEM_PATH + cType)->DoUse(owner, itemid, target);
}

int ItemDoUseInFight( int fightId, int warrior, int itemid, int side, int position)
{
	string cType;

	if ( !is_item_online(itemid) ) return 0 ;

	cType = GetItemSave(itemid, K_ITEM_TYPE);	
	if (sizeof(cType) == 0) 
	{
		debug_message( "ERROR:" + itemid );
		return 0;
	}

	return (ITEM_PATH + cType)->DoUseInFight(fightId, warrior, itemid, side, position);
}

int ItemIsTimeOut( int itemid )
{
	int iTimeOut;

	iTimeOut = GetItemSave( itemid, K_ITEM_TIMEOUT );
	if (iTimeOut > 0)           
	{
		int iCurTime = time();
		if ( iTimeOut <= iCurTime )
		{
			return 1;
		}
	} 
	return 0;
}

int ItemCannotDrop( int itemid )
{
	string cType = GetItemSave(itemid, K_ITEM_TYPE);
	return (ITEM_PATH + cType)->CannotDrop();
}

string GetItemTipsByMap(object oUser, mapping save_data, mapping temp_data)
{
	string cType = save_data[K_ITEM_TYPE];	
	return (ITEM_PATH + cType)->ItemTips(oUser, save_data, temp_data);
}

string GetItemTips(object oUser, int itemid)
{
	return GetItemTipsByMap(oUser, get_item_map(itemid, 1), get_item_map(itemid, 0));
	/*
	string cType = GetItemSave(itemid, K_ITEM_TYPE);	
	return (ITEM_PATH + cType)->ItemTips(oUser, itemid);
	*/
}

object GetItemObject(int itemid)
{
	return find_object(ITEM_PATH + GetItemSave(itemid, K_ITEM_TYPE), 1);
}


// ===========================================================

#define  ITEM_DATA_OBJ   "data/all_items_data"

string getItemName(string type)
{
	return ITEM_DATA_OBJ->getName(type);
}

string getItemDesc(string type)
{
	return ITEM_DATA_OBJ->getDesc(type);
}

int getItemIcon(string type)
{
	return ITEM_DATA_OBJ->getIcon(type);
}


