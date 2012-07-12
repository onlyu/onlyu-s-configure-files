
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
// void rpc_client_all_missions(int uid, class MissionInfo *missions);


// 接受任务
void rpc_client_accept_mission(int uid, class MissionInfo mission);

// 放弃任务 玩家主动放弃
void rpc_server_cancel_mission(object user, string missId);
void rpc_client_cancel_mission(int uid, string missId);

// 删除任务 系统删除
void rpc_client_del_mission(int uid, string missId);

// 更新任务
void rpc_client_update_mission(int uid, class MissionInfo mission);

// 增加可接任务
void rpc_client_add_canaccept_mission(int uid, class MissionInfo mission);

// 删除可接任务
void rpc_client_del_canaccept_mission(int uid, string missId);

// 完成任务
void rpc_client_complete_mission(int uid, string missId);
