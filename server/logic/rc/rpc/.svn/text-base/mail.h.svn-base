#ifndef _rc_rpc_mail
#define _rc_rpc_mail

#include "/rc/rpc/rpc_id.h"


class MailMsg
{
}

class ChatMsg
{
}

void rpc_client_chatmsg(int uid, int toUid, class ChatMsg chatMsg) { fs_rpc_call(uid, RPC_CLIENT_CHATMSG, toUid, chatMsg); }

void rpc_client_multi_chatmsg(int uid, int toUid, class ChatMsg* multiChatMsg) { fs_rpc_call(uid, RPC_CLIENT_MULTI_CHATMSG, toUid, multiChatMsg); }

#endif
