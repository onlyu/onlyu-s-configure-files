
#include "/rc/rpc/item.h"
#include <item_key.h>
#include <frames.h>
#include <debug.h>  
#include <macros.h>
#include <item.h>
#include <user_key.h>

private object logger;
static string use_item_key = "use_item";
#define UNLOCK_BAG_PER_PRICE 2

//static string drop_item_key = "drop";
//#define DROP_ITEM_LOG_FORMAT "operate=%s,usernum1=%d,kind=%s,amount=%d,userip1=%s,hd1=%s,mac1=%s,objstr=%s"

#define DROP_ITEM_LOG_FORMAT "operate=drop,usernum1=%d,kind=%s,amount=%d,%s,objstr=%s"



void create_logger()
{
	logger = Import("LOG")->New("item");
	//uid, target, cType, iAmout 
	//logger->SetDBFormat(use_item_key, "%d,%d,%s,%d");
}

void rpc_server_itembag_unlockquery(object user, int bagId, int curSize, int unlockToSize)
{
	
	int _curSize = user->getProp(K_USER_BAGSIZE);
	int maxBagSize = user->getItemBagMaxSize(ITEM_BAG_NORMAL);
	if (_curSize != curSize || curSize >= unlockToSize || unlockToSize > maxBagSize)
	{
		debug_message("unlockquery _curSize:%d curSize:%d toSize:%d maxSize:%d", _curSize, curSize, unlockToSize, maxBagSize);
		return;
	}
	rpc_client_itembag_unlockquery(user->getId(), bagId, curSize, unlockToSize, (unlockToSize-curSize) * UNLOCK_BAG_PER_PRICE);	
}

void rpc_server_itembag_unlock(object user, int bagId, int curSize, int unlockToSize)
{
	int _curSize = user->getProp(K_USER_BAGSIZE);
	int maxBagSize = user->getItemBagMaxSize(bagId);
	debug_message("unlock _curSize:%d curSize:%d toSize:%d maxSize:%d", _curSize, curSize, unlockToSize, maxBagSize);
	if (_curSize != curSize || curSize >= unlockToSize || unlockToSize > maxBagSize)
		return;
	int neededYuanBao = (unlockToSize - curSize) * UNLOCK_BAG_PER_PRICE;
	int totalYB = user->getTotalYB();
	if (neededYuanBao > totalYB)
	{	
		user->tellMe("元宝不足");
		return;
	}
	user->minusYB(neededYuanBao);
	debug_message("unlock bag uid:%d minusYB:%d totalYB:%d", user->getId(), neededYuanBao, totalYB);
	user->setProp(K_USER_BAGSIZE, unlockToSize);
	debug_message("unlock res uid:%d toSize:%d leftYB:%d" ,user->getId(), unlockToSize, user->getTotalYB());
	rpc_client_itembag_unlock(user->getId(), bagId, unlockToSize);		
}


void rpc_server_item_move(object oUser, class item_move_t _move_t)
{
	int itemid, iBagFrom, iPosFrom, iBagTo, iPosTo, iTarget;
	int * ServerPos ;

	debug_message("%O", _move_t);

	itemid = _move_t->iItemId;
	iBagFrom = _move_t->pos_from->iBag;
	iBagTo = _move_t->pos_to->iBag;
	iPosFrom = _move_t->pos_from->iPos;
	iPosTo = _move_t->pos_to->iPos;
	Import("ITEM_UTIL")->moveItem(oUser, itemid, iBagTo, iPosTo, 1);
  	return;
}

void rpc_server_item_del(object oUser, int itemid)
{
	string cName, cType;
	int iAmount;

	if ( !is_item_online(itemid) ) return ;

	//if ( CanItemOprate( oUser, 1 ) < 0 ) return;
	//if ( !Import("LOCK")->IsUnlock(oUser) ) return ;

	//assert( GetItemTemp(itemid, KT_ITEM_OWNER) == oUser->GetId() );
	//assert( oUser->FindItem(itemid, GetItemTemp(itemid, KT_ITEM_BAG)) == GetItemTemp(itemid, KT_ITEM_POS) );

	//捕捉丢弃动作，如果捕捉成功则不往下执行了
	object oObj = GetItemObject(itemid);
	//if(oObj->CatchDrop(oUser, itemid) > 0) return;

	/*
	if ( !Import("ITEM_UTIL")->CanDrop(itemid) )
	{	
		oUser->TellMe("这件物品不能被丢弃。");
		return ;
	}
	*/
	/*
	if (ItemCannotDrop( itemid ))
	{
		oUser->TellMe("这件物品不能被丢弃。");
		return ;
	}
	*/
	iAmount = GetItemSave(itemid, K_ITEM_AMOUNT);
	cName = GetItemTemp(itemid, KT_ITEM_NAME);
	cType = GetItemSave(itemid, K_ITEM_TYPE);

	//需要ITEM信息，LOG之后再删除
	//catch( LogDropItem( oUser, itemid ) );

 	oUser->RemoveItem(itemid);
	destroy_item(itemid);

	//oUser->TellMe( sprintf("丢弃了%d个%s。", iAmount, cName) );

	//改为LogDropItem
	//logger->Log("[%d] drop %d %s[%s]", oUser->GetId(), iAmount, cType, cName);
  	
}

void rpc_server_item_tips_req(object user, int itemid)
{
	rpc_client_item_tips(user->getId(), itemid, "物品ID:"+itemid);
}


void rpc_server_item_desc_req(object user, string type)
{
	string itemName = getItemName(type);
	string itemDesc = getItemDesc(type);

	int uid = user->getId();

	// 物品描述
	rpc_client_item_desc(uid, type, itemName, itemDesc);

	if ( (ITEM_PATH + type)->isEquip()) {
		"module/equip/main"->sendEquipDesc(uid, type);
	}
}


void rpc_server_movein_storage(object user, int itemId, int toPos)
{
	if (itemId <= 0 || toPos <= 0)
		return;

	int fromBag = ITEM_BAG_NORMAL;
	int toBag = ITEM_BAG_STORAGE;
	int fromBagClient = GetItemTemp(itemId, KT_ITEM_BAG);
	if (fromBag != fromBagClient)
		return;
	Import("ITEM_UTIL")->moveItem(user, itemId, toBag, toPos, 1);
}

void rpc_server_moveout_storage(object user, int itemId, int toPos)
{
	if (itemId <= 0 || toPos <= 0)
		return;

	int toBag = ITEM_BAG_NORMAL;
	int fromBag = ITEM_BAG_STORAGE;
	int fromBagClient = GetItemTemp(itemId, KT_ITEM_BAG);
	if (fromBag != fromBagClient)
		return;
	Import("ITEM_UTIL")->moveItem(user, itemId, toBag, toPos, 1);
}

int sortItem(int id1, int id2)
{
        string kind1 = GetItemSave(id1, K_ITEM_TYPE);
        string kind2 = GetItemSave(id2, K_ITEM_TYPE);

        if (kind1 == kind2) {
                int kindval1 = to_int(kind1[2..]);
                int kindval2 = to_int(kind1[2..]);
                if (kindval1 == kindval2) {
                        return 0;
                }
                else {
                        if (kindval1 > kindval2) {
                                return 1;
                        } else {
                                return -1;
                        }
                }
		return 0;
        }
        else {
                if (kind1 > kind2) {
                        return 1;
                } 
                else {
                        return -1;
                }
        }
        return 0;
}

// 背包整理
void rpc_server_item_clear_up(object user)
{
	int *items = user->getAllItemsInBag(ITEM_BAG_NORMAL);
        // items = sort_array(items, 1);
        items = sort_array(items, (:sortItem:));

	int pos = 0;
	int max = sizeof(items);

	while (pos  < max) {
		Import("ITEM_UTIL")->moveItem(user, items[pos], ITEM_BAG_NORMAL, pos + 1);
		pos++;
	}
}

void create()
{
	logger = Import("LOG")->New("item");
}
