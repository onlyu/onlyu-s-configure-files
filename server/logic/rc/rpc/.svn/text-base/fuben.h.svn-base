#ifndef _rc_rpc_fuben
#define _rc_rpc_fuben

#include "/rc/rpc/rpc_id.h"



class FubenNpc {
	int id;	
	int icon;
	int x;
	int y;
	string name;
	string title;
	int roamWidth;
	int roamHeight;
}

class FubenRewardItem {
	string name;
	string type;
	int icon;
	int amount;
}

class FubenFightResult {
	int win; //1 win 0 lose
	int exp;
	int cash;
	class FubenRewardItem *items;
}

class FubenEntryInfo {
	int fubenId;
	string name;
	int mapId;
	int star;
	int x;
	int y;
}
class FubenEvaluate {
	int star;
	int exp;
	int cash;
	class FubenRewardItem* items;
}

void rpc_client_fuben_start(int uid, int fubenId, string name) { fs_rpc_call(uid, RPC_CLIENT_FUBEN_START, fubenId, name); }
void rpc_client_fuben_end(int uid) { fs_rpc_call(uid, RPC_CLIENT_FUBEN_END); }
void rpc_client_fuben_fight_result(int uid, class FubenFightResult result) { fs_rpc_call(uid, RPC_CLIENT_FUBEN_FIGHT_RESULT, result); }
void rpc_client_fuben_evaluate(int uid, class FubenEvaluate evaluate) { fs_rpc_call(uid, RPC_CLIENT_FUBEN_EVALUATE, evaluate); }
void rpc_server_fuben_exit(object user, int fubenId);
void rpc_server_fuben_continue(object user);
void rpc_server_fuben_fight(object user, int fubenId, int trapId);

void rpc_client_fuben_add_npc(int uid, class FubenNpc * npcs) { fs_rpc_call(uid, RPC_CLIENT_FUBEN_ADD_NPC, npcs); }

//跳转
void rpc_client_transport(object user, int transId, class FubenEntryInfo* fubenEntryList) { fs_rpc_call(user, RPC_CLIENT_TRANSPORT, transId, fubenEntryList); }
void rpc_server_transport(object user, int transId, int fubenId);

void rpc_server_reenter(object user);
#endif
