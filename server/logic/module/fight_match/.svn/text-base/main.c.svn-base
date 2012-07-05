#include "/module/fight/fight.h"


// 
// 1. 玩家每天总共能进行15次竞技场战斗
// 2. 7点半刷新竞技战斗
// 3. 每进行一次战斗后，要间隔3分钟才可以进行下一次竞技场战斗。玩家可以花费元宝清除等待cd。
// 4. 每战胜一个对手，则名次提升为对手名次，对手名次降低为原来的名次减1。


// 每天总共最多能进行的竞技战斗数
#define   MAX_DAY_FIGHT_CNT             15
// 刷新时间
#define   REFRESH_TIME                  "19:30:00"
// 进入战斗CD
#define   FIGHT_CD                      (3 * 60)
// 清除战斗CD的元宝
#define   CLEAR_FIGHT_CD_YB             (3)


int canFight(object user)
{
	// 战斗CD
	return 1;
}

// 异步load
void doChallengeFight(object target, int uid)
{
	object user = get_user(uid);
	if (!objectp(user)) {
		return;
	}

	int tid = user->getId();

	mixed * attack = Import("FIGHT")->userCamp(user);
	mixed * defense = Import("FIGHT")->userCamp(target);
	class Fight fight = Import("FIGHT")->createFight(user, attack, defense);
	Import("FIGHT")->start(fight);

	Import("FIGHT")->movieStart(fight);

	string record = FIGHT_OUTPUT->record(fight->output);

	int win = fight->result->win;
	int lose = (win == uid) ? tid : uid;

	"module/fight/fight_match"->fightResult(win, lose);

	// TODO: 录像存下来
	debug_message("%d, %d, record: %s", uid, tid, record);
}


// 尝试挑战
void tryChallengeFight(object user, int targetId)
{
	if (!canFight(user)) {
		return;
	}
	debug_message("async load: %d", targetId);
	Import("DB")->userobj_aop(targetId, (:doChallengeFight:), user->getId());
}


// 给战魂经验
void addPuppetExp(int uid, int puppet, int lastTime)
{
	// TODO:
}
