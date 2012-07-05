#if 0
#include <summon.h>
#include <debug.h>

static mapping mpAllRates =  ([ ]);

// iGrow,iRealm,iRace, -> (["Shoulong" : "xx", "Rates" : ([ Type1 : Rate1, Type2 :Rate2, ]),  ]);
mapping mpRes = ([]);
mapping mpTypes = ([]);
// iGrow,cType,SummonId
mapping mpSummons = ([]);

void ReloadData()
{
	mpAllRates = ([]);

	foreach( string cType, mapping _mpInfo in "data/shoulong_data"->get_data() )
	{
		int _grow = _mpInfo["grow"];
		if ( undefinedp(mpAllRates[_grow]) )
			mpAllRates[_grow] = ([]);

		if (!undefinedp(mpAllRates[_grow][ _mpInfo["summon_id"] ])) {
			//debug_message(sprintf("ERROR summon grow %d, id : %d duplicate", _grow, _mpInfo["summon_id"]));
			continue;
		}

		mpAllRates[_grow][ _mpInfo["summon_id"] ] = copy(_mpInfo);
		mpAllRates[_grow][ _mpInfo["summon_id"] ]["Type"] = cType;
	}

	mapping mpRacePairs = ([
		"仙灵兽笼" : SUMMON_RACE_XIANLING,
		"精怪兽笼" : SUMMON_RACE_JINGGUAI,
		"魂系兽笼" : SUMMON_RACE_HUN,
		"魔系兽笼" : SUMMON_RACE_MO,
		"异物兽笼" : SUMMON_RACE_YIWU,
			]);

	foreach ( int iGrow, mapping mpRate in mpAllRates )
	{
		mpRes[iGrow] = ([]);
        mpSummons[iGrow] = ([]);

		foreach( int iType, mapping mpInfo in mpRate )
		{
			int iRealm = mpInfo["realm"];
			if ( undefinedp(mpRes[iGrow][iRealm]) )
				mpRes[iGrow][iRealm] = ([]);

			string cShoulong = mpInfo["Type"];
			int iRace = mpRacePairs[mpInfo["item_name"]];
			assert( iRace != 0 );

			if (undefinedp(mpRes[iGrow][iRealm][iRace]))
				mpRes[iGrow][iRealm][iRace] = ([]);	

			mpRes[iGrow][iRealm][iRace]["Shoulong"] = cShoulong;
			// 一一对应的兽笼 和稀有、平常区分开来
			mpRes[iGrow][iRealm][iRace][iType] = cShoulong;

			if (undefinedp(mpRes[iGrow][iRealm][iRace]["Rates"]))
				mpRes[iGrow][iRealm][iRace]["Rates"] = ([]);

			mpRes[iGrow][iRealm][iRace]["Rates"][ iType ] = mpInfo["rate"];

			////debug_message( sprintf("iGrow %d, iRealm %d, iRace %d,init type %O,rate: %O", iGrow, iRealm, iRace, iType, mpRes[iGrow][iRealm][iRace]["Rates"][iType] ) );
            mpTypes[mpInfo["Type"]] = mpInfo;
            mpSummons[iGrow][mpInfo["Type"]] = iType;
		}
	}
}	

void create()
{
	ReloadData();
}

int GetRealmBycType( string cType )
{
    return mpTypes[cType]["realm"];
}

// 获取兽笼里灵兽ID
int GetSummonId( int iGrow, string cType )
{
    if ( undefinedp(mpSummons[iGrow]) )
        return 0;
    return mpSummons[iGrow][cType];
}

string GetSummonName( int iGrow, string cType )
{
	int iSummonId = GetSummonId( iGrow, cType );
	if ( iSummonId == 0 )
		return "";
    return "module/summon/util"->GetNameByType( iSummonId );
}

// 取得兽笼的造型
varargs string GetShoulong(int iRealm, int iGrow, int iRace, int iType)
{
	// 总共5个种族
	if ( !iRace )
	{
		iRace = rand_array(keys(mpRes[iGrow][iRealm]));
	}

	if ( iType )
	{
		return mpRes[iGrow][iRealm][iRace][iType];
	}

	////debug_message( sprintf("%d,%d,%d. %O", iRealm, iGrow, iRace, mpRes) );
	return mpRes[iGrow][iRealm][iRace]["Shoulong"];
}

varargs int RandomSummon(int iRealm, int iGrow, int iRace)
{
	int i = random(1000);
	int _tmp = 0;
	int iType = 0;

	// 总共5个种族
	if ( !iRace )
	{
		iRace = rand_array(keys(mpRes[iGrow][iRealm]));
	}

	foreach ( int _type, int _rate in mpRes[iGrow][iRealm][iRace]["Rates"])
	{	
		_tmp += _rate;

		if ( i > _tmp ) continue;
		iType = _type; break;	
	}

	return iType;
}
#endif
