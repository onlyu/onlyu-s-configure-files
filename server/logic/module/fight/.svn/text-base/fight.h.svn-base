// 战斗头文件
// ---------------------------------------

#ifndef __FIGHT_H__
#define __FIGHT_H__

#include "/module/fight/warrior.h"
#include "/rc/rpc/fight.h"

#define FIGHT_MAIN "module/fight/main"
#define FIGHT_LAYOUT "module/fight/layout"
#define FIGHT_OUTPUT "module/fight/output"

#define FT_SIDE_ATTACK 0
#define FT_SIDE_DEFENSE 1
#define FT_SIDE_SIZE 9

#define POSITION(side, position) ((side) * FT_SIDE_SIZE + (position))
#define SIDE(position) (position <= FT_SIDE_SIZE ? FT_SIDE_ATTACK : FT_SIDE_DEFENSE)

class FightSide {
	mapping warriors;
}

class FightResult {
	int win;
	mapping * death;
}

class Fight {
	int id;
	int sceneId;
	int x;
	int y;
	mapping warriors; // 战斗中没死的战士
	mapping userWarriors; //战斗中所有的玩家战士，包括死亡的
	int *uids;
	class FightSide *side;
	int bout;
	int boutMax;
	int moveBar;
	int moveTime;
	mapping handler;
	class FightResult result;
	mapping output;
	mapping ext;
}


#endif // __FIGHT_H__

