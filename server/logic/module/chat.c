
#include <macros.h>
#include "/rc/rpc/chat.h"

void broadcastWorldChat(object user, string message)
{
	class chat_t chatObj = new(class chat_t);
	chatObj->uid=user->getId();
	chatObj->channel = 0;
	chatObj->name = user->getName();
	chatObj->message = message;
	fs_rpc_call(FS_BROADCAST_UID, RPC_CLIENT_CHAT, chatObj);

}

void rpc_server_chat(object user, int channel, string message)
{
	broadcastWorldChat(user, message);
	return;
}


void tips(int uid, string from, string message)
{
	class chat_t chat = new(class chat_t);
	chat->uid = uid;
	chat->name = from;
	chat->channel = CHANNEL_WORLD;
	chat->message = message;
	rpc_client_chat(uid, chat);
}

void tellMe(int uid, string msg)
{
	rpc_client_tell_me(uid, msg);
}

void rpc_server_chatframe_info(object user, int toUid)
{
	if (user->getId() == toUid)
		return;
	class ChatFrameInfo chatFrameInfo = new(class ChatFrameInfo);		
	object toUser = get_user(toUid);
	chatFrameInfo->uid=toUid;
	chatFrameInfo->isFriend = user->hasFriend(toUid);
	chatFrameInfo->name = "";
	if (objectp(toUser))
	{
		chatFrameInfo->name = toUser->getName();
		chatFrameInfo->grade = toUser->getGrade();
		chatFrameInfo->icon = toUser->getIcon();
		chatFrameInfo->online = 1;
	}
	else
	{
		chatFrameInfo->name = Import("USER_CACHE")->getUserName(toUid);
		chatFrameInfo->grade = Import("USER_CACHE")->getUserGrade(toUid);
		chatFrameInfo->icon = Import("USER_CACHE")->getUserResource(toUid);
	}
	debug_message("chatframe info:%O", chatFrameInfo);
	rpc_client_chatframe_info(user->getId(), chatFrameInfo);
}

