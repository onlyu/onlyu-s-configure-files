// 组队测试使用
#include <user_key.h>

#define REAL_NEED_WIZ_LV	(99)

mapping getConf()
{
	return ([
		"Level" : 4,
		"HelpMsg" : "setprop id,key,iValue",
		"Format" : "int,string,int",
		"Alias" : ({"设置属性",}),
	]);
}


static int checkLevel( object oUser )
{
	if( Import("WIZARD")->wizardLevel(oUser) >= REAL_NEED_WIZ_LV )
	{
		return 1;
	}
	
	return 0;
}

static void _offlineSetProp(object oUser, string prop, mixed value)
{
	if( !checkLevel(oUser) )	return;

	oUser->setProp(prop, value);
}

mixed doCmd(int uid, string cKey, int value)
{
	int size;
	object oTmpUser;
	
	switch( cKey )
	{
	case K_USER_GRADE:
		if ( value <= 0 || value > 120 )
		{
			return ([ "IsSuccess" : 0, "Ret" : "等级必须在0-120之间" ]); 
		}
		break;
	default:
		if ( cKey[0 .. 0] == "c" )
		{
			return ([ "IsSuccess" : 0, "Ret" : "不要设置字符串属性" ]); 
		}
		break;
	}


	oTmpUser = get_user(uid);

	if (objectp(oTmpUser)) 
	{
		if( !checkLevel(oTmpUser) )
		{
			return ([ "IsSuccess" : 0, "Ret" :"权限不够" ]); 
		}

		oTmpUser->setProp(cKey, value);
        }
	else {
		if ( !isUserExist(uid) ) 
		{
			return ([ "IsSuccess" : 0, "Ret" : "没有这个玩家" ]); 
		}
		"rc/db"->vuser_login(uid, (:_offlineSetProp:), cKey, value);
	}
	// 离线操作数据
	return ([ "IsSuccess" : 1, "Ret" : "done" ]); 
}
