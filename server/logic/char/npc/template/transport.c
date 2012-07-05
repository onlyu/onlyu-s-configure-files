#include <npc_key.h>
#include <menu.h>
inherit "char/npc/npc_base";

void initNpc(int npcid)
{
}

void doClick(object user, int npcId)
{
	int uniqId = Import("UNIQ_NPC")->npcId2SystemId(npcId);
        Import("FUBEN_SYS")->fubenEntryList(user, uniqId);
}
