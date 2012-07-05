#ifndef _MISSION_H_
#define _MISSION_H_

#define MISSION_UTIL "module/mission/util"
#define MISSION_MAIN "module/mission/main"
#define MISSION_DATA "module/mission/data"

#define MISSION_BASE "module/mission/base"
#define MISSION_FIND_NPC "module/mission/template/find_npc"
#define MISSION_KILL_NPC "module/mission/template/kill_npc"
#define MISSION_FINISH_FUBEN "module/mission/template/finish_fuben"

// 仅用于显示
#define TYPE_MAIN 1 //主线
#define TYPE_EXT 2 //支线
#define TYPE_ELITE 3 //精英

#define MISSION_STATE_NULL 1 //没接受任务
#define MISSION_STATE_ASSIGN 2 //已分派
#define MISSION_STATE_PROGRESS 3 //任务过程中
#define MISSION_STATE_READY 4 //任务已就绪，可以交任务
#define MISSION_STATE_COMPLETED 5 //已完成

#define MISSION_DATA_ASSIGN_TIME "asign_time"
#define MISSION_DATA_ACCEPT_TIME "accept_time"
#define MISSION_DATA_COMPLETE_TIME "complete_time"

#endif
