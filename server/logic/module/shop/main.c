
#define SHOP_NPC_ID 5

void rpc_server_buygoods(object user, int npcid, string ctype, int amount)
{
	"char/npc/template/shop"->buyGoods(user,npcid,ctype, amount);
}

void rpc_server_shop_shortcut(object user)
{
	int vipLvl = 1;
	if (vipLvl <= 0)
		return;
	
	mapping shopNpcGoods = "char/npc/template/shop"->getGoods(SHOP_NPC_ID);
	"char/npc/template/shop"->sendGoodsList(user, SHOP_NPC_ID, shopNpcGoods);
	
}
