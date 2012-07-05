
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

void rpc_client_worldmap_info(int uid, class City* cityList, int cityId);

void rpc_server_worldmap_entercity(object user, int cityId);





























































