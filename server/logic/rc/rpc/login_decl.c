
// 验证协议头文件
void rpc_server_version(int vfd, string engine_version, string script_version, string pto_md5);
void rpc_client_version_return(int vfd, int stat, string encoding);
void rpc_client_update_pto(int vfd, string protocols);

// 选择uid进行游戏
void rpc_server_login(int vfd, string ums);

void rpc_gamed_user_login(int gamed, int vfd, int uid);

// 没有uid
void rpc_netd_no_uid(int netd, int vfd, int uid);
void rpc_client_no_uid(int vfd, int uid);

// 新建uid
void rpc_server_new_uid(int vfd, int uid, int icon, string name);
void rpc_gamed_new_uid(int gamed, int vfd, int uid, int icon, string name);
