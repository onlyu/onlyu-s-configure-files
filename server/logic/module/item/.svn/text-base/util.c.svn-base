#include <macros.h>
#include <item_key.h>
#include <frames.h>
#include "/rc/rpc/item.h"
#include <debug.h>
#include <item.h>
#include "name.h"
//#include "equal.h"
//#include <save.h>
//#include <mail.h>

static object logger;

varargs int CreateItem(string cType, int iAmount);

/*
 * return 成功返回大于0的整数 失败返回0 
 */
int addItem(object user, int itemId, int bagId)
{
	int res = user->XAddItem(itemId, bagId);
	if ( res < 1)
	{
		destroy_item(itemId);
		res = 0;
	}
	return res;
}

/*
 * return 成功返回itemId失败返回0
 */
varargs int addNewItem(object user, string ctype, int count, int bagId)
{
	if (!bagId)
		bagId = ITEM_BAG_NORMAL;
	int itemId = CreateItem(ctype, count);
	if ( itemId < 1 )
		return 0;
	if(addItem(user, itemId,bagId) >= 1)
		return itemId;
	return 0;
}

int canMoveToBag(int itemId, int bagId, int toPos)
{
	return 1;
}

int canMerge(int iSrc, int iTarget)
{
	int iMaxAmount ;

	if ( iSrc == iTarget ) 
		return 0;

	if ( GetItemSave(iSrc, K_ITEM_TYPE) != GetItemSave(iTarget, K_ITEM_TYPE) )
		return 0;


	iMaxAmount = GetItemTemp(iTarget, KT_ITEM_MAXAMOUNT);
	if ( iMaxAmount < 2 )
		return 0;

	if ( GetItemSave(iTarget, K_ITEM_AMOUNT) + GetItemSave(iSrc, K_ITEM_AMOUNT) > iMaxAmount ) 
		return 0;


	return 1;
}
/*
*@param toPos 0 找一个可用的位置, < 0 无效的参数, > 0 移到指定的位置
*@param isMerge 非0 如果可以融合则融合, 0 交换不融合 
*@param toBag 由调用者指定，不提供默认
*@return 0失败 1成功
*/
int moveItem(object user, int itemId, int toBag, int toPos, int isMerge)
{
	if ( GetItemTemp(itemId, KT_ITEM_OWNER) != user->getId() )
		return 0;

	if(!canMoveToBag(itemId, toBag, toPos))
		return 0;

	int targetItemId = user->getItemByPos(toBag, toPos);
	if (targetItemId == itemId)
		return 0;
	if(!targetItemId)
	{
		return	user->MoveItem(itemId, toBag, toPos);
	}

	if (canMerge(itemId, targetItemId))
	{
		if (isMerge)
		{
			user->MergeItem(itemId, targetItemId, toBag, toPos);
			return 1;
		}
	}
	user->SwapItem(itemId, targetItemId);
	return 1;
}
	
////////////////


void SetAmount(int iItem, int iAmount)
{
	logger->Log("setamount iItem=%d iAmount=%d", iItem, iAmount);
	int iOwner;

	SetItemSave( iItem, K_ITEM_AMOUNT, iAmount );
	iOwner = GetItemTemp(iItem, KT_ITEM_OWNER);

	//debug_message( sprintf("SetItemAmount: iOwner %d, 0x%x, iAmount %d.", iOwner, iItem, iAmount) );

	if (iAmount == 0)
	{
		destroyItem(iItem);
		rpc_client_item_del(iOwner, iItem);
		logger->Log("Delete iOwner:%d iItem=%d", iOwner, iItem);
		return;
	}	


	if (iOwner) {
		// 协议通知client
		rpc_client_item_amount( iOwner, iItem, iAmount );
	}
}

private int _ModiAmount(int iItem, int iDelta)
{
	int iAmount, iMaxAmount;

	iMaxAmount = GetItemTemp(iItem, KT_ITEM_MAXAMOUNT);
	assert( iMaxAmount > 0 );

	iAmount = GetItemSave(iItem, K_ITEM_AMOUNT) + iDelta;

	if ( iAmount > iMaxAmount )
	{	
		debug_message(sprintf("ERROR: 0x%x modiamount +%d greater than iMaxAmount %d, reset to maxamount. owner is %d.", iItem, iDelta, iMaxAmount, GetItemSave( iItem, KT_ITEM_OWNER)) );
		iAmount = iMaxAmount;
	}	

	SetAmount( iItem, iAmount );
	return iAmount ;
}

// 返回 iItem 的剩余个数
int IncAmount(int iItem, int iDelta)
{
	return _ModiAmount( iItem, iDelta);
} 

// 返回 iItem 的剩余个数
int DecAmount(int iItem, int iDelta)
{
	int iAmount = _ModiAmount( iItem, -iDelta);

	if ( iAmount <= 0)
	{
		destroyItem(iItem);	
	}
	return iAmount;
} 


// TODO
int CloneItem( int iItem, int iAmount)
{
	string cType;
	int iClone;

	cType = GetItemSave(iItem, K_ITEM_TYPE);
	iClone = CreateItem(cType);

	if ( iClone > 0 )
	{
		foreach( mixed key, mixed value in get_item_map(iItem, PERM_VAR) )
		{
			SetItemSave(iClone, key, value);	
		}
		foreach( mixed key, mixed value in get_item_map(iItem, TEMP_VAR) )
		{
			SetItemTemp(iClone, key, value);	
		}
	}

	SetItemSave(iClone, K_ITEM_AMOUNT, iAmount);
	SetItemTemp(iClone, KT_ITEM_OWNER, 0);
	SetItemTemp(iClone, KT_ITEM_BAG, 0);
	SetItemTemp(iClone, KT_ITEM_POS, 0);

	return iClone;
}

// TODO 是否被锁定不可以交易
int isLocked(int iItem)
{
	if (GetItemSave(iItem, K_ITEM_LOCK) ) return 1;
	if (GetItemTemp(iItem, KT_ITEM_NOMERGE) ) return 1;

	return 0 ;
}

int isLockedByData(mapping save_data)
{
	if (save_data[K_ITEM_LOCK]) return 1;
	if (save_data[KT_ITEM_NOMERGE]) return 1;
	return 0;
}

int CanDrop(int iItem)
{
	// 任务物品, 不允许丢弃
	if (sizeof(GetItemSave(iItem, K_ITEM_TASK_OWNER))) {
		return 0;
	}

	if (GetItemTemp(iItem, KT_ITEM_BAG) == ITEM_BAG_TASK) {
		return 0;
	}
	return 1;
}

int CanMoveTo(int iItemId, int iBagTo, int iPosTo)
{
	int iBagFrom, iPosFrom ;

	if (isLocked(iItemId))
		return 0;

	iBagFrom = GetItemTemp(iItemId, KT_ITEM_BAG);
	iPosFrom = GetItemTemp(iItemId, KT_ITEM_POS);

	// check iBagFrom && iBagTo
	if (iBagFrom != iBagTo) {
		return 0;
	}

	if (!isNormalItemBag(iBagTo))
		return 0;


	return 1;
}





int restoreItem(mapping mpSave)
{
	string cType = mpSave[K_ITEM_TYPE];
	string path;
	int item;

	if (undefinedp(cType) || !sizeof(cType)) {
		return 0;
	}

	path = ITEM_PATH + cType + ".c";

	if (catch(call_other(path, "???" )) != 0) {
		debug_message(sprintf("can't create item[Type:%s]", cType));
		return 0;
	}

	item = restore_item(path, mpSave);

	return item;
}

varargs int CreateItem(string cType, int iAmount)
{
	string path = ITEM_PATH + cType;
	int item;
	object ob;

	ob = find_object(path, 1);
	if (!objectp(ob)) {
		logger->Log("can't create item[Type:%s]", cType);
		return 0;
	}

	item = new_item(path);

	int maxItemAmount = GetItemTemp(item, KT_ITEM_MAXAMOUNT);
	debug_message(sprintf("maxItemAmount ctype=%s itemId=%d maxamount=%d", cType, item, maxItemAmount));
	if ( iAmount > 0 && maxItemAmount && iAmount <= maxItemAmount)
	{
		SetAmount(item, iAmount);
	}

	//TODO: cGblKey
	if ( !GetItemSave(item, K_GLOBAL_KEY) )
	{	
		//string cGblKey = Import("GLOBAL_KEY")->new_item_globalkey();
		//暂时用时间 + 随机数
		string cGblKey = sprintf("%d%d", time(), random(1000000));
		SetItemSave(item, K_GLOBAL_KEY, cGblKey); 
	}

	if (path->isEquip()) {
		"module/equip/main"->initEquip(item);
	}

	return item;
}

void create()
{
	logger = Import("LOG")->New("item_util");
}

void sendAddItemEffect(object user, string ctype, int amount)
{
	int icon = getItemIcon(ctype);
	rpc_client_play_add_item_effect(user->getId(), ctype, amount);
}

void rpc_AddItemToUser(int usernum, int iItemId)
{
	int iAmount;
	int iLockType;
	string type = GetItemSave(iItemId, K_ITEM_TYPE);

	class item_add_t _add_t = new( class item_add_t );
	class item_pos_t _pos = new( class item_pos_t ); 
	class item_info_t _info = new( class item_info_t );

	_pos->iBag = GetItemTemp(iItemId, KT_ITEM_BAG);
	_pos->iPos = GetItemTemp(iItemId, KT_ITEM_POS);

	_info->iItemId = iItemId;
	_info->iOwner = usernum;
	_info->cType = type;

	iAmount = GetItemSave(iItemId, K_ITEM_AMOUNT);
	_info->iAmount = iAmount;

	iLockType = GetItemSave(iItemId, K_ITEM_LOCK) ? GetItemSave(iItemId, K_ITEM_LOCK) : 0 ;
	_info->iLockType = iLockType;

	_add_t->posinfo = _pos;
	_add_t->iteminfo = _info;

	//debug_message(sprintf("client add %O", _add_t));
	rpc_client_item_add(usernum, _add_t);

	if ( (ITEM_PATH + type)->isEquip()) {
		"module/equip/main"->sendEquipInfo(usernum, iItemId);
	}
}

