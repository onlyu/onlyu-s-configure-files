
#include <macros.h>

//mapping WarnLists = ([]);

/*
void AddTimingVar()
{
	object * AllUser = get_all_users();

	call_out("AddTimingVar", 86400);

	if ( AllUser )
	{
		foreach( object oUser in AllUser )
		{
			oUser->AddTimingVar();
		}
	}	
}
*/

void AllHeartBeat()
{
	object * AllUser = get_all_users();
	//mapping mpIps = ([]);

	if ( AllUser )
	{	
		int iTime = time();

		foreach ( object oUser in AllUser )
		{
			// 因为可能玩家正在异步LOAD...
			int uid = oUser->GetId();
			if (uid) {
				//mpIps[oUser->GetcIp()] += 1;
				oUser->HeartBeat();

				// 没上行finger key，肯定是有问题的 // 登录15秒之后
				if ( ((! oUser->GetTemp("finger_key")) && iTime - oUser->GetLoginTime() > 15)
				|| (oUser->GetTemp("ToKickout") && iTime > oUser->GetTemp("ToKickout") ) ) 
				{
					"module/util"->KickOutUser(uid);
				}
			}
		}
	}

	/*
	int iWarn = 10;
	foreach( string cIp, int _amount in mpIps )
	{
		if ( _amount <= iWarn )
			continue;

		if ( _amount - WarnLists[cIp] < iWarn )
			continue;

		debug_message( sprintf("file: %s login %d id.", cIp, _amount) );
		WarnLists[cIp] = _amount;
	}
	*/

	// 因为在acc奇遇模块自动释放比较麻烦，所以改为定时触发
	Import("SCENE_ACC")->CheckAcc();
}

void create()
{
	// Import("TIME_CTRL")->next_at("24:0:0", "AddTimingVar" );
	//Import("TIME_CTRL")->crontab(HEARTBEAT_TICKET, "AllHeartBeat");
}
