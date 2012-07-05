
#include <save.h>

static int * Chars = ({
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
});

// type hostnum serial_no
// 例如: I0100000001
// >>> pow( 26*2+10, 9)/365.0/100000
// 370879083.45927536
//
// >>> pow( 26*2+10, 8)/365.0/100000
// 5981920.700956055
//
// 最大hostnum限制3844？
// >>> pow(62,2) 
// 3844


DB_VAR(mpGlobal, (["cGSum":"000000000", "cGItem":"000000000", "cGOther":"000000000", ]))

static mapping mpTypes = ([
		"cGItem" : "I",
		"cGOther" : "O",
		"cGSum" : "S",
		]);

static int giGlobalLen = 9;
static int giHostNum   = 0;

static string save_name = "globalkey";

void create()
{
	giHostNum   = getHostNum();
	setSaveName(save_name);
	Import("DB")->load_dat_sync(this_object());
}


varargs string ndec_to_n(int x, int n)
{
	int iCount, iTotalSize ;
	string cRes, cTmp = "A";
	
	if ( undefinedp(n) ) n = giGlobalLen;
	
	cTmp[0] = '0';
	cRes = repeat_string( cTmp, n );
	iCount = 1;
	iTotalSize = sizeof(Chars);
	while ( x != 0 )
	{
		cRes[n-iCount] = Chars[x%iTotalSize]; 
		x = x/iTotalSize;
		iCount ++;
	}
	return cRes ;
}

int n_to_dec(string x)
{
	int i, iRes, iDelta;
	
	iRes = 0;
	iDelta = 1;
	i = sizeof(x);
	
	while ( i -- )
	{
		iRes += iDelta*member_array( x[i], Chars );
		iDelta *= sizeof(Chars);
	}
	return iRes ;
}

string add_one(string cNum)
{
	int i, n;
	string cRes;

	i = sizeof(cNum);
	cRes = cNum ;

	while ( i-- )
	{
		n = member_array( cRes[i], Chars );
		if ( n >= sizeof(Chars)-1 )
		{
			cRes[i] = Chars[0]; // 进位
		}
		else
		{
			cRes[i] = Chars[n+1];
			return cRes ;
		}
	}
	// 这里hardcode进位？！直接 0 -> 1
	return "1" + cRes ; // Chars[1] 进位
}

string add_num( string cNum, int x)
{
	while ( x-- > 0 )	cNum = add_one(cNum);
	return cNum ;
}

string n_add_one(string cNum, int n)
{
	int i = n - sizeof(cNum);

	if ( i < 0 )
	{
		debug_message( sprintf("global key error %O, sizeof cnum =%d, n = %d.", cNum, sizeof(cNum), n) );
		return 0;
	}

	while ( i -- )
		cNum = "0" + cNum ; //"0" is Chars[0]

	return add_one(cNum);
}

string NewGlobalkey(string cType)
{
	// cGItem, cGSum, cGOther
	string cKey;

	mpGlobal[cType] = n_add_one(mpGlobal[cType], giGlobalLen) ;

	cKey = sprintf("%d%s%s", giHostNum, mpTypes[cType], mpGlobal[cType]);

	return cKey;
}	


string new_item_globalkey()
{
	return NewGlobalkey("cGItem");
}

string new_summon_globalkey()
{
	return NewGlobalkey("cGSum");
}

// 标示系统GBLKEY(用于副本数据,或者共享数据块也可以考虑使用)
string new_system_globalkey()
{
	return NewGlobalkey("cGOther");
}

#ifdef __TEST__
int __test__()
{

}

#endif
