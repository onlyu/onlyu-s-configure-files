/*
所有任务都必需先分派了，再接受
*/

/*
任务基类

任务要素：
1.任务id

2.任务名称

3.任务类型(主线、支线、精英)

4.任务目的(找NPC,通关副本次数，杀怪次数)

5.战斗配置( {"enemy":"怪物名字","count":战胜次数}或{"copy":"副本id","count":通关次数} )

6.按任务NPC

7.交任务NPC

8.最低等级(默认无)

9.最高等级(默认无)

10.需要变身编号(默认无)

11.接任务时的对话

12.交任务时的对话

13.任务描述

14.奖励( {"money":1000,"exp":2000,"item":{"ZH0001":10,"ZH0003":3}} )

//15.最多完成次数，默认为1次，-1为不限制

任务状态：
未接受

进行中(已接受)

已完成

*/

#include <user_key.h>
#include <debug.h>
#include "mission.h"

//----------------任务基本信息

//任务id
string getId() { return ""; }

//任务名称
string getName() { return "base mission"; }

//任务类型
int getType() { return TYPE_MAIN; }

//上一个任务id
string getPreId() { return 0; }

//下一个任务id
string *getNextIds() { return 0; }

// 任务目标
string getTemplate() { return 0; }

// 副本id
int getFubenId() { return 0; }

// 目标名称
string getEnemyName() { return 0; }

// 目标数量
int getTargetNum() { return 0; }

//接受任务NPC
int getAcceptNpc() { return 0; }

//交任务NPC
int getCompleteNpc() { return 0; }

//任务追踪NPC
int getFollowNpc() { return 0; }

//最低等级
int getMinGrade() { return 0; }

//最高等级
int getMaxGrade() { return 0; }

//能否放弃任务
int canCancel() { return 0; }

//需要变身编号 
int getCharId() { return 0; }

//接任务时的对话
string getAcceptTalk() { return "acceptTalk"; }

//接任务时的选项文字
string getAcceptOption() { return "acceptOption"; }

//交任务时的对话
string getCompleteTalk() { return "completeTalk"; }

//交任务时的选项文字
string getCompleteOption() { return "completeOption"; }

//任务追踪的文字
string getFollowText() { return "followText"; }

//完成任务隐藏的NPC
int *getHideNpcOnComplete() { return 0; }

//完成任务显示的NPC
int *getShowNpcOnComplete() { return 0; }

//任务描述
string getDesc() { return "base desc"; }

//金钱奖励
int getMoney() { return 0; }

//经验奖励
int getExp() { return 0; }

//物品奖励
mapping getItems() { return 0; }


// 从导表数据里填充数据
/*int init(string missId, mapping data)
{
        id = missId;

        if( !undefinedp(data["name"]) ) name = data["name"];
        if( !undefinedp(data["type"]) ) type = data["type"];
        if( !undefinedp(data["preId"]) ) preId = data["preId"];
        if( !undefinedp(data["nextIds"]) ) nextIds = data["nextIds"];
        if( !undefinedp(data["template"]) ) template = data["template"];
        if( !undefinedp(data["fightInfo"]) ) fightInfo = data["fightInfo"];
        if( !undefinedp(data["acceptNpc"]) ) acceptNpc = data["acceptNpc"];
        if( !undefinedp(data["completeNpc"]) ) completeNpc = data["completeNpc"];
        if( !undefinedp(data["followNpc"]) ) followNpc = data["followNpc"];
        if( !undefinedp(data["minGrade"]) ) minGrade = data["minGrade"];
        if( !undefinedp(data["maxGrade"]) ) maxGrade = data["maxGrade"];
        if( !undefinedp(data["cancel"]) ) cancel = data["cancel"];
        if( !undefinedp(data["charId"]) ) charId = data["charId"];
        if( !undefinedp(data["acceptTalk"]) ) acceptTalk = data["acceptTalk"];
        if( !undefinedp(data["acceptOption"]) ) acceptOption = data["acceptOption"];
        if( !undefinedp(data["completeTalk"]) ) completeTalk = data["completeTalk"];
        if( !undefinedp(data["completeOption"]) ) completeOption = data["completeOption"];
        if( !undefinedp(data["desc"]) ) desc = data["desc"];
        if( !undefinedp(data["followText"]) ) followText = data["followText"];
        if( !undefinedp(data["rewardInfo"]) ) rewardInfo = data["rewardInfo"];
        if( !undefinedp(data["dramaId"]) ) dramaId = data["dramaId"];
}*/
//----------------end 任务基本信息



//----------------任务数据操作
//清除任务裸数据
static void cleanMissionData(object user)
{
        MISSION_DATA->cleanMissionData(user, getId());
}

//任务额外相关数据,人物身上的跟任务相关的数据，在具体应该时可以自主使用
static mixed getExtData(object user, mixed key)
{
        return MISSION_DATA->getMissionExtData(user, getId(), key);
}
static int setExtData(object user, mixed key, mixed value)
{
        return MISSION_DATA->setMissionExtData(user, getId(), key, value);
}


//任务进度相关数据, 人物身上的跟任务相关的数据,例如完成状态、完成时间等
static mixed getBaseData(object user, mixed key)
{
        return MISSION_DATA->getMissionBaseData(user, getId(), key);
}
static int setBaseData(object user, mixed key, mixed value)
{
        return MISSION_DATA->setMissionBaseData(user, getId(), key, value);
}

static void initMissionData(object user)
{
        MISSION_DATA->initMissionData(user, getId());
}

//----------------end 任务数据操作



//是否有这个任务
int hasMission(object user)
{
        int acceptTime = getBaseData(user, MISSION_DATA_ACCEPT_TIME);
        return !undefinedp(acceptTime) ? 1 : 0;
}

int getAcceptTime(object user)
{
        int acceptTime = getBaseData(user, MISSION_DATA_ACCEPT_TIME);
        return !undefinedp(acceptTime) ? acceptTime : -1;
}

//任务状态
int getState(object user)
{
        if( !hasMission(user) ) return MISSION_STATE_NULL;

        int completeTime = getBaseData(user, MISSION_DATA_COMPLETE_TIME);
        if( completeTime ) return MISSION_STATE_COMPLETED;

        return MISSION_STATE_PROGRESS;
}

//是否处于就绪状态
int isReady(object user)
{
        if( getState(user) == MISSION_STATE_READY ) return 1;
        return 0;
}

//是否处于任务过程状态
int isProgress(object user)
{
        if( getState(user) == MISSION_STATE_PROGRESS ) return 1;
        return 0;
}

//是否已完成
int isComplete(object user)
{
        if( getState(user) == MISSION_STATE_COMPLETED ) return 1;
        return 0;
}

// 基本的接受任务条件(只检测基本的等级规则)
int canBaseAccept(object user)
{
        //等级判断
        int grade = user->getGrade();
        int minGrade = getMinGrade();
        int maxGrade = getMaxGrade();

        if( minGrade && grade < minGrade ) return 0;
        if( maxGrade && grade > maxGrade ) return 0;

        return 1;
}

//能否接受任务
int canAccept(object user)
{
        if( hasMission(user) ) return 0;
	if( !canBaseAccept(user) ) return 0;

        return 1;
}


//接受任务
int accept(object user)
{
        if( hasMission(user) || !canAccept(user) ) return 0;

        initMissionData(user);
        setBaseData(user, MISSION_DATA_ACCEPT_TIME, time());
	return 1;
}

// 当前已完成目标次数
int curTargetNum(object user)
{
	return 0;
}

string getFollowDesc(object user)
{
	int total = getTargetNum();
	if( total > 0 && (isProgress(user) || isReady(user)) ) {
		return getFollowText() + sprintf("(%d/%d)", curTargetNum(user), total);
	} else {
		return getFollowText();
	}
}

mapping getFollowTarget(object user)
{
        int state = getState(user);
        int uniqId;
        //根据任务状态生成任务追踪
        switch( getState(user) ) {
                case MISSION_STATE_PROGRESS:
                        uniqId = getFollowNpc();
                        break;
                case MISSION_STATE_READY:
                        uniqId = getCompleteNpc();
                        break;
                default:
                        uniqId = getAcceptNpc();
                        break;

        }

        int npcId = Import("UNIQ_NPC")->systemId2NpcId(uniqId);
	if( npcId ) {
		int *pos = Import("SCENE")->getNpcLocation(npcId);
		return ([ "sceneId": pos[0], "dx": pos[1], "dy": pos[2], "npcId": npcId, "fubenId":getFubenId() ]);
	} else {
		return ([]);
	}
}

//放弃任务
int cancelMission(object user)
{
        if( hasMission(user) ) {
                cleanMissionData(user);
                return 1;
        }
        return 0;
}

//删除任务 任务完成时删除
int delMission(object user)
{
        cleanMissionData(user);
        return 1;
}

//发放奖励
static int giveReward(object user);

//尝试完成任务
int tryComplete(object user, int npcId)
{
        if( !isReady(user) ) return 0;

        // 设置完成标记
        setBaseData(user, MISSION_DATA_COMPLETE_TIME, time());
        return 1;
}

// 战斗结束
void onFightEnd(object user, mixed fight)
{
}

// 副本结束
void onFubenEnd(object user,  mixed fuben)
{

}

void loginCheck(object user)
{

}

