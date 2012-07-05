#ifndef _rc_rpc_worldmap
#define _rc_rpc_worldmap

#include "/rc/rpc/rpc_id.h"


class City
{
	int x;
	int y;
	string name;
	int cityId;
	int icon;
	int lowGrade;
	int highGrade;
}

void rpc_server_worldmap_info(object user);

void rpc_client_worldmap_info(int uid, class City* cityList, int cityId) { fs_rpc_call(uid, RPC_CLIENT_WORLDMAP_INFO, cityList, cityId); }

void rpc_server_worldmap_entercity(object user, int cityId);





























































#endif
