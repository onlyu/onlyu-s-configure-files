
#include <menu.h>
#include "mission.h"

#define MAX_MISSION_AMOUNT (10)

static mapping missionType2Text = ([
        TYPE_MAIN: "主",
        TYPE_EXT: "支",
        TYPE_ELITE: "精",
]);



// ---------------NPC交互

// 点击确认完成
void onClickCompleteMission(object user, int npcId, string missId)
{
        int sysId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
        if( !sysId ) return;

        object missObj = MISSION_MAIN->getMissionObj(missId);
        if( !objectp(missObj) || missObj->getCompleteNpc() != sysId ) return;

        MISSION_MAIN->tryCompleteMission(user, missId);

        // 尝试继续打开NPC对话框
        string *readyList = MISSION_MAIN->getReadyMissions(user);
        foreach(string _missId in readyList) {
                missObj = MISSION_MAIN->getMissionObj(_missId);
                if( objectp(missObj) && missObj->getCompleteNpc() == sysId ) {
                        "module/scene/npc"->clickNpc(user->getId(), npcId);
                        return;
                }
        }

        string *canAcceptList = MISSION_MAIN->getCanAcceptMissions(user);
        foreach(string _missId in canAcceptList) {
                missObj = MISSION_MAIN->getMissionObj(_missId);
                if( objectp(missObj) && missObj->getAcceptNpc() == sysId ) {
                        "module/scene/npc"->clickNpc(user->getId(), npcId);
                        return;
                }
        }
}

// 点击可完成任务选项
void onClickReadyMission(object user, int npcId, string missId)
{
        int sysId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
        if( !sysId ) return;

        if( !MISSION_MAIN->isReadyMission(user, missId) ) return;

        object missObj = MISSION_MAIN->getMissionObj(missId);
        if( !objectp(missObj) || missObj->getCompleteNpc() != sysId ) return;

        mapping options = initNpcOptions(npcId);

        string completeOption = missObj->getCompleteOption();
        mapping opt = createOption(completeOption, "onClickCompleteMission", ({missId}));
        addOption(options, opt);

        opt = createDefaultOption("完成", "onClickCompleteMission", ({missId}));
        addOption(options, opt);

        string completeTalk = missObj->getCompleteTalk();
        tellOptions(user, options, completeTalk);
}

// 点击确认接受任务
void onConfirmAcceptMission(object user, int npcId, string missId)
{
	if( MISSION_MAIN->getMissionAmount(user) >= MAX_MISSION_AMOUNT ) {
		user->tellMe("您领取的任务已达到%d个上限，请完成后再领取任务！", MAX_MISSION_AMOUNT);
		return;
	}

        int sysId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
        if( !sysId ) return;

        object missObj = MISSION_MAIN->getMissionObj(missId);
        if( !objectp(missObj) || missObj->getAcceptNpc() != sysId ) return;

        MISSION_MAIN->acceptMission(user, missId);

        // 尝试继续打开NPC对话框
        string *readyList = MISSION_MAIN->getReadyMissions(user);
        foreach(string _missId in readyList) {
                missObj = MISSION_MAIN->getMissionObj(_missId);
                if( objectp(missObj) && missObj->getCompleteNpc() == sysId ) {
                        "module/scene/npc"->clickNpc(user->getId(), npcId);
                        return;
                }
        }

        string *canAcceptList = MISSION_MAIN->getCanAcceptMissions(user);
        foreach(string _missId in canAcceptList) {
                missObj = MISSION_MAIN->getMissionObj(_missId);
                if( objectp(missObj) && missObj->getAcceptNpc() == sysId ) {
                        "module/scene/npc"->clickNpc(user->getId(), npcId);
                        return;
                }
        }
}

// 点击可接任务选项
void onClickAcceptMission(object user, int npcId, string missId)
{
        int sysId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
        if( !sysId ) return;

        if( !MISSION_MAIN->canAcceptMission(user, missId) ) return;

        object missObj = MISSION_MAIN->getMissionObj(missId);
        if( !objectp(missObj) || missObj->getAcceptNpc() != sysId ) return;

        mapping options = initNpcOptions(npcId);

        int money = missObj->getMoney();
        int exp = missObj->getExp();
        mapping items = missObj->getItems();
        mapping opt = createRewardOption(money, exp, items);
        addOption(options, opt);

        string acceptOption = missObj->getAcceptOption();
        opt = createOption(acceptOption, "onConfirmAcceptMission", ({missId}));
        addOption(options, opt);

        opt = createDefaultOption("接受", "onConfirmAcceptMission", ({missId}));
        addOption(options, opt);

        string acceptTalk = missObj->getAcceptTalk();
        tellOptions(user, options, acceptTalk);
}

// 点击已接任务选项
void onClickAcceptedMission(object user, int npcId, string missId)
{
        int sysId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
        if( !sysId ) return;

        if( !MISSION_MAIN->hasMission(user, missId) ) return;

        mapping options = initNpcOptions(npcId);
        tellOptions(user, options, "快快去吧");
}

// 点击NPC对话
mapping *onClickNpc(object user, int npcId)
{
        int sysId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
        if( !sysId ) return ({});

        mapping *opts = ({});
	mapping defaultOption;

        string *readyList = MISSION_MAIN->getReadyMissions(user);
        foreach(string missId in readyList) {
                object missObj = MISSION_MAIN->getMissionObj(missId);
                if( !objectp(missObj) || missObj->getCompleteNpc() != sysId ) continue;

                int missType = missObj->getType();
                string missName = missObj->getName();
                string optMsg = sprintf("[%s]%s(%s)", missionType2Text[missType], missName, "可完成");
                mapping option = createOption(optMsg, "onClickReadyMission", ({missId}));
                opts += ({ option });

		if( undefinedp(defaultOption) ) {
			defaultOption = createDefaultOption("继续", "onClickReadyMission", ({missId}));
		}
        }

        string *canAcceptList = MISSION_MAIN->getCanAcceptMissions(user);
        foreach(string missId in canAcceptList) {
                object missObj = MISSION_MAIN->getMissionObj(missId);
                if( !objectp(missObj) || missObj->getAcceptNpc() != sysId ) continue;

                int missType = missObj->getType();
                string missName = missObj->getName();
                string optMsg = sprintf("[%s]%s(%s)", missionType2Text[missType], missName, "可接");
                mapping option = createOption(optMsg, "onClickAcceptMission", ({missId}));
                opts += ({ option });

		if( undefinedp(defaultOption) ) {
			defaultOption = createDefaultOption("继续", "onClickAcceptMission", ({missId}));
		}
        }

        string *progressList = MISSION_MAIN->getProgressMissions(user) - canAcceptList;
        foreach(string missId in progressList) {
                object missObj = MISSION_MAIN->getMissionObj(missId);
                if( !objectp(missObj) || missObj->getAcceptNpc() != sysId ) continue;

                int missType = missObj->getType();
                string missName = missObj->getName();
                string optMsg = sprintf("[%s]%s(%s)", missionType2Text[missType], missName, "已接");
                mapping option = createOption(optMsg, "onClickAcceptedMission", ({missId}));
                opts += ({ option });

		if( undefinedp(defaultOption) ) {
			defaultOption = createDefaultOption("继续", "onClickAcceptedMission", ({missId}));
		}
        }

	if( !undefinedp(defaultOption) ) {
		opts += ({ defaultOption });
	}

        return opts;
}
// ----------------end NPC交互

// ----------------战斗相关
// 战斗结束
void onFightEnd(object *users, mixed fight)
{
        foreach(object user in users) {
                string *missions = keys(user->getMission());
                foreach(string missId in missions) {
                        object missObj = MISSION_MAIN->getMissionObj(missId);
                        if( objectp(missObj) ) {
                                missObj->onFightEnd(user, fight);
                        }
                }
        }
}

// 副本结束
void onFubenEnd(object user, mixed fuben)
{
        string *missions = keys(user->getMission());
        foreach(string missId in missions) {
                object missObj = MISSION_MAIN->getMissionObj(missId);
                if( objectp(missObj) ) {
                        missObj->onFubenEnd(user, fuben);
                }
        }
}
// ----------------end 战斗相关


