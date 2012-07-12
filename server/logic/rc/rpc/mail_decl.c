
class MailMsg
{
}

class ChatMsg
{
}

void rpc_client_chatmsg(int uid, int toUid, class ChatMsg chatMsg);

void rpc_client_multi_chatmsg(int uid, int toUid, class ChatMsg* multiChatMsg);

