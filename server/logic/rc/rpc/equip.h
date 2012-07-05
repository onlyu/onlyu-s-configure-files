#ifndef _rc_rpc_equip
#define _rc_rpc_equip

#include "/rc/rpc/rpc_id.h"


// 穿上装备
void rpc_server_wield_equip(object user, int item, int bagId, int locate);
void rpc_server_unwield_equip(object user, int item, int pos);

class equip_value_t {
	string key;
	int value;
}

void rpc_client_equip_item_info(int uid, int item, int qianghuaCnt, class equip_value_t *values, string *baoshis) { fs_rpc_call(uid, RPC_CLIENT_EQUIP_ITEM_INFO, item, qianghuaCnt, values, baoshis); }

// 装备相关的信息
void rpc_client_equip_desc(int uid, string type, int grade, int qianghuaVal, int locate, int color, class equip_value_t *baseValues) { fs_rpc_call(uid, RPC_CLIENT_EQUIP_DESC, type, grade, qianghuaVal, locate, color, baseValues); }

// 强化装备
void rpc_server_equip_qianghua(object user, int item);

// 登录时发
void rpc_client_equip_qianghua_cd(int uid, int sec, int maxSec) { fs_rpc_call(uid, RPC_CLIENT_EQUIP_QIANGHUA_CD, sec, maxSec); }

// 强化
void rpc_client_equip_qianghua_info(int uid, int item, int cash) { fs_rpc_call(uid, RPC_CLIENT_EQUIP_QIANGHUA_INFO, item, cash); }
void rpc_server_equip_qianghua_info_req(object user, int item);

// 洗练
// normalCash : 普通洗练金钱
// directCash : 定向洗练金钱
void rpc_client_equip_xilian_info(int uid, int item, int normalCash, int directCash, class equip_value_t *attr) { fs_rpc_call(uid, RPC_CLIENT_EQUIP_XILIAN_INFO, item, normalCash, directCash, attr); }

// 洗练按钮
void rpc_server_equip_xilian_normal(object user, int item);
void rpc_server_equip_xilian_direct(object user, int item);
void rpc_server_equip_xilian_replace_attr(object user, int item);

// 嵌上宝石
void rpc_server_equip_add_baoshi(object user, int equipItem, int baoshiItem);
// 卸下宝石
void rpc_server_equip_remove_baoshi(object user, int equipItem, string baoshiType);
// 合并宝石
void rpc_server_equip_merge_baoshi(object user, int baoshiItem, int cnt);
// 宝石升级
void rpc_server_baoshi_upgrade(object user, int equipItem, string baoshiType);
// 将宝石更新到grade对应的等级
void rpc_server_baoshi_batch_upgrade(object user, int grade);
// 装备继承
void rpc_server_equip_cash_transfer(object user, int equipItem, int transferItem);

// 获得宝石
void rpc_server_baoshi_fetch(object user, int cnt);
// 免费获得宝石
void rpc_server_baoshi_free_fetch(object user);
// 免费个数
void rpc_client_baoshi_free_fetch_cnt(int uid, int cnt) { fs_rpc_call(uid, RPC_CLIENT_BAOSHI_FREE_FETCH_CNT, cnt); }
#endif
