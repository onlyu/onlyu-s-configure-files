#include <friend.h>

void initFriend()
{
	if (!mapp(friends))
		friends = ([]);	
	if (undefinedp(friends[K_SECTIONS]))
		friends[K_SECTIONS] = ([]);

	if (undefinedp(friends[K_FRIENDS]))
		friends[K_FRIENDS] = ([]);

	if (undefinedp(friends[K_RECENT_CONTACTORS]))
		friends[K_RECENT_CONTACTORS] = ({});

}

int* getRecentContactorList()
{
	return friends[K_RECENT_CONTACTORS];
}

mapping getFriendMap()
{
	return friends[K_FRIENDS];
}

int getFriendSectionId(int friendId)
{
	return getFriendMap()[friendId];
}

void setFriendSectionId(int friendId, int sectionId)
{
	friends[K_FRIENDS][friendId] = sectionId;
}

int* getFriendList()
{
	int* friendList = keys(friends[K_FRIENDS]);
	return friendList;
}

void addFriend(object friendUser)
{
	friends[K_FRIENDS][friendUser->getId()] = SECTION_MY_FRIEND;		
}

void delFriend(int friendId)
{
	map_delete(friends[K_FRIENDS], friendId);

}

int hasFriend(int friendId)
{
	mapping friendMap = getFriendMap();
	debug_message(sprintf("hasfriend map:%O", friendMap));
	if(undefinedp(friendMap[friendId]))
	
		return 0;
	return 1;
}

int isRecentContactor(int uid)
{
	int* recentContactors = getRecentContactorList();
	if (member_array(uid, recentContactors, 0) != -1)
		return 1;
	return 0;
}

int addRecentContactor(int contactorId)
{
	int removedContactor = 0;

	object contactor = get_user(contactorId);
	if(!objectp(contactor))
		return removedContactor;

	int isRecentCont = isRecentContactor(contactorId);
	int* recentContactors = getRecentContactorList();
	debug_message(sprintf("addRecentContactor id:%d beforeList:%O", this_object()->getId(), recentContactors));
	if(isRecentCont)
		recentContactors -= ({contactorId});
	recentContactors = ({contactorId}) + recentContactors;
	int size = sizeof(recentContactors);
	if(size > MAX_RECENT_CONTACTORS)
	{
		removedContactor = recentContactors[size - 1];	
	}
	if (removedContactor)
	{
		recentContactors -= ({removedContactor});
		Import("FRIEND")->sendDelRecentContactor(this_object(), removedContactor);
	}
	friends[K_RECENT_CONTACTORS] = recentContactors;
	if (!isRecentCont)
		Import("FRIEND")->sendAddRecentContactor(this_object(), contactorId);
	return removedContactor;
}

int delRecentContactor(int contactId)
{
	int isRecentCont = isRecentContactor(contactId);	
	if (isRecentCont)
	{
		int* recentContactors = getRecentContactorList();
		recentContactors -= ({contactId});
		Import("FRIEND")->sendDelRecentContactor(this_object(), contactId);
		friends[K_RECENT_CONTACTORS] = recentContactors;
	}
}

int* getOlStrangeContactorList()
{
	int* olStrangeContactorList = ({});
	int* recentContactorList = getRecentContactorList();
	foreach( int uid in recentContactorList)
	{
		if (!hasFriend(uid) && objectp(get_user(uid)))
			olStrangeContactorList += ({uid});
	}
	return olStrangeContactorList;
}
