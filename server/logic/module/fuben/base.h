#ifndef __FUBEN_BASE__
#define __FUBEN_BASE__
#include <user_key.h>
#include <var_prop.h>
#include </rc/rpc/fuben.h>
#include "/module/fight/fight.h"
class MingLei
{
	int id;
	int x;
}

class Fuben 
{
	int id;
	int sceneId;
	int startTime;
	int* fromLocations;
	string name;
	string wanfaModule;
	class MingLei* mingLeiList;
	int nextMingLeiIdx;
}

#define GET_FUBEN(user)  ((user)->getTemp(KT_FUBEN))
#define FB_SCENE "scene"
#define FB_MULTI_PLAYER "multiPlayer"
#define FB_NAME  "fbName"
#define FB_ML_NAME "name"
#define FB_X     "x"
#define FB_Y	 "y"
#define FB_WANFA_MODULE "wanfa"
#define FB_POSITION "position"
#define FB_LAYOUT "layout"
#define FB_ROAM_WIDTH 200
#define FB_ROAM_HEIGHT 350
#define FB_ICON "icon"
#define FB_FIRE_FIGHT_END "战斗结束"
#define FB_FIRE_START "进入副本"
#define FB_WIN "胜"
#define FB_LOST "败"
#define FB_ACTION_PLAY "play"
#define FB_FIRE "fire"
#define FB_FIRE_ARG "fireArg"
#define FB_ACTION "action"
#endif 
