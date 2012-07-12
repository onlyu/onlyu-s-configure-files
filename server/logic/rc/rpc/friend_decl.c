class Friend 
{
	int sectionId;
	int uid;
	int icon; 
	int grade;
	string name;
	int state; 
}

class Section
{
	int sectionId;	
	string name;
}

class AllFriends
{
	class Section* sections;
	class Friend* friends;
}

class Message{
        int time;
        string sender;
        string msg;
	int grade;
	int icon;
}

void rpc_client_all_friend_info(int uid, class AllFriends allFriends);	

void rpc_client_friend_info(int uid, class Friend friend);

void rpc_client_add_friend(int uid, class Friend friend);

void rpc_server_add_friend(object user, int friendId);

void rpc_server_add_friend_byname(object user, string name);

void rpc_client_friend_logout(int uid, int friendId);

void rpc_server_del_friend(object user, int friendId);

void rpc_client_del_friend(int uid, int friendId);

void rpc_server_friend_move_section(object user, int friendId, int sectionId);

void rpc_client_friend_move_section(int uid, int friendId, int sectionId);

void rpc_server_friend_message(object oUser, int toUid, string msg);

void rpc_client_friend_message(int uid, int fromUid, class Message msg);

void rpc_client_recent_contactor_list(int uid, class Friend* recentContactorList);

void rpc_client_recent_contactor_add(int uid, class Friend recentContactor);

void rpc_client_recent_contactor_del(int uid, int delUid);

void rpc_client_recent_contactor_login(int uid, int contactorId);

void rpc_client_recent_contactor_logout(int uid, int contactorId);

































