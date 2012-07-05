#include <var_prop.h>
#include <macros.h>

// #define SYS_NPC_NUM           (1000)

// NPC ID转换为系统ID
MEMORY_VAR(NpcID2SysID, ([]))
// 系统ID转换为NPC ID
MEMORY_VAR(SysID2NpcID, allocate(SYS_NPC_NUM))


void registSystemNpc(int npcId, int systemId)
{
	if (!intp(systemId) || systemId >= SYS_NPC_NUM) {
		debug_message("error: " + systemId);
		return ;
	}
	NpcID2SysID[npcId] = systemId;
	SysID2NpcID[systemId] = npcId;
}


int npcId2SystemId(int npcId)
{
	return NpcID2SysID[npcId];
}

int systemId2NpcId(int systemId)
{
	if (intp(systemId) && systemId < SYS_NPC_NUM) {
		return SysID2NpcID[systemId];
	}
	return 0;
}

void unregistSystemNpc(int npcId)
{
	int systemId = npcId2SystemId(npcId);

	if (intp(systemId) && systemId < SYS_NPC_NUM) {
		SysID2NpcID[systemId] = 0;
		map_delete(NpcID2SysID, npcId);
	}
}
