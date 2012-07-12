// 任务模版---通关副本
inherit "module/mission/base";

#include "/module/mission/mission.h"
#include "/module/fuben/base.h"

#define FINISH_FUBEN_AMOUNT "finishFubenAmount"

int getState(object user)
{
        int state = ::getState(user);
        if( state == MISSION_STATE_PROGRESS ) {
                int totalAmount = getExtData(user, FINISH_FUBEN_AMOUNT);

                if( intp(totalAmount) && totalAmount >= getTargetNum() ) {
                        return MISSION_STATE_READY;
                }
        }

	return state;
}

// 当前已完成目标次数
int curTargetNum(object user)
{
	return min(getExtData(user, FINISH_FUBEN_AMOUNT), getTargetNum());
}

// 副本结束
void onFubenEnd(object user, class Fuben fuben)
{
        if( !Import("FUBEN_BASE")->isDone(user) ) return;
	if( getFubenId() != fuben->id ) return;

        int totalAmount = getExtData(user, FINISH_FUBEN_AMOUNT);
        if( undefinedp(totalAmount) ) totalAmount = 0;

        totalAmount++;
        setExtData(user, FINISH_FUBEN_AMOUNT, totalAmount);
        MISSION_MAIN->renewMission(user, getId());
}
