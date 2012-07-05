class item_pos_t {
        int iBag;
        int iPos;
}

class item_info_t {
        int iItemId;
        //int iBag;
        //int iPos;
        string cType;
        int iAmount;
        int iLockType;
	int iOwner;
}

// 增加个物品 // 或者更新物品属性
class item_add_t {
        class item_pos_t posinfo;
        class item_info_t iteminfo;
}

// 物品移动 // 按鼠标操作分功能，可能是合并，也可能是移动、swap
class item_move_t {
        int iItemId;

        class item_pos_t pos_from;
        class item_pos_t pos_to;
}

// 对物品加解锁
class item_lock_t {
        int iLockType; // 普通锁 0x1, 时间锁 0x2, xx锁 0x4
        int iLockValue; // 0 or 1
}

class item_short_info_t {
		string cType;
		string tips;
}

//增加\删除\移动\合并\拆分\使用\查询物品属性\加解锁\装备

// 查询解锁包括的价格
void rpc_server_itembag_unlockquery(object user, int bagId, int curSize, int unlockToSize);
void rpc_client_itembag_unlockquery(int usernum, int bagId, int curSize, int unlockToSize, int money);

void rpc_server_itembag_unlock(object user, int bagId, int curSize, int unlockToSize);
void rpc_client_itembag_unlock(int usernum, int bagId, int unlockToSize);
// 增加物品
void rpc_client_item_add(int usernum, class item_add_t _add_t);
// 删除物品
void rpc_server_item_del(object oUser, int itemid);
void rpc_client_item_del(int usernum, int itemid);
// 在某BAG里移动物品 合并
void rpc_server_item_move(object oUser, class item_move_t _move_t);
void rpc_client_item_move(int usernum, class item_move_t _move_t);
// 在不同BAG之间转移
void rpc_server_item_bagmove( object oUser, int itemid, int iBagFrom, int iBagTo);
// 分拆物品
void rpc_server_item_split(object oUser, int iItemId, int iAmount);
// 使用物品
void rpc_server_item_use(object oUser, int itemid, int target);
// 查询物品属性
void rpc_server_item_info(object oUser, int itemid);
// 告诉客户端物品状态
void rpc_client_item_info(int uid, class item_short_info_t _info_t);

// 设置物品锁
void rpc_server_set_itemlock(object oUser, class item_lock_t _lock_t);

// 更新某物品数量
void rpc_client_item_amount( int usernum, int itemid, int iAmount);

// 告诉客户端物品栏大小
void rpc_client_itembag_size(int usernum, int bagId, int availSize, int maxSize);

// 更新某物品位置信息
void rpc_client_item_pos( int usernum, int itemid, int iBagId, int iPos );

// 客户端tips
void rpc_client_item_tips( int usernum, int itemid, string tips);

// 请求tips
void rpc_server_item_tips_req(object oUser, int itemid);

void rpc_client_play_add_item_effect(int uid, string type, int amount);

// 物品描述请求
void rpc_server_item_desc_req(object oUser, string type);
// 物品描述
void rpc_client_item_desc(int uid, string type, string name, string desc);

// 背包整理
void rpc_server_item_clear_up(object user);
