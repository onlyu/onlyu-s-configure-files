// 系統配置文件

//#include "/include/config.c"
#include <system_config.h>

// 靜態的mapping不允許隨便腳本修改
static mapping config = ([
	// ---------------------------------------------------------------
	// 最大在線人數
	MAX_ONLINE_KEY : 10000,
	// 允許登錄人數
	MAX_LOGIN_KEY : 100,
	// 系統心跳間隔秒
	SYSTEM_INTERVAL_SEC  : 0,
	// 系統心跳間隔毫秒
	SYSTEM_INTERVAL_USEC : 500000,
	// ---------------------------------------------------------------
	RPC_CFG:"rc/rpc/rpc.cfg",
	//netd 内部对game开放的ip 
	NETD_IP:"127.0.0.1",
	//netd 内部对game开放的port
	NETD_PORT:6662,
	//netd对client开放的端口
	NETD_OUTER_PORT:6661,
	//dbd 内部对game开放的ip 
	DBD_IP:"127.0.0.1",
	//dbd 内部对game开放的port
	DBD_PORT:6663,

	ENGINE_NAME: "fs",

	NETD_HOST_PORT:6662,
	NETD_AUTH_PORT:6663,
	
	SERVER_INI:([]),
]);


mixed GetConfig(string key)
{
	return config[key];
}

mapping GetServerInfo()
{
	return  config[SERVER_INI];
}

void SetServerInfo(mapping m)
{
	config[SERVER_INI] = m;
}

mapping GetHostInfo(int hid)
{
	return config[SERVER_INI][hid];
}

int GetMaxLogin()
{
	return config[MAX_LOGIN_KEY];
}

void create()
{
	mapping configMap;
	string configFile;

	configFile = get_config_file();
	debug_message( "configFile = " + get_config_file());
	if ( configFile == "" )
	{ 
		configMap = File2Map("/etc/config/config.cfg");
	}
	else
	{
		configMap = File2Map(configFile);
	}

	foreach(mixed pairKey,mixed pairValue in configMap )
	{
		debug_message( sprintf("READ CONFIG: %O=%O",pairKey, pairValue) );	
		config[pairKey] = pairValue;
	}	
}

