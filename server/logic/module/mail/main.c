#include "/module/mail/interface.h"


void sendUserChatMsg(int uid, class ChatMsg msg)
{
}

void sendSysMail(int uid, class MailMsg mailMsg)
{
}

void sendMultiUserChatMsg(int uid,  class ChatMsg* msgList)
{
}

void sendMultiSysMail(int uid, class MailMsg* mailMsgList)
{
}

class ChatMsg popUserChatMsg(int uid, int fromUserId)
{
}

class ChatMsg* popAllUserChatMsg(int uid, int fromUserId)
{
}

void sendOfflineChatMsgs(int uid, int fromUserId)
{
	object user = get_user(uid);
	if (!objectp(user))
		return;

	class ChatMsg* offlineMsgs = popAllUserChatMsg(uid, fromUserId);
	if(offlineMsgs && sizeof(offlineMsgs))
		sendMultiUserChatMsg(uid, offlineMsgs);
}

