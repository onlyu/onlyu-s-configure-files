
#include <debug.h>
#include <macros.h>
#include <system_config.h>
#include </rc/rpc/login.h>
#include </rc/db.h>

static string server_ptofile_md5;
static string ptofile_content;

static object logger;

static void calcmd5()
{
	string sys_rpc_cfg = "/rc/rpc/rpcJson.cfg";
	ptofile_content = read_file(sys_rpc_cfg);
	server_ptofile_md5 = md5_hexdigest(ptofile_content);
	logger->Log("cal protocl file md5 md5=%s", server_ptofile_md5);
}

string getptomd5()
{
	if (undefinedp(server_ptofile_md5)) {
		calcmd5();
	}

	return server_ptofile_md5;
}

void create()
{
	logger = Import("LOG")->New("login");
}


int is_internal_host()
{
	return 1;
}

#define DEFAULT_ENCODING "GB18030"

//客户端连接上来之后发的第一条协议
void rpc_server_version(int vfd, string engine_version, string script_version, string pto_md5)
{
	logger->Log("client protocl version vfd=%d,e=%s,s=%s,md5=%s", vfd, engine_version, script_version, pto_md5);

	string encoding = DEFAULT_ENCODING;

	if (pto_md5 == getptomd5()) {
		rpc_client_version_return(vfd, 0, encoding);
		logger->Log("client version ok vfd=%d", vfd);
	} else {

		if (is_internal_host()) {
			//内服要每次都重读一下协议文件，这样的话可以不用重启
			calcmd5();
			rpc_client_update_pto(vfd, ptofile_content);
			logger->Error("update pto to bad version client.vfd=%d,engine=%s,client=%s,ptomd5=%s",
				vfd, engine_version, script_version, pto_md5);
		} else {
			fs_close_vfd_connection(vfd);
			logger->Error("close bad version client.vfd=%d,engine=%s,client=%s,ptomd5=%s",
					vfd, engine_version, script_version, pto_md5);
		}

		rpc_client_version_return(vfd, 1, encoding);
	}
}

static int ums2ID(string ums) 
{
	int c = 0;
	if (stringp(ums)) {
		int i;
		for (i = 0; i < sizeof(ums); ++i) {
			c = c + ums[i]*9;
		}
	}
	return c;
}

// uid进入游戏,登录完成
//1,  登录
void rpc_server_login(int vfd, string ums)
{
	int uid = ums2ID(ums);
	logger->Log("login ums=%s,uid=%d", ums, uid);
	assert(uid > 0);
	if (fs_is_uid_connected(uid)) {
		int oldvfd = fs_get_vfd_by_uid(uid);
		logger->Log("relogin uid into game vfd=%d,ums=%s,uid=%d", vfd, ums, uid); 
		fs_close_uid_connection(uid);
	}

	fs_new_user(vfd, uid);
        //to gamed
	rpc_gamed_user_login_send(FS_GAMED_UID, vfd, uid);
	logger->Log("uid enter game vfd=%d,ums=%s,uid=%d", vfd, ums, uid);
}

//2, 没有uid
void rpc_netd_no_uid(int netd, int vfd, int uid)
{
        //gamed->netd->client
        debug_message("rpc_netd_no_uid %d %d %d", netd, vfd, uid);
        rpc_client_no_uid(vfd, uid);
}


//3, 新建uid
void rpc_server_new_uid(int vfd, int uid, int icon, string name)
{
        debug_message("rpc_server_new_uid, uid:%d", uid);
        // TODO 判断vfd跟uid是否合法

        if( isUserExist(uid) ) {
                debug_message("uid %d has exists, vfd:%d", uid, vfd);
                return;
        }
        //client -> netd-> gamed
        rpc_gamed_new_uid_send(FS_GAMED_UID, vfd, uid, icon, name);
        debug_message("rpc_server_new_uid 22222, uid:%d", uid);
}

//引擎回调
void netd_client_connection_close_cb(int vfd)
{
}


//////以下在gamed中运行

//引擎回调
void gamed_client_connection_close_cb(int vfd)
{
}


private void userLoadCB(object user, int stat, int vfd)
{
	int uid = user->getId();
	if (user == 0) {
		logger->Error("login user load cb already not online uid=%d", uid);
		return;
	}

	debug_message(sprintf("on user load uid:%d,isok:%d\n", uid, stat));
	if (stat == DB_STAT_OK) {
		logger->Log("gamed load user uid=%d", uid);
		user->login(uid);
	} else {
		if (stat == DB_STAT_NULL) {
			logger->Warn("login new null user.uid=%d", uid);
                        rpc_netd_no_uid_send(FS_NETD_UID, vfd, uid);
		} else {
			logger->Error("error login load user.uid=%d", uid);
		}
	}
}

void rpc_gamed_new_uid(int gamed, int vfd, int uid, int icon, string name)
{
        debug_message("rpc_gamed_new_uid %d", uid);

        object user = get_user(uid);
        if( !objectp(user) ) {
                debug_message("can't get user by uid:%d", uid);
                return;
        }

        user->born(uid, icon, name);
        debug_message("[%d] born", uid);
}


void rpc_gamed_user_login(int gamed, int vfd, int uid)
{
	if ("rc/db"->isLoading(uid)) {
		//正在加载数据，可能是上次登录未完成，也可能是异步的虚拟登录
		logger->Error("login user already loading.uid=%d", uid);
		gamed_close_uid_connection(uid);
		return;
	}

	if (is_online(uid)) {
		get_user(uid)->reLogin();
		return;
	}

	int freeLoginCnt = "rc/config"->GetMaxLogin() - get_online_count() - "rc/db"->getUserLoadingCnt();

	debug_message("free login: %d", freeLoginCnt);
	// int ret = user_create(uid);
	if (freeLoginCnt <= 0) {
		logger->Error("login user online num reach max! uid=%d", uid);
		return;
	}

	logger->Log("gamed async load user uid=%d", uid);
	Import("DB")->load_user_async(uid, (:userLoadCB:), vfd);
}
