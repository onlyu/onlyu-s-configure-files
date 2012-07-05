#include "/rc/db.h"
#include <var_prop.h>
#include <log.h>

MEMORY_VAR(loading, ([]))
MEMORY_VAR(loading_sid, 1) //load db时和dbd交互时的sessionid
MEMORY_VAR(saving, ([]))
MEMORY_VAR(save_count, 0)


#define  LOADING_SESSION_ID(data)   data["sessionid"]
#define  LOADING_TYPE(data)         data["type"]
#define  LOADING_SYNC(data)         data["issync"]
#define  LOADING_CALLBACK(data)     data["callback"]
#define  LOADING_OBJECT(data)       data["object"]
#define  LOADING_SETUP_UID(data)    data["setupuid"]
#define  LOADING_IS_VUSER(data)     data["vuser"]


/*
!!!!注意:
从db load存盘数据的接口，返回 0 是正确情况，-1 是接口的其他错误，大于 0 是db返回回来的有关存盘文件的错误
*/

//采用存盘的方式
//#define SAVE_METHOD SAVE_ALL 
#define SAVE_METHOD SAVE_DIRTY 
//发送多少次全局的脏数据给db后才令db进行写盘
#define SAVE_SYNC_NUM 5 

static object logger;
varargs void save_all(int forcesync);

void enable_dirty_object(object obj)
{
	if (SAVE_METHOD == SAVE_DIRTY) {
		fs_enable_dirty_object(obj);
	}
}

int isLoading(mixed key)
{
	return !undefinedp(loading[key]);
}

void removeLoading(mixed key)
{
	map_delete(loading, key);
}

int getUserLoadingCnt()
{
	int total = 0;
	if (sizeof(loading)) {
		foreach (mixed _k, mapping _data in loading) {
			if (LOADING_TYPE(_data) == DB_TYPE_USER) {
				total += 1;
			}
		}
	}
	return total;
}

//处理DBD返回的数据
private object deal_user_load(int stat, int load_uid, buffer data, mapping mpLoad)
{
	int ret;
	if (stat != DB_STAT_OK) {
		return 0;
	}
	int uid = LOADING_SETUP_UID(mpLoad);

	object user = get_user(uid);
	if (objectp(user)) {
		logger->Error("user exist on loaded uid=%d", uid);
		return user;	
	}

	ret = user_create(uid);
	if (ret != 0) {
		logger->Error("user create fail on loaded uid=%d,ret=%d", uid, ret);
		fs_db_object_unload(uid);
		return 0;
	}
	user = get_user(uid);

	ret = fs_db_object_restore(user, data);
	logger->Log("db load restore uid=%d,ret=%d", uid, ret);

	user->setup(uid);
	enable_dirty_object(user);

	if (LOADING_IS_VUSER(mpLoad)) {
		user->setvLogin();
	}
	return user;
}

private object deal_dat_load(int stat, mixed key, buffer data, mapping mpLoad)
{
	object obj = LOADING_OBJECT(mpLoad);
	logger->Log("deal db load dat.key=%s", key);
	if (saving[key]) {
		logger->Log("warn dat already in saving dat.key=%O", key);
		return 0;
	}
	saving[key] = obj;
	//todo check the stat
	if (stat == DB_STAT_OK) {
		int ret = fs_db_object_restore(obj, data);
		logger->Log("deal db load restore dat.key=%s,ret=%d", key, ret);
	} else if (stat == DB_STAT_NULL) {
		//在启动脏数据之前强制全部存一次盘，由于支持同步和异步，
		//要先保证db和gamed的数据对象的状态是相同的，然后才能开始脏数据的合并
		logger->Log("save dat first for null.key=%s", key);
		fs_db_object_save(key, obj, SAVE_ALL);
	}
	//允许加载空的dat
	logger->Log("deal db load cb enable dirty dat.key=%s", key);
	enable_dirty_object(obj);

	return obj;
}

//处理回调
private void db_object_load_cb(int stat, mixed key, object obj, mapping mpLoad)
{
	mapping *callbacks = mpLoad["callback"];
	if( !sizeof(callbacks) ) return;

	logger->Log("callback type=%d,stat=%d,key=%O", LOADING_TYPE(mpLoad), stat, key);

	foreach( mapping _callback in callbacks ) {
		function func = _callback["function"];
		mixed arg = _callback["arg"];
		if (functionp(func)) {
			evaluate(func, obj, stat, arg);
		}
	}

}
// engine call back
private void on_db_object_load(int stat, mixed key, buffer data, int sessionid)
{
	logger->Log("db load cb stat=%d,key=%O,sessionid=%d", stat, key, sessionid);

	if (undefinedp(loading[key])) {
		logger->Warn("warn miss db load cb object.key=%O, stat = DB_STAT_OK? %d lest test user memory.", key, stat == DB_STAT_OK);
		return;
	}

	mapping mpLoad = loading[key];

	if (sessionid != LOADING_SESSION_ID(mpLoad)) {
		//异步情况下sessionid对不上：异步--同步--异步 才会产生的
		logger->Warn("sessionid invalid! avoid! %d---%d key=%d ", sessionid, LOADING_SESSION_ID(mpLoad), key);
		return;
	}

	removeLoading(key);
	logger->Log("db load cb remove loading stat=%d,key=%O", stat, key);

	object obj;

	switch (LOADING_TYPE(mpLoad)) {
		case DB_TYPE_USER:
			obj = deal_user_load(stat, key, data, mpLoad);
			break;
		case DB_TYPE_DAT:
			obj = deal_dat_load(stat, key, data, mpLoad);
			break;
		default:
			logger->Log("obj load type error:%d", mpLoad["type"]);
			return;
	}	
	//回调
	db_object_load_cb(stat, key, obj, mpLoad);

	// 处理掉uid
	//如果是虚拟登录并且是异步的就回收资源
	if (LOADING_TYPE(mpLoad) == DB_TYPE_USER && objectp(obj) && obj->isvLogin()) {
		logout(obj->getId());
		logger->Log("clear vlogin user uid=%d", obj->getId());
	}
}

private int _load_user(int uid, int issync, int load_id, int is_vuser, function on_load_cb, mixed arg)
{
	int iExist = loading[uid] ? 1 : 0;
	mapping mpLoad = ([]);

	//udata: 同步时才有用处
	if (!isLoading(uid)) {
		loading[uid] = ([]);
		mpLoad = loading[uid];
		LOADING_TYPE(mpLoad) = DB_TYPE_USER;
		LOADING_SYNC(mpLoad) = issync;
		LOADING_CALLBACK(mpLoad) = ({});
		LOADING_SETUP_UID(mpLoad) = load_id;
		LOADING_IS_VUSER(mpLoad) = is_vuser;
	} else {
		mpLoad = loading[uid];
		//已经在loading了,那么上一次load肯定是异步的，更新必要信息
		if( issync ) {
			LOADING_SYNC(mpLoad) = issync;
		}
	}

	if (on_load_cb) {
		LOADING_CALLBACK(mpLoad) += ({ ([ "function": on_load_cb, "arg": arg ]) });
	}

	if (issync || !iExist) {
		loading_sid++;
		LOADING_SESSION_ID(mpLoad) = loading_sid;
		logger->Log("load user.uid=%d,sync=%d,sessionid=%d", uid, issync, loading_sid);
		return fs_db_object_load(uid, issync, loading_sid);
	}
	return 0;
}

private int load_user(int uid, int issync, function on_load_cb, mixed arg)
{
	return _load_user(uid, issync, uid, 0, on_load_cb, arg);
}

private int load_vuser(int uid, int issync, function on_load_cb, mixed arg)
{
	return _load_user(uid, issync, uid, 1, on_load_cb, arg);
}

// 暂时只支持同步的
private int simulate_load_user(int uid, int load_uid)
{
	if (load_uid <= 0) return 0;
	if (isLoading(uid)) return 0;
	return _load_user(uid, LOAD_SYNC, load_uid, 0, 0, 0);
}

private int load_dat(object obj, int issync, function on_load_cb, mixed arg)
{
	string key = obj->getSaveName();

	if (loading[key]) {
		logger->Log("warn try to load loading object.key=%s", key);
		return -1;
	}

	if (saving[key]) {
		logger->Log("warn try to load saving object.key=%s", key);
		return -1;
	}

	mapping mpLoad = ([]);

	loading_sid++;

	LOADING_TYPE(mpLoad) = DB_TYPE_DAT;
	LOADING_SYNC(mpLoad) = issync;
	LOADING_CALLBACK(mpLoad) = ({ (["function": on_load_cb, "arg": arg]) });
	LOADING_SESSION_ID(mpLoad) = loading_sid;
	LOADING_OBJECT(mpLoad) = obj;

	loading[key] = mpLoad;

	logger->Log("load dat.key=%s,sync=%d,sessionid=%d", key, issync, loading_sid);
	fs_db_object_load(key, issync, loading_sid);
	return 0;
}

//新建一个存盘的对象，并进行一次存盘,然后就会定时存盘.
int new_dat(object obj)
{
	//new can not be dirty, may be save first then enable dirty
	string key = obj->getSaveName();
	//todo sync and async may be conflict here
	if (loading[key]) {
		logger->Log("warn try to load loading object.key=%s", key);
		return -1;
	}

	if (saving[key]) {
		logger->Log("warn try to load saving object.key=%s", key);
		return -1;
	}

	saving[key] = obj;
	fs_db_object_new(key, obj);
	enable_dirty_object(obj);
	logger->Log("new dat.key=%s", key);

	return 0;
}

//进行一次存盘，然后取消定时存盘
varargs int unload_dat(object obj, int forcesync)
{
	//todo ? check and remove the loading
	string key = obj->getSaveName();
	if (saving[key]) {
		//todo assert obj
		map_delete(saving, key);
		fs_db_object_save(key, obj, SAVE_METHOD); 
		fs_db_object_unload(key);
		if (forcesync) {
			logger->Log("unload dat with force sync.key=%s", key);
			//强制全局同步一次数据并把脏数据写磁盘，
			//因为dbd只有把数据同步到磁盘的时候才会释放掉内存中的object
			save_all(forcesync);
		} else {
			logger->Log("unload dat.key=%s", key);
		}
		return 0;
	}
	return -1;
}

//同步加载一个存盘对象,加载完毕后就会进行定时存盘
int load_dat_sync(object obj)
{
	return load_dat(obj, LOAD_SYNC, 0, 0);
}

//异步加载一个存盘对象, 加载完毕后就会进行定时存盘
int load_dat_async(object obj, function on_load_cb, mixed arg)
{
	return load_dat(obj, LOAD_ASYNC, on_load_cb, arg);
}

//同步加载一个存盘对象,加载完毕后就会进行定时存盘
int load_user_sync(int uid)
{
	return load_user(uid, LOAD_SYNC, 0, 0);
}

//异步加载一个存盘对象, 加载完毕后就会进行定时存盘
int load_user_async(int uid, function on_load_cb, mixed arg)
{
	return load_user(uid, LOAD_ASYNC, on_load_cb, arg);
}

//让db新建一个存盘的玩家对象，并进行一次存盘,然后就会定时存盘.
int new_user(object user)
{
	int uid = user->getId();
	object realuser = get_user(uid);
	if (objectp(realuser)) {
		fs_db_object_new(uid, realuser);
		enable_dirty_object(realuser);
		logger->Log("new user uid=%d", uid);
		return 0;
	}
	return -1;
}

//进行一次存盘，然后取消定时存盘
int unload_user(object user)
{
	//todo check and remove the loading ?
	int uid = user->getId();
	fs_db_object_save(uid, user, SAVE_METHOD);
	fs_db_object_unload(uid);
	logger->Log("unload user.uid=%d", uid);
	return 0;

}

//存盘
int save_user(object user)
{
	int uid = user->getId();
	object realuser = get_user(uid);
	if (objectp(realuser)) {
		fs_db_object_save(uid, realuser, SAVE_METHOD);
		logger->Log("saving user.uid=%d", uid);
		return 0;
	}
	return -1;
}

//存盘
int save_user_all(object user)
{
	int uid = user->getId();
	object realuser = get_user(uid);
	if (objectp(realuser)) {
		fs_db_object_save(uid, realuser, SAVE_ALL);
		logger->Log("saving user all.uid=%d", uid);
		return 0;
	}
	return -1;
}

int save_user_dirty(object user)
{
	int uid = user->getId();
	object realuser = get_user(uid);
	if (objectp(realuser)) {
		fs_db_object_save(uid, realuser, SAVE_DIRTY);
		logger->Log("saving user dirty.uid=%d", uid);
		return 0;
	}
	return -1;
}

//存盘
int save_dat(object  obj)
{
	string key = obj->getSaveName();
	if (saving[key]) {
		fs_db_object_save(key, obj, SAVE_METHOD);
		logger->Log("saving dat.key=%s", key);
		return 0;
	}
	return -1;
}

//全存盘
int save_dat_all(object  obj)
{
	string key = obj->getSaveName();
	if (saving[key]) {
		fs_db_object_save(key, obj, SAVE_ALL);
		logger->Log("saving dat all.key=%s", key);
		return 0;
	}
	return -1;
}

//脏数据存盘
int save_dat_dirty(object  obj)
{
	string key = obj->getSaveName();
	if (saving[key]) {
		fs_db_object_save(key, obj, SAVE_DIRTY);
		logger->Log("saving dat dirty.key=%s", key);
		return 0;
	}
	return -1;
}

void save_all_dat()
{
	foreach (string key, object obj in saving) {
		fs_db_object_save(key, obj, SAVE_METHOD);
		//fs_db_object_save(key, obj, SAVE_ALL);
		//logger->Log("flush dat.key=%s", key);
		//debug_message(sprintf("flush dat.key=%s", key));
	}
}

int rename(object obj, string newsavename)
{
	return fs_db_object_rename(obj->getSaveName(), obj, newsavename);
}

//把db进程的存盘数据写到磁盘上
void sync_db_to_disk()
{
	logger->Log("sync db to disk");
	fs_db_sync(DB_SYNC_SINGLE_THREAD);
}

void sync_db_to_disk_thr_w()
{
	logger->Log("multi thread write sync db to disk");
	fs_db_sync(DB_SYNC_MULTI_W_THREAD);
}

void sync_db_to_disk_thr_rw()
{
	logger->Log("multi thread rw sync db to disk");
	fs_db_sync(DB_SYNC_MULTI_RW_THREAD);
}

varargs void save_all(int forcesync)
{
	logger->Log("save all force=%d", forcesync);
	fs_save_all_user(SAVE_METHOD);
	fs_save_all_dat(saving, SAVE_METHOD);
	save_count++;
	if ((save_count % SAVE_SYNC_NUM) == 0 || forcesync) {
		//sync_db_to_disk();
		sync_db_to_disk_thr_rw();
		/*
		if (GetHostNum() == 1024 ||IsInternalHost()) {
			sync_db_to_disk_thr_rw();
		} else {
			sync_db_to_disk_thr_w();
		}
		*/
	}
	//fs_save_all_dat(saving, SAVE_ALL);
	//save_all_dat();
}


buffer marshal_dirty_map(mapping map)
{
	return fs_marshal_dirty_map(map);
}

buffer marshal_dirty_object(object obj)
{
	return fs_marshal_dirty_object(obj);
}

//merge to db mapping
void merge_dirty_object(mapping m, buffer dirty)
{
	fs_merge_dirty_object(m, dirty);
}


//把存盘的脏数据发给db
void take_snapshot()
{
        call_out( (:take_snapshot:), 60);
	//fs_dirty_mem_pool_stat();
	save_all();
	lpc_gc(0);
	//fs_dirty_mem_pool_stat();
}	

void start()
{
	call_out("take_snapshot", 60);
	//Import("TIME_CTRL")->crontab(60, "take_snapshot");
}

void create()
{
	logger = Import("LOG")->New("save");
	call_out("start", 3);
}

// 异步操作玩家数据
mixed userAsyncOp(int uid, function operation, mixed *arg...)
{
	mixed res;
	object oUser = get_user(uid);
	if (oUser) {
		res = evaluate(operation, oUser, arg...);	
	} else {
		res = load_user_async(uid, operation, arg);
	}
	return res;
}

