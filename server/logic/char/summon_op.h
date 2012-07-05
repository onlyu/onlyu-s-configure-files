#include <frames.h>
#include <user_key.h>
#include <summon.h>
#include <fightmatrix.h>
#include "/rc/rpc/summon.h"

static mapping fightMatrix = ([]);
static mapping summonTypes = ([]);
static mapping mpMaxFightAmount = ([1:(["hold":2,"fight":1]),
			           5:(["hold":3,"fight":2]),
			           10:(["hold":4,"fight":3]),
			           20:(["hold":5,"fight":3]),
			           30:(["hold":6,"fight":4]),
			           40:(["hold":7,"fight":4]),
			           50:(["hold":8,"fight":5]),]);

private mapping getSummonFrame();

int* getAllFightSummon()
{
	mapping mpFrame = getSummonFrame();
	mapping mpBag = mpFrame[SUMMON_BAG_FIGHT];
	return Import("BASE_BAG")->GetGridValues(mpBag);
}

int getMaxHoldSummonAmount()
{
	int grade = getGrade();
	int gradeDownBound = 0;
	foreach (int tempBound, mapping info in mpMaxFightAmount)
	{
		if (tempBound <= grade)
		{
			if (!gradeDownBound || tempBound > gradeDownBound)
			{
				gradeDownBound = tempBound;	
			}
		}
	} 
	return mpMaxFightAmount[gradeDownBound]["hold"];
}

int getTotalSummonAmount()
{
	return sizeof(summonTypes);
}
/*
* 这个如果与人物身上的状态无关，放在这里不合理 应该放在util里面
*/
int getSummonBagMaxSize(int bagId);
/*
*这个感觉也可以放在util里面
*/
int getSummonBagId(int summonId);
int hasSummonByType(int type);
void addSummonType(int type, int summonId);
int isFightMatrixPosLocked(int pos);
varargs int addSummonToBag(int summonId, int bagId, int pos);
int removeSummonFromBag(int summonId, int bagId);
int moveSummonToFightMatrix(int summonId, int fightMatrixPos);
int removeSummonFromFightMatrix(int pos);
int moveOnFightMatrix(int fromPos, int  toPos);
void sendMaxFightAmount();


int getSummonIdByType(int type)
{
	if (!hasSummonByType(type))
		return 0;
	return summonTypes[type];
}

int addRestoreSummon(int summonId, int bagId, int pos);

private void initSummonGlobalKeyMap()
{
	this_object()->setTemp(KT_SUMMON_GLOBALKEY_MAP, ([]));
}

private void initFightMatrix();

private void addSaveSummon(int bagId, int pos, int summonId)
{
	if ( undefinedp(summon[bagId]) )
	{
		summon[bagId] = ([]);
	}
	assert( pos >= 1 );
	summon[bagId][pos] = get_summon_map(summonId, PERM_VAR);
}

private void removeSaveSummon(int iBagId, int iPos)
{
	map_delete(summon[iBagId], iPos);
}

private mapping getSummonFrame()
{
	return summon_frame;
}

private void initSummonFrames()
{
	mapping mpSummon = getSummonFrame();

	// mpSummon[KT_SUMMON_OWNER] = usernum ;

	foreach (int bagId in SUMMON_BAGS)
	{
		int maxSize = getSummonBagMaxSize(bagId);
		mpSummon[bagId] = "module/frame/base_bag"->CreateBag(usernum, bagId, maxSize);
	}
}

int getSummonBagId(int summonId)
{
	return GetSummonTemp(summonId, KT_SUMMON_BAG);	
}

void addSummonType(int type, int summonId)
{
	summonTypes[type] = summonId;
}

int hasSummonByType(int type)
{
	////debug_message("has summon type:%d %O", type, summonTypes);
	//debug_message("summonTypes:%O type:%d", summonTypes, type);
	return summonTypes[type];
}


int isFightMatrixPosLocked(int pos)
{
	return 0;
}

varargs int addSummonToBag(int summonId, int bagId, int pos)
{
	int owner;
	mapping mpGlobalKey;
	string globalKey;
	mapping mpFrame = getSummonFrame();
	owner = GetSummonTemp(summonId, KT_SUMMON_OWNER);
	if ( !owner || owner != usernum)
	{
		////debug_message(sprintf("玩家[%d]添加召唤兽[%d, owner:%d]", usernum, summonId, owner));
		return 0;
	}
	if ( !bagId ) bagId = SUMMON_BAG_NORMAL;
	pos = Import("BASE_BAG")->AddToBag(summonId, mpFrame[bagId], pos);
	////debug_message(sprintf("添加召唤兽位置%d", pos));
	if ( pos > 0 )
	{
		SetSummonTemp(summonId, KT_SUMMON_BAG, bagId);	
		SetSummonTemp(summonId, KT_SUMMON_POS, pos);	
		SetSummonTemp(summonId, KT_SUMMON_OWNER, usernum);	

		addSaveSummon( bagId, pos, summonId );
		//ResortSummons(bagId);

        //
        	globalKey = GetSummonSave( summonId, K_GLOBAL_KEY );
        	mpGlobalKey = getTemp( KT_SUMMON_GLOBALKEY_MAP );
        	if ( undefinedp(mpGlobalKey) )
        	{
            		mpGlobalKey = ([]);
            		mpGlobalKey[globalKey] = summonId;
            		setTemp( KT_SUMMON_GLOBALKEY_MAP, mpGlobalKey );
        	}
        	else
        	{
            		mpGlobalKey[globalKey] = summonId;
        	}	
		Import("SUMMON_UTIL")->rpcSummonInfo(usernum, summonId);
		return pos;
	}
	return 0;
}

int addRestoreSummon(int summonId, int bagId, int pos)
{
	mapping mpFrame = getSummonFrame();

	// ////debug_message(sprintf("add restore itemid:%d,bag:%d,pos:%d\n", summonId, bagId, pos));

	if (!bagId) bagId = SUMMON_BAG_NORMAL;
	// 对Owner的检测？ TODO
	assert ( (!GetSummonTemp(summonId, KT_SUMMON_OWNER)) );

	pos = Import("BASE_BAG")->AddToBag(summonId, mpFrame[bagId], pos);
	// 设置pos
	if (pos > 0 )
	{

		SetSummonTemp(summonId, KT_SUMMON_BAG, bagId);	
		SetSummonTemp(summonId, KT_SUMMON_POS, pos);	
		SetSummonTemp(summonId, KT_SUMMON_OWNER, usernum);	

		// Import("SUMMON_UTIL")->rpcSummonInfo(usernum, summonId);
	}
	////debug_message(sprintf("restore summon pos=%d bagid=%d summonid=%d", pos, bagId, summonId));
	return pos;
	
}

// 将某包裹的物品发给客户端
void sendBagSummon(int bagId)
{
	mapping allSummons = Import("BASE_BAG")->GetGrids( getSummonFrame()[bagId] );	

	// rsend to client
	foreach (int pos, int summonId in allSummons)
	{
		Import("SUMMON_UTIL")->rpcSummonInfo(usernum, summonId);
	}
}

private void sendAllSummons()
{
	mapping mpFrame = getSummonFrame();

	foreach( int _bagId, mapping _bagData in mpFrame) {
		sendBagSummon(_bagId);
	}
}

int getSummonBagMaxSize(int bagId)
{
	//暂时写死
	return 100;
}



int removeSummonFromBag(int summonId, int bagId)
{
	int bagId_ = getSummonBagId(summonId);
 	if (bagId_ != bagId)
		return 0;	
 	if (GetSummonTemp(summonId, KT_SUMMON_OWNER) != usernum )
		return 0;
	string globalKey = GetSummonSave( summonId, K_GLOBAL_KEY );
	mapping mpGlobalKey = getTemp( KT_SUMMON_GLOBALKEY_MAP );
	if ( !undefinedp(mpGlobalKey) )
	{
		map_delete( mpGlobalKey, globalKey ); 
	}
	int pos = GetSummonTemp(summonId, KT_SUMMON_POS);
	Import("BASE_BAG")->RemoveByPos(getSummonFrame()[bagId], pos);
	removeSaveSummon(bagId, pos);
	rpc_client_summon_del(usernum, bagId, summonId);
	return 1;
}

class FightMatrixMaxInfo  getFightMatrixMaxInfo()
{
	class FightMatrixMaxInfo maxInfo = new(class FightMatrixMaxInfo);
	int defaultMax = 6;
	int maxFightAmount = 0;
	int grade = this_object()->getGrade();
	int gradeDownBound = 0;
	int gradeUpBound = 0;
	foreach (int tempBound, mapping info in mpMaxFightAmount)
	{
		if (tempBound <= grade)
		{
			if (!gradeDownBound || tempBound > gradeDownBound)
			{
				gradeDownBound = tempBound;	
			}
		}
		if (tempBound > grade)
		{
			if (!gradeUpBound || tempBound < gradeUpBound)
			{
				gradeUpBound = tempBound;	
			}
			//debug_message("gradeUpBound:%d", gradeUpBound);
		}
	} 
	maxInfo->maxHold = mpMaxFightAmount[gradeDownBound]["hold"];
	maxInfo->maxFight = mpMaxFightAmount[gradeDownBound]["fight"];
	if (gradeUpBound)
	{
		maxInfo->nextMaxHold = mpMaxFightAmount[gradeUpBound]["hold"];
		maxInfo->nextMaxFight = mpMaxFightAmount[gradeUpBound]["fight"];
		maxInfo->nextGrade = gradeUpBound;
	}
	//debug_message("fight maxtrix max info:%O", maxInfo);
	return maxInfo;
}

int getFightSummonAmount()
{
	return sizeof("module/frame/base_bag"->GetGrids(getSummonFrame()[SUMMON_BAG_FIGHT]));
}

void sendFightMatrixInfo()
{
	rpc_client_fightmatrix_maxinfo(usernum, getFightMatrixMaxInfo());
	foreach (int pos, class FightGridInfo fightRole in fightMatrix)
	{
		rpc_client_fightmatrix_add(usernum, fightRole);
	}

}
private void initFightMatrix()
{
	mapping mpSummon = getSummonFrame();	
	mapping mpFightMatrixSummon = "module/frame/base_bag"->GetGrids(mpSummon[SUMMON_BAG_FIGHT]);

	//init hero pos
	int heroPos = this_object()->getHeroFightMatrixPos();
	class FightGridInfo fightMatrixPosInfo = new (class FightGridInfo);
	fightMatrixPosInfo->type = FIGHT_GRID_HERO;
	fightMatrixPosInfo->id = usernum;
	fightMatrixPosInfo->pos = heroPos;
	fightMatrix[heroPos] = fightMatrixPosInfo;

	//init fight summon pos
	foreach (int pos, int summonId in mpFightMatrixSummon)
	{
		class FightGridInfo summonInfo = new(class FightGridInfo);
		summonInfo->type = FIGHT_GRID_SUMMON;
		summonInfo->id = summonId;
		summonInfo->pos = pos;
		fightMatrix[pos] = summonInfo;
	}

	//init empty and locked pos
	int idxPos = 1;
	while (1)
	{
		if (idxPos > MAX_FIGHT_MATRIX_POS)
			break;
		if (undefinedp(fightMatrix[idxPos]))
		{
			if (isFightMatrixPosLocked(idxPos))
			{
				class FightGridInfo summonInfo = new(class FightGridInfo);
				summonInfo->type = FIGHT_GRID_LOCKED;
				summonInfo->id = 0;
				summonInfo->pos = idxPos;
				fightMatrix[idxPos] = summonInfo;
			}
			else
			{
				class FightGridInfo summonInfo = new(class FightGridInfo);
				summonInfo->type = FIGHT_GRID_EMPTY;
				summonInfo->id = 0;
				summonInfo->pos = idxPos;
				fightMatrix[idxPos] = summonInfo;
			}
		}
		idxPos++;
	}
	
	////debug_message(sprintf("fight matrix %O", fightMatrix));
	//send fight matrix info
}

private void addSummonToFightMatrix(int summonId, int pos)
{
	class FightGridInfo summonInfo = new(class FightGridInfo);
	summonInfo->type = FIGHT_GRID_SUMMON;
	summonInfo->id = summonId;
	summonInfo->pos = pos;
	fightMatrix[pos] = summonInfo;
	rpc_client_fightmatrix_add(usernum, summonInfo);
}

int moveSummonToFightMatrix(int summonId, int fightMatrixPos)
{
	if (summonId <= 0 || fightMatrixPos > MAX_FIGHT_MATRIX_POS || fightMatrixPos <= 0)
		return 0;
	int bagId = getSummonBagId(summonId);
	if (bagId != SUMMON_BAG_NORMAL)
	{
		////debug_message(sprintf("add fight summon bagId=%d summonId=%d", bagId, summonId));
		return 0;
	}
	
	class FightGridInfo oldMatrixRoleInfo  = fightMatrix[fightMatrixPos];
	if (oldMatrixRoleInfo->type == FIGHT_GRID_LOCKED)
		return 0;
	if (oldMatrixRoleInfo->type == FIGHT_GRID_HERO)
		return 0;

	if (oldMatrixRoleInfo->type == FIGHT_GRID_EMPTY)
	{
		if (getFightSummonAmount() + 1 >= getFightMatrixMaxInfo()->maxFight)
			return 0;
		removeSummonFromBag(summonId, SUMMON_BAG_NORMAL);
		addSummonToBag(summonId, SUMMON_BAG_FIGHT, fightMatrixPos);
		addSummonToFightMatrix(summonId, fightMatrixPos);
		return 1;
	}
	if (oldMatrixRoleInfo->type == FIGHT_GRID_SUMMON)
	{
		removeSummonFromBag(summonId, SUMMON_BAG_NORMAL);
		removeSummonFromBag(oldMatrixRoleInfo->id, SUMMON_BAG_FIGHT);
		addSummonToBag(summonId, SUMMON_BAG_FIGHT, fightMatrixPos);
		addSummonToBag(oldMatrixRoleInfo->id, SUMMON_BAG_NORMAL );
		addSummonToFightMatrix(summonId, fightMatrixPos);
		return 0;
	}

}

int removeSummonFromFightMatrix(int pos)
{
	if (pos <= 0 || pos > MAX_FIGHT_MATRIX_POS)
		return 0;
	class FightGridInfo summonInfo  = fightMatrix[pos];
	if (summonInfo->type != FIGHT_GRID_SUMMON )
		return 0;
	int summonId = summonInfo->id;
	map_delete(fightMatrix, pos);
	removeSummonFromBag(summonId, SUMMON_BAG_FIGHT);
	addSummonToBag(summonId, SUMMON_BAG_NORMAL);
	summonInfo->type = FIGHT_GRID_EMPTY;
	summonInfo->id = 0;
	summonInfo->pos = pos;
	fightMatrix[pos] = summonInfo;
	rpc_client_fightmatrix_add(usernum, summonInfo);
}

private int moveHeroOnFightMatrix(class FightGridInfo heroInfo, class FightGridInfo toEmptyInfo)
{
	int newEmptyPos = heroInfo->pos;
	int newHeroPos 	= toEmptyInfo->pos;
	this_object()->setHeroFightPos(newHeroPos);
	heroInfo->pos = newHeroPos;
	toEmptyInfo->pos = newEmptyPos;
	fightMatrix[newHeroPos] = heroInfo;
	fightMatrix[newEmptyPos] = toEmptyInfo;
	rpc_client_fightmatrix_add(usernum, heroInfo);
	rpc_client_fightmatrix_add(usernum, toEmptyInfo);
}

private int swapHeroSummonOnFightMatrix(class FightGridInfo heroInfo, class FightGridInfo summonInfo)
{
	if (heroInfo->pos == summonInfo->pos)
		return 0;
	int newSummonPos = heroInfo->pos;
	int oldSummonPos = summonInfo->pos;
	int newHeroPos = summonInfo->pos;
	int summonId = summonInfo->id;
	this_object()->setHeroFightPos(newHeroPos);

	heroInfo->pos = newHeroPos;
	summonInfo->pos = newSummonPos;
	fightMatrix[newHeroPos] = heroInfo;
	fightMatrix[newSummonPos] = summonInfo;
	
	removeSummonFromBag(summonId, SUMMON_BAG_FIGHT);
	addSummonToBag(summonId, SUMMON_BAG_FIGHT, newSummonPos);
	rpc_client_fightmatrix_add(usernum, heroInfo);
	rpc_client_fightmatrix_add(usernum, summonInfo);
	return 1;
}

private int swapSummonOnFightMatrix(class FightGridInfo summonInfoA, class FightGridInfo summonInfoB)
{
	int newSummonAPos = summonInfoB->pos;
	int newSummonBPos = summonInfoA->pos;
	int oldSummonAPos = newSummonBPos;
	int oldSummonBPos = newSummonAPos;
	int summonAId = summonInfoA->id;
	int summonBId = summonInfoB->id;

	summonInfoA->pos = newSummonAPos;
	summonInfoB->pos = newSummonBPos;
	fightMatrix[newSummonAPos] = summonInfoA;
	fightMatrix[newSummonBPos] = summonInfoB;

	removeSummonFromBag(summonAId, SUMMON_BAG_FIGHT);
	removeSummonFromBag(summonBId, SUMMON_BAG_FIGHT);
	addSummonToBag(summonAId, SUMMON_BAG_FIGHT, newSummonAPos);
	addSummonToBag(summonBId, SUMMON_BAG_FIGHT, newSummonBPos);
	rpc_client_fightmatrix_add(usernum, summonInfoA);
	rpc_client_fightmatrix_add(usernum, summonInfoB);
}

private int moveSummonOnFightMatrix(class FightGridInfo summonInfo, class FightGridInfo toEmptyInfo)
{
	int newEmptyPos = summonInfo->pos;
	int newSummonPos = toEmptyInfo->pos;
	int oldSummonPos = newEmptyPos;
	int summonId = summonInfo->id;
	
	summonInfo->pos = newSummonPos;
	toEmptyInfo->pos = newEmptyPos;
	fightMatrix[newSummonPos] = summonInfo;
	fightMatrix[newEmptyPos] = toEmptyInfo;

	removeSummonFromBag(summonId, SUMMON_BAG_FIGHT);
	addSummonToBag(summonId, SUMMON_BAG_FIGHT, newSummonPos);
	rpc_client_fightmatrix_add(usernum, summonInfo);
	rpc_client_fightmatrix_add(usernum, toEmptyInfo);
}

int moveOnFightMatrix(int fromPos, int  toPos)
{
	if (toPos == fromPos || fromPos <= 0 || fromPos > MAX_FIGHT_MATRIX_POS || toPos <= 0 || toPos > MAX_FIGHT_MATRIX_POS)
		return 0;

	class FightGridInfo posInfoFrom  = fightMatrix[fromPos];
	class FightGridInfo posInfoTo  = fightMatrix[toPos];
	if (posInfoFrom->type == FIGHT_GRID_HERO)
	{
		if (posInfoTo->type == FIGHT_GRID_EMPTY)
		{
			moveHeroOnFightMatrix(posInfoFrom, posInfoTo);
		}
		else if (posInfoTo->type == FIGHT_GRID_SUMMON)
		{
			swapHeroSummonOnFightMatrix(posInfoFrom, posInfoTo);
		}
		else
		{
			return 0;
		}
	}
	else if (posInfoFrom->type == FIGHT_GRID_SUMMON)
	{
		if (posInfoTo->type == FIGHT_GRID_SUMMON)
			swapSummonOnFightMatrix(posInfoFrom, posInfoTo);
		else if (posInfoTo->type == FIGHT_GRID_HERO)
		{
			swapHeroSummonOnFightMatrix(posInfoTo, posInfoFrom);
		}
		else if (posInfoTo->type == FIGHT_GRID_EMPTY)
		{
			moveSummonOnFightMatrix(posInfoFrom, posInfoTo);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
void sendMaxFightAmount()
{
	rpc_client_fightmatrix_maxinfo(usernum, getFightMatrixMaxInfo());
}


class FightGridInfo getFightGridInfo(int pos)
{
	if (pos <= 0 || pos > MAX_FIGHT_MATRIX_POS)
		return 0;
	return fightMatrix[pos];
}

mapping getFightMatrix()
{
	return fightMatrix;
}

// 获取包裹的所有灵兽
mapping getAllSummonsInBag(int bagId)
{
	if (!bagId) bagId = SUMMON_BAG_NORMAL;
	return Import("BASE_BAG")->GetGridValues( getSummonFrame()[bagId] );	
}

