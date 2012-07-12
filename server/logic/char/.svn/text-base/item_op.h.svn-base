
#include <frames.h>
#include <item_key.h>
#include <item.h>
#include <debug.h>
//#include "/rc/rpc/item_decl.c"
#include "/rc/rpc/item.h"
#include <user_key.h>

// 这样OO的写法，方便于少传参数
// 用include方式空间换时间，省却多重语法查找
// 同时也避免了内存中存在多个object的写法，其实个人认为存多个object会省事很多

// Ver 0.2 2010-07-28 10:44 by canoe
// 在Item上加了iBag, pos 信息，方便查询 操作 少传参数

varargs int RemoveItem(int itemId, int inputBagId);

// 用函数操作mapping，不应该有对item的直接操作
private mapping getItemFrame()
{
	return item_frame;
}

// 把item 加入dirty mapping 
private void addSaveItem(int bagId, int pos, int itemId)
{
	if ( undefinedp(item[bagId]) )
	{
		item[bagId] = ([ ]);
	}
	item[bagId][pos] = get_item_map(itemId, PERM_VAR);
}

// 把item 从dirty mapping 删除
private void removeSaveItem(int bagId, int pos)
{
	map_delete(item[bagId], pos);
}

// 删除一个背包, 必须先保证这个背包的物品已经清除掉
void removeBag(int bagId)
{
	// 非装备栏不允许删除
	if (!isEquipBag(bagId)) {
		return;
	}
	// 不存在
	if (undefinedp(item[bagId])) {
		return;
	}
	// 物品不为空
	if (sizeof(item[bagId])) {
		return;
	}
	mapping mpItem = getItemFrame();
	map_delete(item, bagId);
	map_delete(mpItem, bagId);
}

int* findItemsByType(string type, int bagId)
{
	int* items = ({});
	mapping bag = item[bagId];

	if (sizeof(bag))
	{
		mapping itemBagFrame = getItemFrame()[bagId];
		mapping itemGrids = Import("BASE_BAG")->GetGrids(itemBagFrame);
		foreach (int pos, mapping itemInfo in bag)
		{
			if (itemInfo[K_ITEM_TYPE] == type)
			{
				items += ({itemGrids[pos]});
			}
		}
	}
	return items;
}
int getSameTypeItemAmount(string type, int bagId)
{
	mapping bag = item[bagId];
	int amount = 0;
	if (sizeof(bag))
	{
		foreach (int pos, mapping itemInfo in bag)
		{
			if (itemInfo[K_ITEM_TYPE] == type)
				amount += itemInfo[K_ITEM_AMOUNT];
		}
	}
	return amount;
	
}

int delSameTypeItemAmount(string type, int totalAmount, int bagId)
{
	if (totalAmount <=0  || getSameTypeItemAmount(type, bagId) < totalAmount)
		return 0;
	debug_message("del SameTypeItemAmount");
	foreach ( int itemId in findItemsByType(type, bagId) )
	{
		int maxAmount = GetItemTemp(itemId, KT_ITEM_MAXAMOUNT);

		if ( maxAmount > 0 ) // 可数
		{
			int amount = GetItemSave(itemId, K_ITEM_AMOUNT);
			if ( amount >= totalAmount )
			{
				amount = totalAmount;
			}

			Import("ITEM_UTIL")->DecAmount(itemId, amount);
			totalAmount -= amount;
		}
		else
		{
			debug_message("enter for each");
			destroyItem(itemId);
			totalAmount -= 1;
		}

		if ( totalAmount < 1 )
			return 1;
	}
}
// 玩家身上的物品格子数量
int getItemBagSize(int bagId)
{
	if (bagId == ITEM_BAG_NORMAL) {
		int cnt = this_object()->getProp(K_USER_BAGSIZE);
		if (!cnt) {
			cnt = getItemBagDefaultBaseSize(bagId);
			this_object()->setProp(K_USER_BAGSIZE, cnt);
		}
		return cnt;
	}
	return getItemBagDefaultMaxSize(bagId);
}

int getItemBagMaxSize(int bagId)
{
	return getItemBagDefaultMaxSize(bagId);
}

int getItemByPos(int bagId, int pos);

private void initItemFrame(int bagId)
{
	mapping mpItem = getItemFrame();

	int defMaxSize = getItemBagDefaultMaxSize(bagId);
	int curSize = getItemBagSize(bagId);
	int maxSize = getItemBagMaxSize(bagId);

	mpItem[bagId] = "module/frame/base_bag"->CreateBag(usernum, bagId, maxSize);
}

private void initItemFrames()
{
	mapping mpItem = getItemFrame();
	// mpItem[KT_ITEM_OWNER] = usernum ;

	foreach (int bagId in ITEM_BAGS) {
		initItemFrame(bagId);
	}
}

// 创建一个新的装备栏
int createSummonEquipFrames(string globalKey)
{
	mapping mpItem = getItemFrame();

	int i = ITEM_BAG_EQUIP + 1;
	while (i < ITEM_BAG_EQUIP + ITEM_BAG_REGION) {
		if (undefinedp(mpItem[i])) {
			initItemFrame(i);
			mpItem[i]["summGblKey"] = globalKey;
			return i;
		}

		if (sizeof(mpItem[i]["summGblKey"])) {
			// TODO: 如果召唤兽不存在，那么返回
		}
		i++;
	}
	return 0;
}

private void DestItemFrames()
{
	mapping mpItem = getItemFrame();

	if (!sizeof(mpItem)) {
		return;
	}

	foreach( int _bagId, mapping _bagData in mpItem) {
		mapping Items = Import("BASE_BAG")->GetGrids(_bagData);
		foreach (int pos, int itemIdId in Items) {
			destroy_item(itemIdId);
		}
	}
}

// 将某包裹的物品发给客户端
void sendBagItem(int bagId)
{
	mapping Items = Import("BASE_BAG")->GetGrids( getItemFrame()[bagId] );	
	// rsend to client
	foreach (int pos, int itemIdId in Items)
	{
		Import("ITEM_UTIL")->rpc_AddItemToUser( usernum, itemIdId);
	}
}


private void sendAllItems()
{
	mapping mpFrame = getItemFrame();

	foreach( int _bagId, mapping _bagData in mpFrame) {
		int curSize = getItemBagSize(_bagId);
		int maxSize = Import("BASE_BAG")->GetBagSize(_bagData);
		rpc_client_itembag_size(usernum, _bagId, curSize, maxSize);

		sendBagItem(_bagId);
	}
}


// 不叠加，单纯往Bag里添加物品
private varargs int AddItem(int itemId, int bagId, int pos)
{
	mapping mpFrame = getItemFrame();

	if (!bagId) bagId = ITEM_BAG_NORMAL; 
	assert( itemId > 0 );
	// 对Owner的检测？ TODO
	assert( (!GetItemTemp(itemId, KT_ITEM_OWNER)) || GetItemTemp(itemId, KT_ITEM_OWNER) == usernum );

	pos = Import("BASE_BAG")->AddToBag(itemId, mpFrame[bagId], pos);
	// 设置pos
	if (pos > 0 )
	{
		int _bagId, _pos;
		_bagId = GetItemTemp( itemId, KT_ITEM_BAG);
		_pos = GetItemTemp( itemId, KT_ITEM_POS);

		debug_message(sprintf("AddItem BagPos %d from (%d,%d,%d) to (%d,%d,%d)", itemId, GetItemTemp(itemId, KT_ITEM_OWNER), _bagId,_pos, usernum, bagId, pos));
		if ( _bagId && _pos )
		{
			RemoveItem(itemId, _bagId);		
		}

		SetItemTemp( itemId, KT_ITEM_BAG, bagId );  
		SetItemTemp( itemId, KT_ITEM_POS, pos ); 

		SetItemTemp( itemId, KT_ITEM_OWNER, usernum ); 
		// add global key TODO

		//维护item的dirty map
		addSaveItem(bagId, pos, itemId);

		Import("ITEM_UTIL")->rpc_AddItemToUser(usernum, itemId);
	}

	return pos;
}

varargs int AddRestoreItem(int itemId, int bagId, int pos)
{
	mapping mpFrame = getItemFrame();

	// debug_message(sprintf("add restore itemId:%d,bag:%d,pos:%d\n", itemId, bagId, pos));

	if (!bagId) bagId = ITEM_BAG_NORMAL;
	// 对Owner的检测？ TODO
	assert ( (!GetItemTemp(itemId, KT_ITEM_OWNER)) );

	pos = Import("BASE_BAG")->AddToBag(itemId, mpFrame[bagId], pos);
	// 设置pos
	if (pos > 0 )
	{
		SetItemTemp( itemId, KT_ITEM_BAG, bagId );  
		SetItemTemp( itemId, KT_ITEM_POS, pos ); 

		SetItemTemp( itemId, KT_ITEM_OWNER, usernum ); 

		// Import("ITEM_UTIL")->rpc_AddItemToUser(usernum, itemId);
	}

	return pos;
}


varargs int _XAddItem(int itemId, int bagId, int posFrom, int posTo)
{
	int _id, maxAmount, amount, iLeft, iRes, i, _Free;
	string cType; 
	mapping mpIds = ([]);

	if (!bagId) bagId = ITEM_BAG_NORMAL;
	if ( !posFrom ) posFrom = 1;
	int iMaxSize = getItemBagSize(bagId);
	if ( !posTo || posTo > iMaxSize ) posTo = iMaxSize;

	// 同一个cType的物品，MaxAmount一定相同
	maxAmount = GetItemTemp(itemId, KT_ITEM_MAXAMOUNT);
	cType = GetItemSave(itemId, K_ITEM_TYPE);
	amount = GetItemSave(itemId, K_ITEM_AMOUNT);
	iLeft = amount; // 默认一定是1

	if ((ITEM_PATH + cType)->isBaoshi()) {
		bagId = ITEM_BAG_BAOSHI;
	}


	if ( maxAmount > 1 )
	{
		for ( i=posFrom; i<=posTo; i++)
		{
			_id = getItemByPos(bagId, i);

			if ( _id && Import("ITEM_UTIL")->canMerge(_id, itemId) )
			{
				_Free = (maxAmount - GetItemSave( _id, K_ITEM_AMOUNT));

				if (_Free >= iLeft)
				{
					mpIds[_id] = iLeft;
					iLeft = 0;
					break;
				}

				mpIds[_id] = _Free;
				iLeft -= _Free;
			}
		}	
	}

	// 两种条件会跑进这个逻辑：
	// 1、可叠加但放不下？
	// 2、是不可叠加物品
	if ((iLeft > 0 && iLeft <= maxAmount) || maxAmount <= 1)
	{
		for ( i=posFrom; i<=posTo; i++)
		{
			// 按不可叠加规则加一次  
			iRes = AddItem(itemId, bagId, i);
			if (iRes > 0) return iRes;
		}	

		return 0;
	}

	foreach ( _id, _Free in mpIds )
	{
		Import("ITEM_UTIL")->IncAmount(_id, _Free);
	}

	Import("ITEM_UTIL")->DecAmount(itemId, amount);
	// 返回个magic number，表示做了合并处理
	return MAGIC_NUMBER_OK; 
}


varargs int XAddItem(int itemId, int bagId, int posFrom, int posTo)
{
	int iRes = _XAddItem(itemId, bagId, posFrom, posTo);

	// 落袋动画
	/*
	if (iRes > 0 && !posFrom && !posTo) {
		rpc_client_play_add_item_effect(usernum, cType, amount);

		//if (Import("ITEM_UTIL")->IsEquip(cType)) {
		//	Import("CMD_TASK")->ShowGuideIcon(this_object(), "宝盒");
		//}
	}
	*/
	return iRes;
}


varargs int RemoveItem(int itemId, int inputBagId)
{
	int iRes, bagId, pos;

	pos = GetItemTemp(itemId, KT_ITEM_POS);
	bagId = GetItemTemp(itemId, KT_ITEM_BAG);

	// 这里有些逻辑校验 TODO
	assert( pos > 0 );
	assert(bagId > 0);
	if (inputBagId) assert(bagId == inputBagId);

	xassert(GetItemTemp(itemId, KT_ITEM_OWNER) == usernum, GetItemTemp(itemId, KT_ITEM_OWNER));
	// debug_message(sprintf("RemoveItem: pos=%d, FindItem=%d", pos, FindItem(itemId, bagId)) ); 

	iRes = Import("BASE_BAG")->RemoveByPos( getItemFrame()[bagId], pos );
	if (iRes)
	{
		SetItemTemp( itemId, KT_ITEM_BAG, 0);
		SetItemTemp( itemId, KT_ITEM_POS, 0);
		SetItemTemp( itemId, KT_ITEM_OWNER, 0);

		removeSaveItem(bagId, pos);
		rpc_client_item_del(usernum, itemId);
	}
	return iRes;
}

int GetEmptyPos(int bagId)
{
	mapping mpFrame = getItemFrame(); 
	if (!bagId) bagId = ITEM_BAG_NORMAL; 

	return Import("BASE_BAG")->GetEmptyPos(mpFrame[bagId]);
}


int MoveItem(int itemId, int iBagTo, int posTo)
{

	// RemoveItem(itemId);
	return AddItem(itemId, iBagTo, posTo);
}

int SwapItem(int itemId, int iTarget)
{
	int iBagFrom, posFrom, iBagTo, posTo;

	iBagFrom = GetItemTemp(itemId, KT_ITEM_BAG);
	posFrom = GetItemTemp(itemId, KT_ITEM_POS);

	iBagTo = GetItemTemp(iTarget, KT_ITEM_BAG);
	posTo = GetItemTemp(iTarget, KT_ITEM_POS);

	// debug_message( sprintf("0x%x(%d,%d) swap with 0x%x(%d,%d)", itemId, GetItemTemp(itemId, KT_ITEM_BAG), GetItemTemp(itemId, KT_ITEM_POS), iTarget, GetItemTemp(iTarget, KT_ITEM_BAG), GetItemTemp(iTarget, KT_ITEM_POS)) );

	RemoveItem(itemId);
	RemoveItem(iTarget);
	AddItem(itemId, iBagTo, posTo);
	return AddItem(iTarget, iBagFrom, posFrom);
}

// 2 add into 98, => 1 and 99
// TODO
int MergeItem(int itemId1, int itemId2)
{
	int maxAmount, iDelta, amount1, amount2;

	maxAmount = GetItemTemp(itemId1, KT_ITEM_MAXAMOUNT); 
	amount1 = GetItemSave(itemId1, K_ITEM_AMOUNT);
	amount2 = GetItemSave(itemId2, K_ITEM_AMOUNT);

	iDelta = amount1 + amount2 - maxAmount;

	// debug_message( sprintf("MergetItem: %d[%d]+%d[%d]=(iDelta)%d+(maxAmount)+%d", itemId1, amount1, itemId2, amount2, iDelta, maxAmount) );

	if ( iDelta < 1 )
	{
		Import("ITEM_UTIL")->IncAmount(itemId2, amount1);
		Import("ITEM_UTIL")->DecAmount(itemId1, amount1);
	}
	else
	{
		Import("ITEM_UTIL")->SetAmount(itemId2, maxAmount);
		Import("ITEM_UTIL")->SetAmount(itemId1, iDelta);
	}	

	return iDelta;
}	

// 获取包裹的所有物品
int *getAllItemsInBag(int bagId)
{
	if (!bagId) bagId = ITEM_BAG_NORMAL; 
	return Import("BASE_BAG")->GetGridValues( getItemFrame()[bagId] );	
}

// 根据包裹号和位置获取物品ID
int getItemByPos(int bagId, int pos)
{
	mapping items = Import("BASE_BAG")->GetGrids(getItemFrame()[bagId]);
	return items[pos];
}


