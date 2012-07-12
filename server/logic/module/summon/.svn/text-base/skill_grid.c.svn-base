#if 0
// 每个召唤兽有6个技能格子
// M_SUMM_SKILL_GRID 必须存盘
//

#include <summon_key.h>
#include <debug.h>

#define NULL_KEY ""
// 此数值要根据开放境界取出 目前是 JINGJIE_LINGXU
#define NUM_OF_GRIDS 7

#define UnSet(mpGrid,i) { mpGrid[i] = NULL_KEY; }
#define SetKey(mpGrid,i,key) { mpGrid[i] = key; }

private object logger_grid;

void create_logger_grid()
{
	logger_grid = Import("LOG")->New("summon");
}	

int HaveKey(mapping mpGrid,int i)
{
	return !(!mpGrid[i] || mpGrid[i] == NULL_KEY);
}

mapping AllSkillGrids(int summid)
{
	int iSize;
	string * Skills = ({});
	mapping mpRes = ([]);
	mapping mpSkillGrid = GetSummonSave(summid, M_SUMM_SKILL_GRID);
	mapping mpSkills = GetSummonSave(summid, M_SUMM_SKILLS);

	for ( int i=1; i <= NUM_OF_GRIDS; i ++ )
	{
		string _cSkill = mpSkillGrid[i];
		
		if ( HaveKey( mpSkillGrid, i) )
		{	
			Skills += ({ sprintf("%s%d", _cSkill, mpSkills[_cSkill]) });
			// 位置从1开始
			mpRes[i] = (["skill":_cSkill, "level":mpSkills[_cSkill]]);
		}	
		else
			Skills += ({ NULL_KEY, });
	}	

	// 保留临时数据
	SetSummonTemp(summid, A_T_SUMM_SKILL, Skills);

	return mpRes;
}

// 根据位置得到技能
string GetGridSkill(int summid, int iPos)
{
	mapping mpSkillGrid = GetSummonSave(summid, M_SUMM_SKILL_GRID);

	return mpSkillGrid[iPos];
}

int GridDelSkill(int summid, string cSkill)
{
	mapping mpSkillGrid = GetSummonSave(summid, M_SUMM_SKILL_GRID);

	for( int i=1; i <= NUM_OF_GRIDS; i ++ )
	{
		if( ! HaveKey(mpSkillGrid, i) )
			continue;
		if ( mpSkillGrid[i] == cSkill )	
		{
			UnSet(mpSkillGrid, i);
			SetSummonSave( summid, M_SUMM_SKILL_GRID, mpSkillGrid);
			logger_grid->Log("summon %s %d grid delskill %s.", GetSummonSave(summid, K_GLOBAL_KEY), i, cSkill);
			return i;
		}
	}

	return 0;
}

//int GridReplaceSkill(int summid, int iPos, string cSkill)
int GridSetSkill(int summid, int iPos, string cSkill)
{
	mapping mpSkillGrid = GetSummonSave(summid, M_SUMM_SKILL_GRID);
	
	assert( iPos );

	if ( HaveKey(mpSkillGrid, iPos) )
	{
		UnSet(mpSkillGrid, iPos);
	}
	logger_grid->Log("summon %s %d grid setskill %s.", GetSummonSave(summid, K_GLOBAL_KEY), iPos, cSkill);
	SetKey(mpSkillGrid, iPos, cSkill);

	SetSummonSave(summid, M_SUMM_SKILL_GRID, mpSkillGrid);
}

int GridAddSkill( int summid, string cSkill)
{
	int iPos ;
	mapping mpSkillGrid = GetSummonSave(summid, M_SUMM_SKILL_GRID);

	for( int i=1; i <= NUM_OF_GRIDS; i ++ ) // hardcode 技能grid位置
	{
		if ( HaveKey(mpSkillGrid, i) )
			continue;

		// logger_grid->Log("summon %s %d grid addskill %s.", GetSummonSave(summid, K_GLOBAL_KEY), i, cSkill);
		SetKey(mpSkillGrid, i, cSkill);
		SetSummonSave(summid, M_SUMM_SKILL_GRID, mpSkillGrid);

		return i;
	}
}
#endif
