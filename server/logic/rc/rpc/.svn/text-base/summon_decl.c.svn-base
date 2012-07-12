#include <fightmatrix.h>
class SkillInfo
{
	string name;
	string desc;
}
//伙伴信息
class Summon
{
	int summonId;
	int typeid;
	int icon;
	int bagId;
	int pos;

	string name;
	
	int grade;
	int hp;
	int maxHP;
	int attack;
	int defence;
	int speed;
	int qiangfa;
	int kangfa;
	int liliang;
	int zhili;
	int minjie;
	int naili;
	int zhanli;

	int xibie;
	int equipBagId;

	class SkillInfo bdSkill;
	class SkillInfo zdSkill;
}

class NBFightAttr
{
	int dodge; //闪避
	int hit;
	int baoji;
	int kangBao;
	int zhaojia;
	int poJi;
	int ycHit; //异常命中
	int ycDodge; //异常闪避
	int fanji;  //反击
	int lianji; // 连击
	int bisha; //必杀
	int hurtMore; //伤害加深
	int hurtLess; //伤害减免
}

class FightMatrixMaxInfo
{
	int maxHold;
	int maxFight;
	int nextMaxHold;
	int nextMaxFight;
	int nextGrade;
}

void rpc_client_summon_info(int usernum, class Summon summonInfo);

void rpc_client_summon_del(int usernum, int bagId, int summonId);

void rpc_server_summon_nbfight_attr(object user, int summonId);

void rpc_client_summon_nbfight_attr(int usernum, class NBFightAttr nbFightAttr);

void rpc_client_fightmatrix_add(int usernum, class FightGridInfo fightRole); 

void rpc_client_fightmatrix_maxinfo(int usernum, class FightMatrixMaxInfo maxinfo); 

void rpc_server_fightmatrix_add(object user, int summonId, int pos);

void rpc_server_fightmatrix_del(object user, int pos); 

void rpc_server_fightmatrix_move(object user, int fromPos, int toPos);



