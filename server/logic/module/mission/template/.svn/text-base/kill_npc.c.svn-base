// 任务模版---杀怪
inherit "module/mission/base";

#include "/module/mission/mission.h"

#define KILL_NPC_AMOUNT "killNpcAmount"

int getState(object user)
{
        int state = ::getState(user);
        if( state == MISSION_STATE_PROGRESS ) {
                int totalAmount = getExtData(user, KILL_NPC_AMOUNT);

                if( intp(totalAmount) && totalAmount >= getTargetNum() ) {
                        return MISSION_STATE_READY;
                }
        } else {
                return state;
        }
}

// 当前已完成目标次数
int curTargetNum(object user)
{
	return min(getExtData(user, KILL_NPC_AMOUNT), getTargetNum());
}

// 战斗结束
void onFightEnd(object user, mixed fight)
{
        if( !Import("FIGHT")->isAttackWin(fight) ) return;

	int fubenId = getFubenId();
	if( !fubenId || fubenId != Import("FUBEN_BASE")->getFubenId(user) ) return;

        string key = getEnemyName();
        int amount = Import("FIGHT")->defenseDeadNpcNum(fight, key);

        int totalAmount = getExtData(user, KILL_NPC_AMOUNT);
        if( undefinedp(totalAmount) ) totalAmount = 0;

        totalAmount += amount;
        setExtData(user, KILL_NPC_AMOUNT, totalAmount);
        MISSION_MAIN->renewMission(user, getId());
}
