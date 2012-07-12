#ifndef _rc_rpc_shop
#define _rc_rpc_shop

#include "/rc/rpc/rpc_id.h"



class ShopGoods
{
	string ctype;
	int maxAmount;
	int pos;
	int price;
	int icon;
	string name;
	string desc;
}

void rpc_client_goods_list(int usernum, int npcid, class ShopGoods* shopGoodsList) { fs_rpc_call(usernum, RPC_CLIENT_GOODS_LIST, npcid, shopGoodsList); }
void rpc_server_buygoods(object user, int npcid, string ctype, int amount);

void rpc_server_shop_shortcut(object user);
#endif
