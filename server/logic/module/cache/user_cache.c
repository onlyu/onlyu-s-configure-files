#include <user_key.h>
#include <lru.h>

#define MAX_USER_CACHE       50000
#include <macros.h>

static string saveName = "userCache";

static int isLoaded = 0;

void initUserCache()
{
	if (!isLoaded) {
		initLRU(MAX_USER_CACHE);
		isLoaded = 1;
	}
}


void create()
{
	setSaveName(saveName);
	doRestore();
	call_out("InitUserCache", 1);
}

// 有需要时，缓存用户的数据
void setUserPropData(int uid, mixed data)
{
	setLruData(uid, data);
}


void updateUserPropData(object oUser)
{
	setUserPropData(oUser->GetId(), oUser->GetProps() );
}	

private mixed getUserProp(int uid, mixed key)
{
	mapping prop;
	object oUser = get_user(uid);

	if (objectp(oUser)) {
		if (!is_online(uid)) {
			debug_message("出现数据错误， 玩家不在先，但是数据在线!");
		}
		return oUser->getProp(key);
	}

	initUserCache();

	prop = getLruData(uid);
	if ( prop )
	{
		return prop[key];
	}

	if ( !isUserExist(uid) ) return 0;

	log_file("user_cache", sprintf("[%s][%d] conflict\n", short_time(), uid));

	return "rc/user_data_op"->getProp( uid, key, (: setUserPropData :) );
}

string getUserName(int uid)
{
	return getUserProp(uid, K_USER_NAME);
}

int getUserGrade(int uid)
{
	return getUserProp(uid, K_USER_GRADE);
}

int getUserResource(int uid)
{
	return getUserProp(uid, K_USER_ICON);
}























































































































