#ifndef _rc_rpc_login
#define _rc_rpc_login

#include "/rc/rpc/rpc_id.h"


// 验证协议头文件
void rpc_server_version(int vfd, string engine_version, string script_version, string pto_md5);
void rpc_client_version_return(int vfd, int stat, string encoding) { fs_rpc_call(vfd, RPC_CLIENT_VERSION_RETURN, stat, encoding); }
void rpc_client_update_pto(int vfd, string protocols) { fs_rpc_call(vfd, RPC_CLIENT_UPDATE_PTO, protocols); }

// 选择uid进行游戏
void rpc_server_login(int vfd, string ums);

void rpc_gamed_user_login_send(int gamed, int vfd, int uid) { fs_rpc_call(gamed, RPC_GAMED_USER_LOGIN, vfd, uid); }
void rpc_gamed_user_login(int gamed, int vfd, int uid);

// 没有uid
void rpc_netd_no_uid_send(int netd, int vfd, int uid) { fs_rpc_call(netd, RPC_NETD_NO_UID, vfd, uid); }
void rpc_netd_no_uid(int netd, int vfd, int uid);
void rpc_client_no_uid(int vfd, int uid) { fs_rpc_call(vfd, RPC_CLIENT_NO_UID, uid); }

// 新建uid
void rpc_server_new_uid(int vfd, int uid, int icon, string name);
void rpc_gamed_new_uid_send(int gamed, int vfd, int uid, int icon, string name) { fs_rpc_call(gamed, RPC_GAMED_NEW_UID, vfd, uid, icon, name); }
void rpc_gamed_new_uid(int gamed, int vfd, int uid, int icon, string name);
#endif
