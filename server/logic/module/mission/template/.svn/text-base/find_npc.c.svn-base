// 任务模版---寻找NPC
inherit "module/mission/base";

#include "/module/mission/mission.h"

// 拜访NPC的任务，接受后直接可以完成
int getState(object user)
{
        int state = ::getState(user);
        if( state == MISSION_STATE_PROGRESS ) return MISSION_STATE_READY;

        return state;
}


