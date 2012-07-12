#include <arena.h>
#include <macros.h>
#include <debug.h>
#include "main.h"

#define ARENA_SAVE Import("ARENA_SAVE")

void onWinChallenage(object challenger, int loserId)
{
	int winnerId = challenger->getId();
	int robbedWuhun = ARENA_SAVE->robLoserWuhun(winnerId, loserId);

	//set loser robbed
	if (robbedWuhun == ARENA_SAVE->getOwnWuhunId(loserId))
	{
		ARENA_SAVE->setUserRobbed(loserId, 1);
	}
	//unset winner robbed
	if (robbedWuhun == ARENA_SAVE->getOwnWuhunId(winnerId))
	{
		ARENA_SAVE->setUserRobbed(winnerId, 0);
	}

	//update robbed wuhun info
	else if (robbedWuhun)
		ARENA_SAVE->updateRobbedWuhun(robbedWuhun, winnerId);

	int oldWinnerRank = ARENA_SAVE->getUserRank(winnerId);
	ARENA_SAVE->replaceLoserRank(challenger, loserId);	
	int newWinnerRank = ARENA_SAVE->getUserRank(winnerId);

	//update winner info
	ARENA_SAVE->updateUnstopWin(winnerId, newWinnerRank, newWinnerRank - oldWinnerRank);

	//update loser info
	ARENA_SAVE->stopUnstopWin(loserId);
}

void onLoseChallenge(object challenger, int winnerId)
{
	ARENA_SAVE->robLoserWuhun(winnerId, challenger->getId());
}

class ArenaFighter* getCanChallengeList(object user)
{
	int uid = user->getId();
	int userRank = ARENA_SAVE->getUserRank(uid);
	int* rankList = ARENA_SAVE->getRankList();
	assert(sizeof(rankList) >= userRank);
	int* challengeList;
	if (userRank > 10)
	{
		challengeList = rankList[userRank - 11..userRank-1];
	}
	else
	{
		challengeList = rankList[0..userRank-2]; 
		int size = sizeof(rankList);
		int end = size > 10 ? 10 : size;
		if (size > userRank)
			challengeList += rankList[userRank..end - 1];
	}
	return packCanChallengeList(challengeList);
}

private class ArenaFighter* packCanChallengeList(int* canChallengeList)
{
	class ArenaFighter* fighterList = allocate(sizeof(canChallengeList));
	int idx = 0;
	class ArenaFighter tempFighter;
	foreach(int uid in canChallengeList)
	{
		tempFighter = new(class ArenaFighter);	
		tempFighter->uid = uid;
		tempFighter->rank = ARENA_SAVE->getUserRank(uid);
		tempFighter->name = ARENA_SAVE->getUserName(uid);
		fighterList[idx++] = tempFighter;
	}
	return  fighterList;
}
