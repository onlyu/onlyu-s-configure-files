#include <save.h>
#include <var_prop.h>
#include <debug.h>

DB_VAR(_data, ([]))
DB_VAR(_rankData, ([]))

// 最高排名
#define  K_MAX_RANK          (0)
// 战斗胜利次数
#define  K_WIN_CNT           (1)
// 战斗失败次数
#define  K_LOSE_CNT          (2)
// 获得战魂
#define  K_PUPPETS           (3)
// 傀儡主人
#define  K_PUPPET_OWNER      (4)
// 上次战斗结果
#define  K_LAST_FT_RESULT    (5)
// 当前连胜次数
#define  K_CONTINUE_WIN_CNT  (6)
// 当前排名
#define  K_CUR_RANK          (7)
#define  KEY_MAX             (8)


#define  FT_RESULT_WIN       (1)
#define  FT_RESULT_LOSE      (2)


#define  USER_HASH_BASE      (10000)

static string saveFile = "fight_match";

RESET_ONUPDATE_VAR(userMaxCnt, 0)

void init()
{
	userMaxCnt = 0;
	foreach (int _hash, mapping _hdata in _rankData) {
		userMaxCnt += sizeof(_hdata);
	}
}

void create()
{
	setSaveName(saveFile);
	doRestore();
	
}

private mixed getUserData(int uid)
{
	int hash = uid % USER_HASH_BASE;
	mapping hashData = getOrSetMapping(_data, hash);

	if (undefinedp(hashData[uid])) {
		hashData[uid] = allocate(KEY_MAX);
	} else {
		if (sizeof(hashData[uid]) < KEY_MAX) {
			mixed *newList = allocate(KEY_MAX);
			int cnt = sizeof(hashData[uid]);
			int i = 0;
			while (i < cnt) {
				newList[i] = hashData[uid][i];
				i++;
			}
			hashData[uid] = newList;
		}
	}
	return hashData[uid];
}

private mapping getRankHashData(int rank)
{
	// 每500名放在一个区间中
	int hash = rank / 500;
	return getOrSetMapping(_rankData, hash);
}


int getRankUser(int rank)
{
	mapping hashData = getRankHashData(rank);
	return hashData[rank];
}

int getUserRank(int uid)
{
	mapping data = getUserData(uid);
	return data[K_CUR_RANK];
}

// 插入到排名队列中
private int _doInsert(int uid, int rank)
{
	if (rank <= 0) return 0;

	mapping data = getUserData(uid);

	data[K_CUR_RANK] = rank;

	if (rank < data[K_MAX_RANK]) {
		data[K_MAX_RANK] = rank;
	}

	mapping rankData = getRankHashData(rank);
	rankData[rank] = uid;

	return 1;
}

// 删除uid中的战魂
private int _doDelPuppet(int uid, int puppet)
{
	mapping ownerData = getUserData(uid);

	if (!mapp(ownerData[K_PUPPETS]) || undefinedp(ownerData[K_PUPPETS][puppet])) {
		return 1;
	}

	mapping puppetData = getUserData(puppet);

	xassert((puppetData[K_PUPPET_OWNER] == uid), puppet);

	int time = ownerData[K_PUPPETS][puppet];

	map_delete(ownerData[K_PUPPETS], puppet);
	puppetData[K_PUPPET_OWNER] = 0;
	
	"module/fight_match/main"->addPuppetExp(uid, puppet, time() - time);
	return 1;
}

// 添加uid中的战魂
private int _doAddPuppet(int uid, int puppet)
{
	mapping ownerData = getUserData(uid);
	mapping puppetData = getUserData(puppet);

	if (!mapp(ownerData[K_PUPPETS])) {
		ownerData[K_PUPPETS] = ([]);
	}
	// 如果已经在傀儡列表中
	if (!undefinedp(ownerData[K_PUPPETS][puppet]) && puppetData[K_PUPPET_OWNER] == uid) {
		return 1;
	}
	// 如果傀儡原来已经有主人的话，删除它们的关系
	if (puppetData[K_PUPPET_OWNER] > 0) {
		_doDelPuppet(puppetData[K_PUPPET_OWNER], puppet);
	}

	// TODO: 是否傀儡列表满?

	ownerData[K_PUPPETS][uid] = time();
	puppetData[K_PUPPET_OWNER] = uid;
	return 1;
}

private void _doIncWinCnt(int uid)
{
	mapping data = getUserData(uid);

	if (data[K_LAST_FT_RESULT] != FT_RESULT_LOSE) {
		data[K_CONTINUE_WIN_CNT] += 1;
	}
	data[K_LAST_FT_RESULT] = FT_RESULT_WIN;
	data[K_WIN_CNT] += 1;
}

private void _doIncLoseCnt(int uid)
{
	mapping data = getUserData(uid);

	data[K_LAST_FT_RESULT] = FT_RESULT_LOSE;
	data[K_CONTINUE_WIN_CNT] = 0;
	data[K_LOSE_CNT] += 1;
}

// 插入是uid的名次低于other的名次
private void _doSwapRank(int uid, int other)
{
	mapping data = getUserData(uid);
	mapping otherData = getUserData(uid);

	if (data[K_CUR_RANK] <= 0 
		|| otherData[K_CUR_RANK] <= 0
		|| data[K_CUR_RANK] > otherData[K_CUR_RANK]) {
		return;
	}

	_doInsert(uid, otherData[K_CUR_RANK]);
	_doInsert(other, data[K_CUR_RANK]);
}

// 将一个玩家插入到最后一名中
int insertLast(int uid)
{
	if (getUserRank(uid) <= 0) {
		return 0;
	}
	return _doInsert(uid, ++userMaxCnt);
}

// winner 战胜 loser
int fightResult(int winner, int loser)
{
	if (winner == loser) return 0;

	_doIncWinCnt(winner);
	_doIncLoseCnt(loser);

	// 对换名次
	_doSwapRank(winner, loser);
	
	// 获得对方战魂
	_doAddPuppet(winner, loser);
}
