#ifndef _rc_rpc_mission
#define _rc_rpc_mission

#include "/rc/rpc/rpc_id.h"


#define  S_TRACK_ASSIGN        1
#define  S_TRACK_ACTION        2
#define  S_TRACK_FINISH        3

class MissionFollowTarget {
        string id;
        int sceneId;
        int dx;
        int dy;
        int npcId;
	int fubenId;
}


class MissionInfo {
        string id;
        int type;
        int state;
	int acceptTime;
        string name;
        string followText;
        string desc;

        // 一个任务只支持一个追踪目标
        class MissionFollowTarget followTarget;
}


// 所有已接、可接任务
// void rpc_server_all_missions(object user);
// void rpc_client_all_missions(int uid, class MissionInfo *missions) { fs_rpc_call(uid, RPC_CLIENT_ALL_MISSIONS, missions); }


// 接受任务
void rpc_client_accept_mission(int uid, class MissionInfo mission) { fs_rpc_call(uid, RPC_CLIENT_ACCEPT_MISSION, mission); }

// 放弃任务 玩家主动放弃
void rpc_server_cancel_mission(object user, string missId);
void rpc_client_cancel_mission(int uid, string missId) { fs_rpc_call(uid, RPC_CLIENT_CANCEL_MISSION, missId); }

// 删除任务 系统删除
void rpc_client_del_mission(int uid, string missId) { fs_rpc_call(uid, RPC_CLIENT_DEL_MISSION, missId); }

// 更新任务
void rpc_client_update_mission(int uid, class MissionInfo mission) { fs_rpc_call(uid, RPC_CLIENT_UPDATE_MISSION, mission); }

// 增加可接任务
void rpc_client_add_canaccept_mission(int uid, class MissionInfo mission) { fs_rpc_call(uid, RPC_CLIENT_ADD_CANACCEPT_MISSION, mission); }

// 删除可接任务
void rpc_client_del_canaccept_mission(int uid, string missId) { fs_rpc_call(uid, RPC_CLIENT_DEL_CANACCEPT_MISSION, missId); }

// 完成任务
void rpc_client_complete_mission(int uid, string missId) { fs_rpc_call(uid, RPC_CLIENT_COMPLETE_MISSION, missId); }
#endif
