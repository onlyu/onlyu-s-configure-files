

static mapping mpGroups = ([
		"admin" : 99,
		"gm" : 2,
		"gm_leader" : 3,
		"maintain" : 4,
		]);

static mapping UrsNamePairs = ([
		]);

// 本地内服
static mapping mpLocal = ([
		]);

// 外部测试服务器
static mapping mpTest = ([
		]);


// 外部生产服务器
static mapping mpGameServerDefault = ([
		//"tomup" : "gm",
		]);


static mapping mpHostWizards = ([
		"default" : mpGameServerDefault,
		201: mpLocal,  /*开发内测服务器, 统一*/
		221: mpLocal,  /*开发内测服务器, 统一*/
		222: mpLocal,  /*开发内测服务器, 统一*/
		241: mpLocal,  /*开发内测服务器, 统一*/
		242: mpLocal,  /*开发内测服务器, 统一*/
		243: mpLocal,  /*开发内测服务器, 统一*/
		261: mpLocal,  /*开发内测服务器, 统一*/
		262: mpLocal,  /*开发内测服务器, 统一*/
		265: mpLocal,  /*开发内测服务器, 统一*/
		900: mpLocal,  /*开发内测服务器, 统一*/
		]);

int isTestHost()
{
	// 将hostnum修改为900, 900~1000以上的服务器为测试服务器，模仿外服
	// 990 为 内服验证
	// by cyh
	int hostnum = getHostNum();

	return ((hostnum < 900) || (hostnum == 990));
}

int wizardLevel(object oUser)
{
	//TODO 临时先返回99
	return 99;
	int hostnum = getHostNum();
	mapping _mpTmp ;
	string _cGroup;
	string urs = oUser->getUMS();

	if ( !UrsNamePairs[urs] ) return 0;

	if ( mpHostWizards[hostnum] )
		_mpTmp = mpHostWizards[hostnum];
	else
		_mpTmp = isTestHost() ? mpLocal : mpHostWizards["default"];

	_cGroup = _mpTmp[urs];

	if ( !_cGroup ) 
		return 0;

	return mpGroups[_cGroup];
}

static mapping mpReturnTbl = ([
		]);

int isReturnPoint(object oUser)
{
	mixed res = mpReturnTbl[oUser->getUMS()];
	//debug_message( sprintf("ums is %s, res is %O", oUser->getUMS(), res) );
	return !(undefinedp(res));
}
