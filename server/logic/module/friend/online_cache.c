// 缓冲名字查找信息
#include <var_prop.h>

MEMORY_VAR(mpOnlineNames, ([]))
// 保存加我为好友的列表，方便广播上下线消息
MEMORY_VAR(mpOlFriends, ([]))

void addFriend(int uid, int friendId)
{
	if ( undefinedp( mpOlFriends[uid]) )
	{
		mpOlFriends[uid] = ({ friendId });	
		return ;
	}

	mpOlFriends[uid] += ({ friendId });
}

void delFriend(int uid, int friendId)
{
	if ( undefinedp( mpOlFriends[uid]) )
	{
		return ;
	}
	mpOlFriends[uid] -= ({ friendId });
	if ( !sizeof( mpOlFriends[uid]) )
		map_delete( mpOlFriends, uid );

}

int getOnlineUserId(string name)
{
	if (undefinedp(mpOnlineNames[name])) {
		return 0;
	}
	return mpOnlineNames[name];
}

int* getOnlineFriendList(int uid)
{
	int* onlineFriendList = mpOlFriends[uid];
	if (!onlineFriendList)
		onlineFriendList = ({});
	return onlineFriendList;
}

void loginCheck(object user)
{
	string name = user->getName();
	int uid = user->getId();

	mpOnlineNames[name] = uid;

	foreach ( int friendId in user->getFriendList() )
	{
		addFriend(friendId, uid);
	}
}

void logoutCheck(object user)
{
	string name = user->getName();
	int uid = user->getId();

	map_delete(mpOnlineNames,name);

	foreach ( int friendId in user->getFriendList() )
	{
		delFriend(friendId, uid);
	}
	debug_message("online friend cache logout");
}


















































