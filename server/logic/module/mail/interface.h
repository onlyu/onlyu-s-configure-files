#include "/rc/rpc/mail.h"

void sendUserChatMsg(int uid, class ChatMsg msg);

void sendSysMail(int uid, class MailMsg mailMsg);

void sendMultiUserChatMsg(int uid, class ChatMsg* msgList);

void sendMultiSysMail(int uid, class MailMsg* mailMsgList);

class ChatMsg popUserChatMsg(int uid, int fromUserId);

class ChatMsg* popAllUserChatMsg(int uid, int fromUserId);


void sendOfflineChatMsgs(int uid, int fromUserId);
