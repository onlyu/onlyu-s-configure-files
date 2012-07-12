/*
不做外部模块register进来的hook，现在做的是：谁要hook，就在相应hook函数里加个消息分发
*/

// 点击NPC
void clickNpc(object user, int npcId)
{

}

// 战斗结束
void fightEnd(object *users, mixed fight)
{
        Import("MISSION_HOOK")->onFightEnd(users, fight);
}

// 副本结束
void fubenEnd(object user, mixed fuben)
{
        Import("MISSION_HOOK")->onFubenEnd(user, fuben);
}

// 任务完成
void missionComplete(object user, string missId)
{

}
