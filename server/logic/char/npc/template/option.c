#include <npc_key.h>
#include <menu.h>
inherit "char/npc/npc_base";

void initNpc(int npcid)
{
}

/*
回调函数配置
返回一个配置数组，与策划填的闲话表的选项一一对应
static mapping *example = ({
        ([ "func": "cb1", "args": ({}), ]),
        ([ "func": "cb2", "args": ({}), ]),
});
*/
static mapping *getCBFunc()
{
        debug_message("请继承此方法");
        return ({});
}

static string getChat(object user, int npcId)
{
        int uniqId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
        if( !uniqId ) return "";

        return "module/scene/data/npc_chat"->getNpcChat(uniqId);
}

void onClickOpt(object user, int npcId, int optId)
{
        mapping *cbFunc = getCBFunc();
        if( !arrayp(cbFunc) || (sizeof(cbFunc) - 1 < optId) ) return;

        mapping mpCB = cbFunc[optId];
        if( !mpCB ) return;

        call_other(this_object(), ({ mpCB["func"] }) + mpCB["args"]);
}

// 可手写一些选项
static mapping *getExtOpts(object user, int npcId)
{
        return ({});
}

void doClick(object user, int npcId)
{
        mapping *opts = ({});

        // 额外选项
        opts += getExtOpts(user, npcId);

	// 任务模块的响应
        opts += Import("MISSION_HOOK")->onClickNpc(user, npcId);

        // NPC闲话表响应
        int uniqId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
        if(uniqId) {
                string *optsStr = "module/scene/data/npc_chat"->getNpcOpts(uniqId);
                mapping *tmps = allocate(sizeof(optsStr));

                for(int i = 0; i < sizeof(optsStr); i++) {
                        string str = optsStr[i];
                        mapping option = createOption(str, "onClickOpt", ({i}));
                        tmps[i] = option;
                }

                opts += tmps;
        }

        // build menu
        mapping options = initNpcOptions(npcId);
        foreach(mapping opt in opts) {
                addOption(options, opt);
        }

        string chat = getChat(user, npcId);
        tellOptions(user, options, chat);
}
