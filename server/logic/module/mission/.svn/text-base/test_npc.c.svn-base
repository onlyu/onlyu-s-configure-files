#include <npc_key.h>
#include <menu.h>
inherit "char/npc/template/option";

void doAssign(object user, int npcId)
{
        string missId = optionsGetInput(user, "missId");
        object missObj = Import("MISSION_MAIN")->getMissionObj(missId);
        if( !objectp(missObj) ) return;

        Import("MISSION_MAIN")->assignMission(user, missId);
        Import("MISSION_MAIN")->renewMission(user, missId);
}

void assignMission(object user, int npcId)
{
        mapping options = initNpcOptions(npcId);

        mapping option = createTextInputOption("任务id", "missId");
        addOption(options, option);

        option = createButtonOption("确定", "doAssign", ({}));
        addOption(options, option);
        
        tellOptions(user, options, "请输入需要分派的任务id");
}

void doAssignAccept(object user, int npcId)
{
        string missId = optionsGetInput(user, "missId");
        object missObj = Import("MISSION_MAIN")->getMissionObj(missId);
        if( !objectp(missObj) ) return;

        Import("MISSION_MAIN")->assignMission(user, missId);

        if( !Import("MISSION_MAIN")->acceptMission(user, missId) ) {
		user->tellMe("接受任务失败");
	}

        Import("MISSION_MAIN")->renewMission(user, missId);
}

void assignAcceptMission(object user, int npcId)
{
        mapping options = initNpcOptions(npcId);

        mapping option = createTextInputOption("任务id", "missId");
        addOption(options, option);

        option = createButtonOption("确定", "doAssignAccept", ({}));
        addOption(options, option);
        
        tellOptions(user, options, "请输入任务id");

}

void delAllMission(object user, int npcId)
{
        Import("MISSION_MAIN")->delAllMission(user);
        //Import("MISSION_MAIN")->loginCheck(user);
}

mapping *getExtOpts(object user, int npcId, mixed arg)
{
        mapping *opts = ({});
        mapping option;

        option = createOption("分派一个任务", "assignMission", ({}));
        opts += ({ option });

        option = createOption("分派并接受一个任务", "assignAcceptMission", ({}));
        opts += ({ option });

        option = createOption("删除所有任务", "delAllMission", ({}));
        opts += ({ option });

        return opts;
}


void InitNpc(int npcid)
{
        SetNpc(npcid, K_NPC_NAME, "任务测试NPC");
        SetNpc(npcid, K_NPC_ICON, 10001);
}
