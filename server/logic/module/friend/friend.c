#include <lru.h>
#include "/rc/rpc/friend.h"
#include "/module/friend/tips.h"
#include <friend.h>

#define ONLINE_CACHE "/module/friend/online_cache"

static mapping mpDefaultSection = ([
		SECTION_MY_FRIEND : "我的好友",
		SECTION_BLACK_LIST : "黑名单",
		]);

private int getState(int uid)
{
	object oUser = get_user(uid);

	if ( !objectp(oUser) ) return STATE_OFFLINE;

	return STATE_ONLINE;
}

private class Section* loadSections(object user)
{
	class Section* sections = ({});
	foreach(int sectionId, string name in mpDefaultSection)
	{
		sections += ({new(class Section, sectionId:sectionId, name:name)});
	}
	return sections;
}


private class Friend packFriend(int friendId, int sectionId)
{
	class Friend friend = new(class Friend);
	friend->uid = friendId;
	friend->sectionId = sectionId;
	friend->name = Import("USER_CACHE")->getUserName(friendId);
	friend->grade = Import("USER_CACHE")->getUserGrade(friendId);
	friend->icon = Import("USER_CACHE")->getUserResource(friendId);
	friend->state = getState(friendId);
	return friend;
}

void sendAddFriend(object user, int friendId, int sectionId)
{
	rpc_client_add_friend(user->getId(), packFriend(friendId, sectionId));
}

void sendFriendInfo(object user, int friendId, int sectionId)
{
	class Friend friend = packFriend(friendId, sectionId);
	rpc_client_friend_info(user->getId(), friend);
	//debug_message(sprintf("send friend Info:%O", friend));
}

private class Friend* loadFriends(object user)
{
	class Friend* friends = ({});
	mapping mpFriends = user->getFriendMap();	
	foreach(int friendId, int sectionId in mpFriends)
	{
		friends += ({packFriend(friendId, sectionId)});	
	}
	return friends;
}

private class Friend* loadRecentContactors(object user)
{
	class Friend* friends = ({});
	int* recentContactorList = user->getRecentContactorList();
	int sectionId;
	foreach(int contactorId in recentContactorList)
	{
		sectionId = user->getFriendSectionId();
		friends += ({packFriend(contactorId, sectionId)});
	}
	return friends;
}

void loadFriendData(object user)
{
	class Section* sections = loadSections(user);	
	class Friend* friends = loadFriends(user);
	class Friend* recentContactorList = loadRecentContactors(user);
	class AllFriends allFriends = new(class AllFriends, sections:sections, friends:friends);
	rpc_client_all_friend_info(user->getId(),allFriends);	
	//debug_message(sprintf("load friend id:%d sections:%O, friends:%O", user->getId(), sections, friends)); 
	if (recentContactorList && sizeof(recentContactorList))
		rpc_client_recent_contactor_list(user->getId(), recentContactorList);
}

void notifyLogin(object user, int friendId)
{
	object friendUser = get_user(friendId);
	if(!objectp(friendUser))
		return;
	if (!friendUser->hasFriend(user->getId()))
		return;

	friendUser->tellMe(sprintf("您的好友%s上线了", user->getName()));
	sendFriendInfo(friendUser, user->getId(), user->getFriendSectionId(friendUser->getId()));
}

void notifyLoginAll(object user)
{
	int* onlineFriendList = ONLINE_CACHE->getOnlineFriendList(user->getId());
	//debug_message(sprintf("login user:%s onlinefriendlist:%O", user->getName(), onlineFriendList));
	foreach(int friendId in onlineFriendList)
	{
		notifyLogin(user, friendId);
	}
}

void notifyStrangeContactorLoginAll(object user)
{
	int* strangeContactorList = user->getOlStrangeContactorList(); 
	foreach(int strangeContactor in strangeContactorList)
		rpc_client_recent_contactor_login(user->getId(), strangeContactor);
}

void notifyStrangeContactorLoginOutAll(object user)
{
	int* strangeContactorList = user->getOlStrangeContactorList(); 
	foreach(int strangeContactor in strangeContactorList)
		rpc_client_recent_contactor_logout(user->getId(), strangeContactor);
}
void notifyLogout(object user, int friendId)
{
	object friendUser = get_user(friendId);
	if(!objectp(friendUser))
		return;
	if (!friendUser->hasFriend(user->getId()))
		return;
	friendUser->tellMe(sprintf("您的好友%s下线了", user->getName()));
	rpc_client_friend_logout(friendUser->getId(), user->getId());
}

void notifyLogoutAll(object user)
{
	int* onlineFriendList = ONLINE_CACHE->getOnlineFriendList(user->getId());
	//debug_message(sprintf("logout user:%s onlinefriendlist:%O", user->getName(), onlineFriendList));
	foreach(int friendId in onlineFriendList)
	{
		notifyLogout(user, friendId);
	}
}

void loginCheck(object user)
{
	user->initFriend();
	ONLINE_CACHE->loginCheck(user);
	loadFriendData(user);
	notifyLoginAll(user);
	notifyStrangeContactorLoginAll(user);
}

void logoutCheck(object user)
{
	notifyLogoutAll(user);
	notifyStrangeContactorLoginOutAll(user);
	ONLINE_CACHE->logoutCheck(user);
}

int getOnlineUserId(string name)
{
	return ONLINE_CACHE->getOnlineUserId(name);
}

void sendDelRecentContactor(object user, int delId)
{
	rpc_client_recent_contactor_del(user->getId(), delId);
}

void sendAddRecentContactor(object user, int addId)
{
	debug_message("send add contactor uid:%d addid:%d", user->getId(), addId);
	rpc_client_recent_contactor_add(user->getId(), packFriend(addId, 0));
}

private void addFriend(object user, object friendUser)
{
	ONLINE_CACHE->addFriend(user->getId(), friendUser->getId());
	user->addFriend(friendUser);
	sendAddFriend(user, friendUser->getId(), SECTION_MY_FRIEND);
}

void addFriendById(object user, int friendId)
{
	if (user->getId() == friendId)
		return;
	if (getState(friendId) == STATE_OFFLINE)
	{
		tellUserNotExist(user);
		return;
	}
	if (user->hasFriend(friendId))
	{
		tellUserHasFriend(user); 
		return;
	}
	object friendUser = get_user(friendId);
	addFriend(user, friendUser);
	tellUserAdd(user, friendUser->getName());
	tellUserBeAdded(friendUser, user->getName());
	if (!friendUser->hasFriend(user->getId()))
	{
		addFriend(friendUser, user);
	}
}

void addFriendByName(object user, string name)
{
	if(!sizeof(name))
		return;
	int friendId = getOnlineUserId(name);
	if(!friendId)	
	{
		tellUserNotExist(user);
		return;
	}
	addFriendById(user, friendId);
}

void rpc_server_add_friend_byname(object user, string name)
{
	addFriendByName(user, name);
}

void rpc_server_add_friend(object user,  int friendId)
{
	addFriendById(user, friendId);
}


void rpc_server_friend_move_section(object user, int friendId, int clientSectionId)
{
	int serverSectionId = user->getFriendSectionId(friendId);	
	//debug_message(sprintf("movesection uid:%d friendId:%d sectionId:%d serverSectionId:%d", user->getId(), friendId, clientSectionId, serverSectionId));
	if (!serverSectionId || serverSectionId == clientSectionId)
		return;
	if (!mpDefaultSection[clientSectionId])
		return;
	//debug_message("move section action");
	user->setFriendSectionId(friendId, clientSectionId);
	rpc_client_friend_move_section(user->getId(), friendId, clientSectionId);
}

void rpc_server_del_friend(object user, int friendId)
{
	debug_message(sprintf("delfriend uid=%d friendId=%d", user->getId(), friendId));
	if (!user->hasFriend(friendId))
		return;
	debug_message(sprintf("delfriend hasfriend uid=%d friendId=%d", user->getId(), friendId));
	user->delFriend(friendId);
	object friendUser = get_user(friendId);
	if (objectp(friendUser))
		ONLINE_CACHE->delFriend(user->getId(), friendId);
	rpc_client_del_friend(user->getId(), friendId);
	user->delRecentContactor(friendId);
	tellDelFriend(user, Import("USER_CACHE")->getUserName(friendId));
}

void rpc_server_friend_message(object user, int toUid, string msg)
{
	debug_message("server friend msg uid:%d", user->getId());
	object toUser = get_user(toUid);
	if(!objectp(toUser))
	{
		tellUserOffline(user);
		return;
	}
	int now = time();
	class Message message = new(class Message, time:now, sender:user->getName(), msg:msg, grade:user->getGrade(), icon:user->getIcon());
	rpc_client_friend_message(toUid, user->getId(), message);
	user->addRecentContactor(toUid);
	toUser->addRecentContactor(user->getId());
}

















































