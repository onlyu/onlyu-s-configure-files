#include <npc_key.h>
#include <menu.h>
#include <user_key.h>
#include <frames.h>
inherit "char/npc/npc_base";
#include "/rc/rpc/shop.h"

static mapping mpNpcGoods = (
	[
		17:
		([
			"ZH0001":(["pos":1, "maxAmount":100, "price":10]),
			"EQ0001":(["pos":2, "maxAmount":100, "price":10]),
		])
	]
);

static mapping *getCBFunc()
{
        //debug_message("请继承此方法");
        return ({});
}

mapping getGoods(int npcId)
{
	mapping npcGoods =  mpNpcGoods[npcId];
	if (undefinedp(npcGoods))
		return ([]);
	return npcGoods;
}

void sendGoodsList(object user, int npcId, mapping mpNpcGoods )
{
	int size = sizeof(mpNpcGoods);
	if (!size)
		return;

	class ShopGoods* goodsList = allocate(size);
	class ShopGoods tmpGoods;
	int index = 0;
	foreach( string ctype, mapping goodsInfo in mpNpcGoods)
	{
		tmpGoods = new( class ShopGoods );
		//debug_message(sprintf("ctype goods:%s", ctype));
		tmpGoods->ctype = ctype;
		tmpGoods->price = goodsInfo["price"];
		tmpGoods->maxAmount = goodsInfo["maxAmount"];
		tmpGoods->pos = goodsInfo["pos"];
		tmpGoods->price = goodsInfo["price"];
		tmpGoods->name = getItemName(ctype);
        	tmpGoods->desc = getItemDesc(ctype);
		tmpGoods->icon = getItemIcon(ctype);
		goodsList[index++] = tmpGoods;
	}
	rpc_client_goods_list(user->getId(), npcId, goodsList);
	//debug_message(sprintf("send shop godds list %O", goodsList));

}

void buyGoods(object user, int npcId, string ctype, int amount)
{
	//debug_message(sprintf("enter buygoods npcId=%d ctype=%s amount=%d ", npcId, ctype, amount));
	if(amount <= 0 || npcId <= 0 || !ctype)
		return;
	int uniqNpcId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
	if (!uniqNpcId)
		return;
	mapping mpGoodsInfo = mpNpcGoods[uniqNpcId][ctype];
	//debug_message(sprintf("buygoods npcId=%d ctype=%s amount=%d uniqNpcId=%d mpGoodsInfo=%O", npcId, ctype, amount, uniqNpcId, mpNpcGoods));
	if( undefinedp(mpGoodsInfo) )
		return;

	if (amount > mpGoodsInfo["maxAmount"])
		return;

	int totalPrice = mpGoodsInfo["price"] * amount;
	int userMoneyBeforeBuy = user->getProp(K_USER_CASH);
	if (userMoneyBeforeBuy < totalPrice)
	{
		user->tellMe("您的钱数不足");
		return;
	}
	user->subCash(totalPrice);
	Import("ITEM_UTIL")->addNewItem(user, ctype, amount, ITEM_BAG_NORMAL);
	Import("ITEM_UTIL")->sendAddItemEffect(user, ctype, amount);
	//debug_message("leave buygoods");
}

void doClick(object user, int npcId)
{
	//todo
	//check if user is within the valid range of the npc
        int uniqId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
        if( !uniqId ) 
		return;
	mapping npcGoods = getGoods(uniqId);
	//debug_message("npcGoods npcId=%d, goods:%O", npcId, npcGoods);
	sendGoodsList(user, npcId, npcGoods);
}


