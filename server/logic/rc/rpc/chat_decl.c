
class chat_t {
	int uid;
	string name;
	int channel;
	string message;
}

class ChatFrameInfo
{
	int uid;	
	string name;
	int grade;
	int online;
	int isFriend;
	int icon;
}

void rpc_server_chat(object user, int channel, string message);
void rpc_client_chat(int uid, class chat_t chat);

// NPC冒泡
void rpc_client_npc_say(int uid, int npcid, string message);

// 广播频道单独处理 (- -)
void rpc_client_broadcast(int uid, class chat_t chat);
void rpc_server_broadcast(object user, string message);

void rpc_client_reward_broadcast(int uid, string msg);

void rpc_client_tell_me(int uid, string msg);

void rpc_server_chatframe_info(object user, int toUid);

void rpc_client_chatframe_info(int uid, class ChatFrameInfo chatFrameInfo);
//玩家信息

