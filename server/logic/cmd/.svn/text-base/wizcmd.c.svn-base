// HelpMsg\Level\alias 记录在文件里

// 这样写的目的是便于集中找到某权限所有能使用的指令，拼的指令table也简单直观
mapping mpCmdsByLevel = ([ ]);

// 多对一的别名系统
mapping mpAlias = ([ ]);	
// 便于书写逻辑的指令表
mapping mpCmdTables = ([ ]);

private int TRUE = 1;
private int FALSE = 0;

void BuildCmdTabls()
{
	string * FileList ;

	FileList = get_dir(__DIR__"/wizcmds/*.c");
	if ( !FileList ) FileList = get_dir("/bin/"__DIR__"/wizcmds/*.b");

	foreach ( string cFile in FileList )	
	{
		mapping mpConf = ([ ]);
		int iLevel;
		string HelpMsg;
		string * Alias;

		// 文件名中是否有两个"."？
		//cFile = explode( cFile, ".")[0];
		cFile = cFile[0..strsrch(cFile, '.', -1)-1];

		// debug_message( "found wizcmd:" + cFile );
		if( catch(mpConf = find_object(__DIR__+"wizcmds/"+cFile, 1)->getConf()) != 0 ) continue;
		iLevel = mpConf["Level"];
		HelpMsg = mpConf["HelpMsg"];
		Alias = mpConf["Alias"];

		// debug_message( sprintf("cFiles =========== %s, %O",__DIR__+"wizcmds/"+cFile, HelpMsg)  );

		if ( undefinedp(mpCmdsByLevel[iLevel]) )
			mpCmdsByLevel[iLevel] = ([ ]);

		mpCmdsByLevel[iLevel][cFile] = HelpMsg;	
		mpCmdTables[cFile] = ([ "Level":iLevel, "Usage":HelpMsg ]);

		if (Alias && sizeof(Alias))
		{
			foreach( string cAlias in Alias)
			{
				mpAlias[cAlias] = cFile;
			}
		}
	}
}

mapping * GetMappings()
{
	return ({ mpCmdsByLevel, mpAlias, mpCmdTables });
}

// 格式化参数
// * 默认是string
private mixed FormatArgs(object oUser, string type, int i, mixed args)
{
	mixed res;
	mixed mVal;
	if ( i >= 0 )
	{	
		if ( sizeof(args) <= i ) return ({TRUE, 0});
			
		mVal = all_trim(args[i]);
	}	
	// i < 0 的情况只有一种：cmdid or cmdobj，此参数处在的一个位置
	
	switch (type) 
	{
		case "int":
			return ({ TRUE, to_int(mVal) });

		case "uob":
		{
			int uid = to_int(mVal);
			object ob = get_user(uid);

			if ( objectp(ob) )
			{
				return ({TRUE, ob});
			}
			else
			{
				return ({ FALSE, "对方不在线"});
			}
		}
		case "uid":
		{
			int uid = to_int(mVal);
			if ( !get_user(uid) ) return  ({ FALSE, "对方不在线"});

			return ({TRUE, uid});
		}	
		case "obj":
		{
			object ob;
			
			if ( objectp(mVal) )
				ob = mVal;
			else
				ob = find_object(mVal);
			return ({ objectp(ob) ? TRUE : FALSE, ob});
		}
		case "i_list": 
		{
			string *s_list = explode(args[i], " ");
			int *i_list =({});

			foreach(string s_val in s_list)
			{
				i_list += ({to_int(s_val)});
			}
			return ({TRUE, i_list}); 
		}
		case "s_list":
		{
			return  ({TRUE, explode(args[i], " ")});
		}
		case "string":
		{	
			if ( !stringp(mVal) || !sizeof(mVal) )
				return ({ FALSE, mVal });	
			return ({ TRUE, mVal });
		}	
		case "float":
			return ({TRUE, to_float(mVal)});

		case "cmdid":
			return ({ TRUE, oUser->getId() });

		case "cmdobj":
			return ({ TRUE, oUser });

		case "*":
			return ({TRUE, mVal});
		case "...":
			mVal = implode(args[i..], ",");
			return ({TRUE, mVal});
	}
	return ({FALSE,"参数错误"});
}

mapping ParseArgs(object UserObj, string cCmd, string StrArgs)
{
	string cFileName = "cmd/wizcmds/" + cCmd + ".c";
	object CmdObj ;
	mapping mpConf = ([]);
	mapping mpRes = ([ "IsSuccess" : 0, "Ret" : "", ]);
	string cHelpMsg;
	mixed *format_args = ({ "doCmd", });
	mixed args;
	string *formats;
	int i = 0;
	int iIgnore = 0;
	int _tmp = 0;

	CmdObj = find_object(cFileName);
	StrArgs = replace_string( StrArgs, "selfid", sprintf("%d", UserObj->getId()) );
	StrArgs = replace_string( StrArgs, "selfobj", sprintf("get_user(%d)", UserObj->getId()) );

	/*
	if (cCmd == "eval")
	{
		format_args += ({ StrArgs });
		return ([ "IsSuccess" : 1,  "Ret" : format_args ]);
	}
	else if (cCmd == "call")
	{
		format_args += ({ UserObj->getId(), StrArgs });
		return ([ "IsSuccess" : 1,  "Ret" : format_args ]);
	}
	*/

	args = explode(StrArgs, ",");

	mpConf = CmdObj->getConf();
	cHelpMsg = mpCmdTables[cCmd]["Usage"];

	if (undefinedp(mpConf["Format"]) )  
	{
		throw( sprintf("%s命令，Format字段必填", cCmd) );
	}

	formats = explode(mpConf["Format"], ",");
	while (i < sizeof(formats)) 
	{
		string type = formats[i];

		if ( strsrch(type, "cmd") == 0)
		{	
			if ( i != 0 )
			{
				return ([ "IsSuccess" : 0,  "Ret" : "Format字段错误，cmdid or cmdobj必须在第一个位置" ]);
			}
			iIgnore ++;

			if ( iIgnore > 1 )
			{
				return ([ "IsSuccess" : 0,  "Ret" : "Format字段错误，cmdid or cmdobj只能存在一个" ]);
			}
		}
		else if ( type == "*" )
		{
			_tmp ++;
		}

		i++;
	}	

	i = sizeof(args);

	if (formats[sizeof(formats) - 1] != "...") {
		if ( i > sizeof(formats) || (i+_tmp+iIgnore) < sizeof(formats))
		{
			string cArgCount;
			int _min, _max;

			_max = sizeof(formats);
			_min = _max - _tmp - iIgnore;
			if ( _min != _max )
			{
				cArgCount = sprintf("参数必须在%d-%d个之间,", _min, _max );
			}
			else
				cArgCount = sprintf("参数必须有%d个,", _max);

			return ([ "IsSuccess" : 0,  "Ret" : cArgCount + cHelpMsg ]);
		}
	}
	for ( i=0; i < sizeof(formats); i ++)
	{
		string type = formats[i];
		mixed mRet;
		
		mRet = FormatArgs(UserObj, type, i-iIgnore, args);

		if (mRet[0] != TRUE ) 
		{
			return ([ "IsSuccess" : 0,  "Ret" : sprintf("第%d个参数, %s, %s", i-iIgnore+1, mRet[1], cHelpMsg) ]);
		}
		format_args += ({mRet[1]});
	}

	return ([ "IsSuccess" : 1,  "Ret" : format_args ]);
}

// 客户端掉用指令串
// ...协议解析部分太丑了吧。。。
void rpc_server_execute(int uid, string sCommand)
{
	string *Tmp;
	object oUser;
	mapping mpRet;
	string cCmd, StrArgs, cName;
	string cFileName;
	object CmdObj;
	debug_message("%O", sCommand);
	log_file("wizcmd", "uid=%d sCommand=%s\n", uid, sCommand);
	oUser = get_user(uid);
	if (!objectp(oUser) || !sizeof(sCommand)  )
	{
		return;
	}

	Tmp = explode( sCommand, " ");
	if ( !sizeof(Tmp) )
	{	
		return ;
	}

	cCmd = Tmp[0];
	if (mpAlias[cCmd]) cCmd = mpAlias[cCmd];	

	if ( !mpCmdTables[cCmd] 
			|| Import("WIZARD")->wizardLevel(oUser)  < mpCmdTables[cCmd]["Level"] )
	{
		log_file("wizcmd", "leave 283 %d\n", mpCmdTables[cCmd]["Level"]);
		return ;
	}

	cFileName = __DIR__ + "wizcmds/" + cCmd ;
	CmdObj = find_object(cFileName);
	if ( !objectp(CmdObj) ) 
	{
		log_file("wizcmd", "leave 291\n");
		return;
	}

	StrArgs = implode( Tmp[1..], " ");
	cName = oUser->getName();
	mpRet = ParseArgs(oUser, cCmd, StrArgs);

	if ( mpRet["IsSuccess"] != 1 )
	{
		oUser->tellTips("错误." + mpRet["Ret"] );
		return ;
	}	

	mpRet = call_other(CmdObj, mpRet["Ret"] );
	log_file("wizcmd", sprintf("[%s][%s][%d]wizcmd[%s].\n", short_time(), cName, uid, sCommand));

	if ( !objectp(oUser) ) return ;

	// oUser->TellMe( sprintf( "command: %s, args=%s, ret: %O", sCommand, StrArgs, mpRet) );
	if ( mapp(mpRet) && stringp(mpRet["Ret"]) )
		oUser->tellTips( cCmd + " "+ mpRet["Ret"]);
	else
		oUser->tellTips( sprintf( "%O", mpRet) ); 
}


void create()
{
	BuildCmdTabls();
	// debug_message(sprintf("all wizard cmds:%O", GetMappings()));
}
