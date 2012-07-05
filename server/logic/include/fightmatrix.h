#ifndef __FIGHT_MATRIX__
#define __FIGHT_MATRIX__

#ifndef __FIGHT_GRID_INFO__
#define __FIGHT_GRID_INFO__
class FightGridInfo{
	int pos;
	int type;
	int id;
}
#endif
/*
 *type 1 为人物，2为伙伴 3为空位置，4为锁位置
 *id 为 人物或伙伴的id
 */
#define FIGHT_GRID_HERO 1
#define FIGHT_GRID_SUMMON 2
#define FIGHT_GRID_EMPTY 3
#define FIGHT_GRID_LOCKED 4

#define MAX_FIGHT_MATRIX_POS 9

#endif
