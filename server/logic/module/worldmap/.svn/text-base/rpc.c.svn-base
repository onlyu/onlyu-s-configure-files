#include "/rc/rpc/worldmap.h"

void rpc_server_worldmap_info(object user)
{
	mapping cityData = "module/worldmap/city"->get_data();
	int *pos =  user->getLocation();
	int curSceneId = pos[0];
	mapping curCity = cityData[curSceneId];
	if (undefinedp(curCity))
	{
		user->tellTips("当前场景不能打开世界地图");
		return;
	}
	class City* cityList = ({});
	class City city;
	foreach(int cityId, mapping cityInfo in cityData)
	{
		city = new(class City);
		city->x = cityInfo["x"];
		city->y = cityInfo["y"];
		city->cityId = cityId;
		city->name = cityInfo["city"];
		city->icon = cityInfo["resourceId"];
		city->lowGrade = cityInfo["lowGrade"];
		city->highGrade = cityInfo["highGrade"];
		cityList += ({city});
	}
//	debug_message(sprintf("city list:%O", cityList));
	/*
	int x = curCity["x"];
	int y = curCity["y"];
	*/
	rpc_client_worldmap_info(user->getId(), cityList, curSceneId);
}

void rpc_server_worldmap_entercity(object user, int cityId)
{
	mapping cityData = "module/worldmap/city"->get_data();
	mapping cityInfo = cityData[cityId];
	if(undefinedp(cityInfo))
	{
		return;
	}
	int sceneId = cityId;
	user->enterScene(sceneId, cityInfo["zcx"], cityInfo["zcy"]);
}






































































