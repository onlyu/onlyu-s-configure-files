
#include "fs_config.h"
#include "fs_comm.h"
#include "fs_ipc.h"
#include "fs_dbi.h"
#include "fs_marshal.h"
#include "fs_data_save.h"
#include "fs_db_format.h"
//#include "fs_mysql.h"
#include "fs_rpc_lpc.h"
#include "lpc_args_op.h"

#include "lpc.h"
#include "lpc_incl.h"

#include "mkdir.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <libgen.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <sys/sysctl.h>

#include "fs_log.h"

#define PATH_BUF_LEN 1024

#define DIR_MODE (S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH)

//for 64-bits machine
// total max buf size: 512k * 20k = 1024M = 1G
static size_t game_max_outbuf = (1.8 * 1024 * 1024 * 1024);
static char *dbd_ip = "127.0.0.1";
static unsigned short dbd_port = 60010;
static const int backlog = 15;

static fs_net_acceptor_t *game_acceptor = NULL;
fs_net_connection_t *game_conn = NULL;
fs_net_connection_t *game_conn_sync = NULL;
static int is_service_open = 0;

static fs_rpc_lpc_t *dbd_rpc_lpc = NULL;

static fs_net_mbuf_t *gamed_netbuf = NULL;

static fs_apr_hash_t dbo_base = {NULL, NULL};

/*多个异步事件的同步系列化*/
#define SEVENT_NULL 		0
#define SEVENT_SYNC_SAVE	0x00000001  /*同步存盘*/
#define SEVENT_SIG_SAVE 	0x00000002  /*接收到存盘信号*/
#define SEVENT_SIG_EXIT 	0x00000004  /*接收到退出信号*/
#define SEVENT_CONN_ERR 	0x00000008  /*socket连接错误*/
#define SEVENT_GAMED_EXIT 	0x00000010  /*gamed已经退出*/

//从左到右处理
static unsigned int serialized_seq[] = {SEVENT_SYNC_SAVE, SEVENT_SIG_SAVE, SEVENT_SIG_EXIT, SEVENT_CONN_ERR, SEVENT_GAMED_EXIT};
static unsigned int serialized_event = SEVENT_NULL;

#define DBD_CALLOUT_SEC (0)
#define DBD_CALLOUT_USEC (300000)


#define LOG_DEBUG(what, key, fmt, args...)  DB_KEY_LOG("debug "what, key, fmt, ##args)
#define LOG_WARN(what, key, fmt, args...)  DB_KEY_LOG("warn "what, key, fmt, ##args)
#define LOG_ERROR(what, key, fmt, args...)  DB_KEY_LOG("error "what, key, fmt, ##args)


typedef enum {
	FS_DB_STAT_ONLINE = 0,
	FS_DB_STAT_OFFLINE,
	FS_DB_STAT_UNDEFINED,
} fs_db_stat_t;


typedef struct fs_db_user_s {
	db_key_t key;
	fs_db_stat_t stat;
	int dirty;
	char *pathname;
	char *dir;
	char *tmppathname;
	char *newpathname;
	char *newdir;
	svalue_t map;
} fs_db_object_t;

typedef struct fs_dirty_statics_s {
	unsigned count;
	size_t size;
} fs_dirty_statics_t;

fs_dirty_statics_t dirty_statics = {0, 0};

void dirty_statics_reset(fs_dirty_statics_t *st)
{
	st->count = 0;
	st->size = 0;
}


void restore_dbd_proctitle()
{
	setproctitle("-dbd_%s", fs_config_get_string(ENGINE_NAME));
}

#define DAT_PATTERN "dat/%s.py"

static void get_pathname(db_key_t *key, char *out, size_t size)
{
 	memset(out, 0, size);
	if (DB_KEY_IS_STR(key)) {
	       snprintf(out, size, DAT_PATTERN, DB_KEY_STR(key));
	} else {
	       fs_uid_t uid = DB_KEY_INT(key);
	       snprintf(out, size, "user/%02d/u%d.py", (uid%100), uid);
	}
}

static void get_inactive_pathname(db_key_t *key, char *out, size_t size)
{
	memset(out, 0, size);
	if (DB_KEY_IS_STR(key)) {
	       snprintf(out, size, "inactive_dat/%s.py", DB_KEY_STR(key));
	} else {
	       fs_uid_t uid = DB_KEY_INT(key);
	       snprintf(out, size, "inactive_user/%02d/u%d.py", (uid%100), uid);
	}
}

#define MAKE_TMPPATH(buf, len, path) do {\
	memset((buf), 0, (len));	\
	snprintf((buf), (len), "%s.%s", (path), "tmp");\
} while (0)

static void setup_pathname(fs_db_object_t *dbo)
{
	char pathname[PATH_BUF_LEN];
	char tmppathname[PATH_BUF_LEN];

	db_key_t *key = &dbo->key;
	memset(pathname, 0, sizeof(pathname));
	get_pathname(key, pathname, sizeof(pathname));
	MAKE_TMPPATH(tmppathname, sizeof(tmppathname), pathname);

	dbo->pathname = strdup(pathname);
	dbo->dir = strdup(dirname(pathname));
	dbo->tmppathname = strdup(tmppathname);
	dbo->newpathname = NULL;
	dbo->newdir = NULL;
}

static fs_db_object_t *fs_db_object_new(db_key_t *ref)
{
	fs_db_object_t *dbo = calloc(1, sizeof(fs_db_object_t));
	mapping_t *map = allocate_mapping(1);

	dbo->stat = FS_DB_STAT_UNDEFINED;
	dbo->dirty = 0;
	set_svalue_mapping(&dbo->map, map);

	db_key_init(&dbo->key, ref->type, ref->value, ref->len);
	setup_pathname(dbo);
	return dbo;
}

void fs_db_object_free(fs_db_object_t *dbo)
{
	db_key_free(&dbo->key);
	free_svalue(&dbo->map, "db dat free");
	free(dbo->pathname);
	free(dbo->tmppathname);
	free(dbo->dir);
	if (dbo->newpathname) {
		free(dbo->newpathname);
	}
	if (dbo->newdir) {
		free(dbo->newdir);
	}
	free(dbo);
}



inline int static is_exist(char *pathname)
{
	struct stat sb;
	return stat(pathname, &sb) == 0;
}


static int unformat(svalue_t *map, char *str, size_t len, char *pathname)
{
	svalue_t tmp = const0u;

	if (fs_db_unformat(pathname, str, len, &tmp)) {
		fprintf(stderr, "unformat error!\n");
		return -1;
	}

	if (tmp.type != T_MAPPING) {
		free_svalue(&tmp, "unformat");
		return -1;
	}

	assign_svalue(map, &tmp);
	free_svalue(&tmp, "unformat");

	return 0;
}

static int raw_save_to_file_thr_safe(const char *pathname,  char *tmppathname, char *dir, unsigned char *data, size_t len)
{
	int fd;
	int ret;
	//char *dir = dirname(tmppathname);
	//mode_t mode;

	//mode = umask(0);

	if (dir == NULL) {
		//fprintf(stderr, "error save null dir pathname=%s\n", pathname);
		log_error_thr_safe(MOD_DB, "error save null dir pathname=%s", pathname);
		goto error;
	}

	if (!is_exist(dir)) {
		if (0 == build(dir, DIR_MODE)) {
			//fprintf(stderr, "error save mkdir pathname=%s,dir=%s\n", pathname, dir);
			log_error_thr_safe(MOD_DB, "error save mkdir pathname=%s,dir=%s", pathname, dir);
			goto error;
		}
	}

	fd = open(tmppathname, O_WRONLY|O_TRUNC|O_CREAT, FILE_MODE);
	if (fd < 0) {
		//fprintf(stderr, "error save open pathname=%s,strerror=%s\n", pathname, strerror(errno));
		log_error_thr_safe(MOD_DB, "error save open pathname=%s,strerror=%s", pathname, strerror(threrrno));
		goto error;
	}

	//write & unlink 成功才是成功
	ret = write(fd, data, len);
	if (ret < 0) {
		//fprintf(stderr, "error save pathname=%s,strerror=%s\n", pathname, strerror(errno));
		log_error_thr_safe(MOD_DB, "error save pathname=%s,strerror=%s", pathname, strerror(threrrno));
		if (unlink(tmppathname) < 0) {
			//fprintf(stderr, "error unlink pathname=%s,strerror=%s\n", pathname, strerror(errno));
			log_error_thr_safe(MOD_DB, "error unlink pathname=%s,strerror=%s", pathname, strerror(threrrno));
		}
	} else {
		ret = rename(tmppathname, pathname);
		if (ret < 0) {
			//fprintf(stderr, "error save rename pathname=%s,strerror=%s\n", pathname, strerror(errno));
			log_error_thr_safe(MOD_DB, "error save rename pathname=%s,strerror=%s", pathname, strerror(threrrno));
		}
	}

	close(fd);
	//umask(mode);
	return ret < 0 ? -1 : 0;

error:
	//umask(mode);
	return -1;
}

static int save_to_file_thr_safe(const char *pathname,  char *tmppathname, char *dir, svalue_t *svmap)
{
	struct evbuffer *data_buf = evbuffer_new();

	if (fs_db_format_thr_safe(svmap, data_buf)) {
		//fprintf(stderr, "error object format pathname=%s\n", pathname);
		log_error_thr_safe(MOD_DB, "error object format pathname=%s", pathname);
		goto error;
	}

	if (raw_save_to_file_thr_safe(pathname, tmppathname, dir, EVBUFFER_DATA(data_buf), EVBUFFER_LENGTH(data_buf))) {
		//fprintf(stderr, "error object save pathname=%s\n", pathname);
		log_error_thr_safe(MOD_DB, "error object save pathname=%s", pathname);
		goto error;
	}	

	evbuffer_free(data_buf);
	return 0;

error:
	evbuffer_free(data_buf);
	return -1;
}

int restore_from_file(char *pathname, svalue_t *svmap)
{
	size_t size;
	char *buf;
	int fd;
	struct stat sb;

	log_info(MOD_DB, "try restore file pathname=%s", pathname);

	fd = open(pathname, O_RDONLY, 0);
	if (fd < 0) {
		fprintf(stderr, "Warning:restore open pathname=%s,errmsg=%s\n", pathname, strerror(errno));
		return -1;
	}

	if (fstat(fd, &sb)) {
		close(fd);
		fprintf(stderr, "Warning:restore file pathname=%s,errmsg=%s\n", pathname, strerror(errno));
		return -1;
	}

	size = sb.st_size;
	buf = calloc(1, size + 1);
	read(fd, buf, size);
	close(fd);
	buf[size] = '\0';

	if (unformat(svmap, buf, size, pathname)) { 
		free(buf);
		fprintf(stderr, "Warning:restore unformat pathname=%s\n", pathname);
		return -1;
	}

	free(buf);
	return 0;
}


#define DBO_STAT_NOT_EXIST 0
#define DBO_STAT_ACTIVE 1
#define DBO_STAT_INACTIVE 2

#define IS_DBO_STAT_EXIST(stat) ((stat) == DBO_STAT_ACTIVE || (stat) == DBO_STAT_INACTIVE)


static int db_object_file_stat(db_key_t *key)
{
	char pathname[PATH_BUF_LEN];
	get_pathname(key, pathname, sizeof(pathname));
	if (is_exist(pathname)) {
		return DBO_STAT_ACTIVE;
	} else {
		get_inactive_pathname(key, pathname, sizeof(pathname));
		if (is_exist(pathname)) {
			return DBO_STAT_INACTIVE;
		} else {
			return DBO_STAT_NOT_EXIST;
		}
	}
}

static int has_db_object_file(db_key_t *key)
{
	int ret = db_object_file_stat(key);
	return IS_DBO_STAT_EXIST(ret);
}

static int db_object_save_thr_safe(fs_db_object_t *dbo)
{
	int ret = save_to_file_thr_safe(dbo->pathname, dbo->tmppathname, dbo->dir, &dbo->map);
	if (ret == 0) {
		if (dbo->newpathname) {
			if (dbo->newdir == NULL) {
				log_error_thr_safe(MOD_DB, "error rename save null dir pathname=%s,newpathname=%d", 
						dbo->pathname, dbo->newpathname);
				return -1;
			}

			if (!is_exist(dbo->newdir)) {
				if (0 == build(dbo->newdir, DIR_MODE)) {
					log_error_thr_safe(MOD_DB, "error rename save mkdir pathname=%s,newpathname=%d,dir=%s", 
						dbo->pathname, dbo->newpathname, dbo->dir);
					return -1;
				}
			}

			ret = rename(dbo->pathname, dbo->newpathname);
			if (ret == 0) {
				log_info_thr_safe(MOD_DB, "rename pathname=%s,newpathname=%s", 
					dbo->pathname, dbo->newpathname);
			} else {
				log_error_thr_safe(MOD_DB, "error rename pathname=%s,newpathname=%s,strerror=%s", 
					dbo->pathname, dbo->newpathname, strerror(threrrno));
			}
		}
	}

	return ret;
}

static int db_object_restore(fs_db_object_t *dbo, int dbostat)
{
	if (dbostat == DBO_STAT_ACTIVE) {
		if (restore_from_file(dbo->pathname, &dbo->map)) {
			fprintf(stderr, "error db object restore active pathname=%s\n", dbo->pathname);
			return -1;
		}	
		return 0;
	} else if (dbostat == DBO_STAT_INACTIVE) {
		char pathname[PATH_BUF_LEN];
		get_inactive_pathname(&dbo->key, pathname, sizeof(pathname));
		if (restore_from_file(pathname, &dbo->map)) {
			fprintf(stderr, "error db object restore inactive pathname=%s\n", pathname);
			return -1;
		}	
		return 0;
	}

	return -1;
}

// string
void f_fs_restore_from_file(void)
{
	int ret;
	SET_ALL_ARGS;
	svalue_t * pathname = ARGS(0);
	svalue_t svmap;
	mapping_t *map = allocate_mapping(0);
	set_svalue_mapping(&svmap, map);
	ret = restore_from_file(pathname->u.string, &svmap);
	POP_ALL_ARGS;
	if (ret == 0) {
		push_svalue(&svmap);
	} else {
		push_undefined();
	}
	free_svalue(&svmap, "restore");
}

// string, mapping
void f_fs_save_to_file(void)
{
	int ret;
	SET_ALL_ARGS;
	svalue_t * pathname = ARGS(0);
	svalue_t * svmap = ARGS(1);
	char tmppathname[PATH_BUF_LEN];
	MAKE_TMPPATH(tmppathname, sizeof(tmppathname), pathname->u.string);
	ret = save_to_file_thr_safe(pathname->u.string, tmppathname, dirname(pathname->u.string), svmap);
	POP_ALL_ARGS;
	push_number(ret);
}


static fs_db_object_t *dbo_new(db_key_t *key)
{
	fs_db_object_t *dbo = NULL;

	if (has_db_object_file(key)) {
		LOG_WARN("new replace the old file", key, "");
	}
	dbo = fs_db_object_new(key);
	dbo->stat = FS_DB_STAT_ONLINE;
	dbo->dirty = 1;
	apr_hash_set(dbo_base.hash, dbo->key.value, dbo->key.len, dbo);
	return dbo;
}


static void game_input_dbo_new(fs_net_connection_t *conn, fs_gd_header_t *header, void *buf, size_t len)
{
	// need to the the last data from game?
	fs_dbi_array_iter_t iter;
	db_key_t key;

	FS_DBI_ARRAY_ITER_FIRST(&iter, buf, len);
	db_key_ref(header, &iter, &key);

	//fprintf(stderr, "dat new:%s\n", save_name);
	LOG_DEBUG("new cmd", &key, "");
	fs_db_object_t *dbo = apr_hash_get(dbo_base.hash, key.value, key.len);
	if (dbo == NULL) {
		dbo = dbo_new(&key);
		if (db_map_unmarshal(&dbo->map, &iter)) {
			//todo error log
			//fprintf(stderr, "dat new error. save name:%s\n", save_name);
			assert(0);
		}
		LOG_DEBUG("new ok", &key, "");
	} else {
		//fprintf(stderr, "warnning duplicated dat new:%s\n", save_name);
		LOG_WARN("skip new duplicated", &key, "");
	}
}

static void game_input_dbo_load(fs_net_connection_t *conn, fs_gd_header_t *header, void *buf, size_t len, int sync)
{
	fs_dbi_array_iter_t iter;
	db_key_t key;
	fs_gd_header_t hdr;
	int sid;
	memset(&hdr, 0, sizeof(hdr));

	FS_DBI_ARRAY_ITER_FIRST(&iter, buf, len);
	db_key_ref(header, &iter, &key);
	sid = iter.tv->number.n;
	FS_DBI_ARRAY_ITER_NEXT(&iter);

	hdr.cmd = FS_GD_CMD_DB_OBJ_DATA;
	hdr.uid = header->uid;
	hdr.flag = header->flag;

	LOG_DEBUG("load cmd", &key, "sync=%d", sync);
	fs_db_object_t *dbo = apr_hash_get(dbo_base.hash, key.value, key.len);
	if (dbo == NULL) { 
		int dbostat = db_object_file_stat(&key);
		if (IS_DBO_STAT_EXIST(dbostat)) {
			dbo = fs_db_object_new(&key);
			if (db_object_restore(dbo, dbostat)) {
				fs_db_object_free(dbo);
				hdr.stat = FS_GD_STAT_ERROR;
				db_map_marshal_to(conn, &hdr, &key, NULL, sid);
				LOG_ERROR("load fail", &key, "sync=%d", sync);
				return;
			}
			if (dbo->map.type == T_MAPPING) {
				hdr.stat = FS_GD_STAT_OK;
				db_map_marshal_to(conn, &hdr, &key, &dbo->map, sid);
				apr_hash_set(dbo_base.hash, dbo->key.value, dbo->key.len, dbo);
				dbo->dirty = 0;
				LOG_DEBUG("load ok", &key, "sync=%d", sync);
			} else {
				fs_db_object_free(dbo);
				hdr.stat = FS_GD_STAT_ERROR;
				db_map_marshal_to(conn, &hdr, &key, NULL, sid);
				LOG_ERROR("load error", &key, "sync=%d", sync);
			}
		} else	{
			hdr.stat = FS_GD_STAT_NULL;
			db_map_marshal_to(conn, &hdr, &key, NULL, sid);
			LOG_WARN("load null", &key, "sync=%d", sync);
		}
	} else { //has db object cache
		dbo->stat = FS_DB_STAT_ONLINE;
		if (dbo->map.type == T_MAPPING) {
			hdr.stat = FS_GD_STAT_OK;
			db_map_marshal_to(conn, &hdr, &key, &dbo->map, sid);
			LOG_DEBUG("load cache ok", &key, "sync=%d", sync);
		} else {
			hdr.stat = FS_GD_STAT_ERROR;
			db_map_marshal_to(conn, &hdr, &key, NULL, sid);
			LOG_ERROR("load cache error", &key, "sync=%d", sync);
		}
	} //else
}


static void game_input_dbo_unload(fs_net_connection_t *conn, fs_gd_header_t *header, void *buf, size_t len)
{
	fs_dbi_array_iter_t iter;
	db_key_t key;

	FS_DBI_ARRAY_ITER_FIRST(&iter, buf, len);
	db_key_ref(header, &iter, &key);

	LOG_DEBUG("unload cmd", &key, "");
	fs_db_object_t *dbo = apr_hash_get(dbo_base.hash, key.value, key.len);
	if (dbo != NULL) { 
		//db_user_dirty_recv(&user->map, buf, len);
		/*
		if (db_map_unmarshal(&dbo->map, &iter)) {
			//todo log
			return; 
		}
		dbo->dirty = 1;
		*/
		dbo->stat = FS_DB_STAT_OFFLINE;
		LOG_DEBUG("unload ok", &key, "");
	} else {
		LOG_WARN("unload miss", &key, "");
	}
}


static void game_input_dbo_all_data(fs_net_connection_t *conn, fs_gd_header_t *header, void *buf, size_t len)
{
	fs_dbi_array_iter_t iter;
	db_key_t key;

	FS_DBI_ARRAY_ITER_FIRST(&iter, buf, len);
	db_key_ref(header, &iter, &key);

	LOG_DEBUG("data cmd", &key, "");
	fs_db_object_t *dbo = apr_hash_get(dbo_base.hash, key.value, key.len);
	if (dbo != NULL) { 
		dbo->dirty = 1;
		//db_user_dirty_recv(&user->map, buf, len);
	} else {
		LOG_WARN("data miss and just new it", &key, "");
		dbo = dbo_new(&key);
	}

	if (db_map_unmarshal(&dbo->map, &iter)) {
		LOG_ERROR("data unmarshal fail", &key, "");
		return;
	}
	LOG_DEBUG("data ok", &key, "");
}

static void game_input_dbo_dirty_data(fs_net_connection_t *conn, fs_gd_header_t *header, void *buf, size_t len)
{
	fs_dbi_array_iter_t iter;
	db_key_t key;

	FS_DBI_ARRAY_ITER_FIRST(&iter, buf, len);
	db_key_ref(header, &iter, &key);

	//LOG_DEBUG("dirty cmd", &key, "");
	fs_db_object_t *dbo = apr_hash_get(dbo_base.hash, key.value, key.len);
	if (dbo != NULL) { 
		dbo->dirty = 1;
		merge_dirty_db_object_to_map(&dbo->map, &iter);
		//LOG_DEBUG("dirty ok", &key, "");
	} else {
		LOG_ERROR("dirty miss.no such object", &key, "");
		//这里不能new一个dbo，否则merge脏数据的时候会出错，因为new出来的dbo是一个空数据。
		//dbo = dbo_new(&key);
	}
}

//newpathname 要和 pathname同一个目录
static void game_input_dbo_rename(fs_net_connection_t *conn, fs_gd_header_t *header, void *buf, size_t len)
{
	fs_dbi_array_iter_t iter;
	db_key_t key;

	FS_DBI_ARRAY_ITER_FIRST(&iter, buf, len);
	db_key_ref(header, &iter, &key);

	LOG_DEBUG("rename cmd", &key, "");
	fs_db_object_t *dbo = apr_hash_get(dbo_base.hash, key.value, key.len);
	if (dbo != NULL) { 
		char *newdir;
		char newpathname[PATH_BUF_LEN];
		memset(newpathname, 0, sizeof(newpathname));
		snprintf(newpathname, sizeof(newpathname), DAT_PATTERN, iter.tv->string.str);
		//重名判断不一定管用，因为有被rename的dbobject还在内存里没有刷到磁盘上。
		if (is_exist(newpathname)) {
			LOG_ERROR("rename newfile already exists", &key, "oldpathname=%s,newpathname=%s", dbo->pathname, newpathname);
			return;
		} 
		if (strcmp(newpathname, dbo->pathname) == 0) {
			LOG_WARN("rename skip", &key, "oldpathname=%s,newpathname=%s", dbo->pathname, newpathname);
			return;
		}
		newdir = dirname(newpathname);
		if (newdir == NULL) {
			LOG_ERROR("rename newdir error", &key, "oldpathname=%s,newpathname=%s", dbo->pathname, newpathname);
			return;
		}

		dbo->dirty = 1;
		LOG_DEBUG("try rename", &key, "oldpathname=%s,newpathname=%s", dbo->pathname, newpathname);
		if (dbo->newpathname) {
			free(dbo->newpathname);
		}
		dbo->newpathname = strdup(newpathname);
		if (dbo->newdir) {
			free(dbo->newdir);
		}
		dbo->newdir = strdup(newdir);
	} else {
		LOG_WARN("rename miss", &key, "");
	}
}

static void free_offline()
{
	apr_hash_index_t *hi;
	void *hdbo;
	fs_db_object_t *dbo;
	for (hi = apr_hash_first(NULL, dbo_base.hash); hi; hi = apr_hash_next(hi)) {
		apr_hash_this(hi, NULL, NULL, &hdbo);
		dbo = hdbo;
		// can save remove me in the iteration?
		if (dbo->stat == FS_DB_STAT_OFFLINE && dbo->dirty == 0) {
			apr_hash_set(dbo_base.hash, dbo->key.value, dbo->key.len, NULL);
			fs_db_object_free(dbo);
			LOG_DEBUG("free", &dbo->key, "");
		}
	}

	restore_dbd_proctitle();
}

static void flush_all_db_object()
{
	//sync all data to storage
	apr_hash_index_t *hi;
	void *hdbo;
	fs_db_object_t *dbo;

	setproctitle("-dbd %s", "flushing dirty");
	log_info(MOD_DB, "sync dirty to disk begin");

	// not thread safe for the iterator of apr_hash unless you porvide the apr_pool for allocating iterator
	for (hi = apr_hash_first(NULL, dbo_base.hash); hi; hi = apr_hash_next(hi)) {
		apr_hash_this(hi, NULL, NULL, &hdbo);
		dbo = hdbo;
		if (dbo->dirty) {
			if (db_object_save_thr_safe(dbo)) { //存盘错误
				continue;
			}
			dbo->dirty = 0;
			//LOG_DEBUG("flush", &dbo->key, "");
		}
		/*
		// can save remove me in the iteration?
		if (dbo->stat == FS_DB_STAT_OFFLINE) {
			apr_hash_set(dbo_base.hash, dbo->key.value, dbo->key.len, NULL);
			fs_db_object_free(dbo);
			LOG_DEBUG("free", &dbo->key, "");
		}
		*/
	}

	free_offline();
	log_info(MOD_DB, "sync dirty to disk end");
}

typedef struct {
	fs_db_object_t ** dirtylist;
	int start;
	int end;
	int threadcnt;
} thr_arg_t;

static void * thread_main(void * param)
{
	int i;
	fs_db_object_t *dbo;
	thr_arg_t *arg = param;

	for (i = arg->start; i < arg->end; i += arg->threadcnt) {
		dbo = arg->dirtylist[i];
		if (0 == db_object_save_thr_safe(dbo)) {
			dbo->dirty = 0;
			//log_info_thr_safe(MOD_DB, "%dth thread save object pathname=%s", arg->start, dbo->pathname);
		}
	}

	return NULL;
}

static int query_hw_ncpu()
{
        int mib[2];
	int ncpu = 0;
        size_t len;

        mib[0] = CTL_HW;
        mib[1] = HW_NCPU;
        len = sizeof(ncpu);
        if (sysctl(mib, sizeof(mib)/sizeof(mib[0]), &ncpu, &len, NULL, 0)) {
                perror("sysctl error");
		return -1;
        }

        return ncpu;
}

static void thread_flush_all_db_object()
{
	//sync all data to storage
	apr_hash_index_t *hi;
	void *hdbo;
	fs_db_object_t *dbo;

	int size = apr_hash_count(dbo_base.hash);
	fs_db_object_t **dirtylist = calloc(size, sizeof(fs_db_object_t*));
	int index = 0, threadcnt = 0;

	setproctitle("-dbd %s", "multi-thread flushing dirty");
	log_info(MOD_DB, "multi-thread sync dirty to disk begin");

	// not thread safe for the iterator of apr_hash unless you porvide the apr_pool for allocating iterator
	for (index = 0, hi = apr_hash_first(NULL, dbo_base.hash); hi; hi = apr_hash_next(hi)) {
		apr_hash_this(hi, NULL, NULL, &hdbo);
		dbo = hdbo;
		if (dbo->dirty) {
			dirtylist[index++] = dbo;
		}
	}

	if (index > 0) {
		threadcnt = query_hw_ncpu();
		if (threadcnt <= 0) {
			threadcnt = 4;
		}
		threadcnt = index > threadcnt ? threadcnt : index;

		pthread_t thread[threadcnt];
		thr_arg_t arg[threadcnt];
		int i;

		for (i = 0; i < threadcnt; i++) {
			arg[i].dirtylist = dirtylist;
			arg[i].start = i;
			arg[i].end = index;
			arg[i].threadcnt = threadcnt;
			if (pthread_create(&thread[i], NULL, thread_main, &arg[i])) {
				//todo fixme: will skip saving of dirty object
				log_error_thr_safe(MOD_DB, "error create thread index=%d,errno=%d", i, errno);
			} 
		}
		for (i = 0; i < threadcnt; i++) {
			if (pthread_join(thread[i], NULL)) {
				log_error_thr_safe(MOD_DB, "error join thread index=%d,errno=%d", i, errno);
			}
			log_info_thr_safe(MOD_DB, "join thread ok index=%d", i);
		}
	}

	for (hi = apr_hash_first(NULL, dbo_base.hash); hi; hi = apr_hash_next(hi)) {
		apr_hash_this(hi, NULL, NULL, &hdbo);
		dbo = hdbo;
		// can save remove me in the iteration?
		if (dbo->stat == FS_DB_STAT_OFFLINE && dbo->dirty == 0) {
			apr_hash_set(dbo_base.hash, dbo->key.value, dbo->key.len, NULL);
			fs_db_object_free(dbo);
			LOG_DEBUG("free", &dbo->key, "");
		}
	}

	free(dirtylist);
	restore_dbd_proctitle();
	log_info(MOD_DB, "multi-thread sync dirty to disk end.threadcnt=%d", threadcnt);
}

/*
*
*/
static pthread_rwlock_t global_lpc_rwlock =  PTHREAD_RWLOCK_INITIALIZER;
#define GLOBAL_RLOCK_REQUIRE { int ret = pthread_rwlock_rdlock(&global_lpc_rwlock); assert(ret == 0);}
#define GLOBAL_WLOCK_REQUIRE { int ret = pthread_rwlock_wrlock(&global_lpc_rwlock); assert(ret == 0);}
#define GLOBAL_LOCK_RELEASE { int ret = pthread_rwlock_unlock(&global_lpc_rwlock); assert(ret == 0);}

typedef struct {
	pthread_t pthid;
	int index;
} fs_worker_t;

typedef struct {
	fs_db_object_t ** dirtylist;
	size_t dirtycnt;
	fs_worker_t *workerlist;
	int workercnt;
	int doscnt;
	int isworking;
	int dosfds[2];
} fs_blackboard_t;

int add_timer(struct timeval period, void (*callback)(void *), void * arg);
static const char dos_fin_work = 'F';
static fs_blackboard_t *blackboard = NULL;

static int init_blackboard()
{
	assert(blackboard == NULL);
	blackboard = calloc(sizeof(fs_blackboard_t), 1);
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, blackboard->dosfds)) {
		log_error(MOD_DB, "multi-rw-thread sync dirty create dosfds fail errno=%d", errno);
		return -1;
	}
	return 0;
}

static void reset_blackboard()
{
	if (blackboard->dirtylist) {
		free(blackboard->dirtylist);
		blackboard->dirtylist = NULL;
	}
	if (blackboard->workerlist) {
		free(blackboard->workerlist);
		blackboard->workerlist = NULL;
	}
	blackboard->dirtycnt = 0;
	blackboard->workercnt = 0;
	blackboard->isworking = 0;
	blackboard->doscnt = 0;
}

static void * worker_main(void * param)
{
	int i;
	ssize_t ret;
	fs_db_object_t *dbo;
	fs_worker_t *worker = param;

	for (i = worker->index; i < blackboard->dirtycnt; i += blackboard->workercnt) {
		dbo = blackboard->dirtylist[i];
		GLOBAL_RLOCK_REQUIRE;
		if (0 == db_object_save_thr_safe(dbo)) {
			//以下dirty变量的改写需要注意:通过读写锁保证了主线程和工作线程的互斥，工作线程之间改写的内存是空集。
			dbo->dirty = 0; 
			//log_info_thr_safe(MOD_DB, "%dth thread save object pathname=%s", worker->index, dbo->pathname);
		}
		GLOBAL_LOCK_RELEASE;
	}
	ret = send(blackboard->dosfds[1], &dos_fin_work, sizeof(dos_fin_work), 0);
	if (ret < sizeof(dos_fin_work)) {
		//todo: if reach here, will not start saving any more.
		log_error_thr_safe(MOD_DB, "%dth thread send dos faile ret=%d,errno=%d", worker->index, ret, errno);
	} else {
		log_info_thr_safe(MOD_DB, "%dth thread finish and send dos ok", worker->index);
	}

	return NULL;
}


static int check_thread_finish()
{
	if (blackboard->doscnt == blackboard->workercnt) {
		//进入这里工作线程都已经完成工作了
		int i;
		for (i = 0; i < blackboard->workercnt; ++i) {
			if (pthread_join(blackboard->workerlist[i].pthid, NULL)) {
				log_error(MOD_DB, "error join thread index=%d,errno=%d", i, errno);
			}
		}
		log_info(MOD_DB, "multi-rw-thread sync dirty to disk end.dirtycnt=%lu,workercnt=%d", 
			blackboard->dirtycnt, blackboard->workercnt);
		free_offline();
		reset_blackboard();
		return 1;
	}

	return 0;
}

static void close_service();
void dbd_shutdown_daemon();
/*事件入口*/
static void thr_safe_timeout_cb(void *arg)
{
	int ret;

	if (blackboard == NULL) {
		log_error_thr_safe(MOD_DB, "error no blackboard");
		return;
	}

	if (blackboard->isworking) {
		//先检查一次线程是否结束, blackboard->workercnt 为0 的情况就会被处理掉而不需要去recv。
		if (0 == check_thread_finish()) {
			char dos[256];
			ret = recv(blackboard->dosfds[0], &dos, sizeof(dos) , MSG_DONTWAIT);
			if (ret > 0) {
				//todo == ?
				blackboard->doscnt += ret;
				check_thread_finish();
			} else if (ret == 0) {
				log_error_thr_safe(MOD_DB, "error dos read eof");
			} else {
				if (errno != EAGAIN) {
					log_error_thr_safe(MOD_DB, "error dos read errno=%d", errno);
				}
			}
		}
	} else {
		if (serialized_event != SEVENT_NULL) {
			int i;
			unsigned int event;
			log_info(MOD_DB, "check serialized event:%x", serialized_event);
			//先存一次盘再说
			flush_all_db_object();
			for(i = 0; i < sizeof(serialized_seq)/sizeof(serialized_seq[0]); ++i) {
				event = serialized_event & serialized_seq[i];
				if (event) {
					log_info(MOD_DB, "on serialized event:%x", event);
				}
				switch (event) {
					case SEVENT_SYNC_SAVE:
						break;
					case SEVENT_SIG_SAVE:
						break;
					case SEVENT_GAMED_EXIT:
						break;
					case SEVENT_SIG_EXIT:
						dbd_shutdown_daemon();
						break;
					case SEVENT_CONN_ERR:
						close_service();
						break;
				}
			}
			serialized_event = SEVENT_NULL;
		}
	}
}


static fs_db_object_t ** get_dbo_dirtylist(size_t *psize)
{
	apr_hash_index_t *hi;
	void *hdbo;
	fs_db_object_t *dbo;

	size_t index = 0;
	size_t size = apr_hash_count(dbo_base.hash);
	fs_db_object_t **dirtylist = calloc(size, sizeof(fs_db_object_t*));
	
	for ( index = 0, hi = apr_hash_first(NULL, dbo_base.hash); hi; hi = apr_hash_next(hi)) {
		apr_hash_this(hi, NULL, NULL, &hdbo);
		dbo = hdbo;
		if (dbo->dirty) {
			dirtylist[index++] = dbo;
		}
	}

	*psize = index;
	if (index > 0) {
		return dirtylist;
	} else {
		free(dirtylist);
		return NULL;
	}
}

static int get_worker_cnt(size_t dirtylistsize)
{
	int threadcnt = query_hw_ncpu();
	if (threadcnt <= 0) {
		threadcnt = 4;
	}
	threadcnt = dirtylistsize > threadcnt ? threadcnt : dirtylistsize;
	return threadcnt;
}

static void wr_thread_flush_all_db_object()
{
	fs_db_object_t **dirtylist;
	size_t dirtycnt;
	fs_worker_t *workerlist;	
	int workercnt, i;

	if (blackboard == NULL) {
		log_error_thr_safe(MOD_DB, "multi-rw-thread sync dirty no blackboard");
		return;
	}
	if (blackboard->isworking) {
		log_error_thr_safe(MOD_DB, "multi-rw-thread sync dirty are still working");
		return;
	}

	dirtylist = get_dbo_dirtylist(&dirtycnt);
	if (dirtylist == NULL || dirtycnt == 0) {
		log_info(MOD_DB, "multi-rw-thread sync no dirty to do");
		return;
	}
	workercnt = get_worker_cnt(dirtycnt);
	workerlist = calloc(sizeof(fs_worker_t), workercnt);

	if (blackboard->dirtylist) {
		free(blackboard->dirtylist);
	}
	if (blackboard->workerlist) {
		free(blackboard->workerlist);
	}
	blackboard->dirtylist = dirtylist;
	blackboard->dirtycnt = dirtycnt;
	blackboard->workerlist = workerlist;
	blackboard->isworking = 1;
	blackboard->workercnt = 0;
	blackboard->doscnt = 0;
	
	setproctitle("-dbd %s", "multi-rw-thread flushing dirty");
	log_info(MOD_DB, "multi-rw-thread sync dirty to disk begin");
	for (i = 0; i < workercnt; i++) {
		fs_worker_t *workerarg = &blackboard->workerlist[blackboard->workercnt];
		workerarg->index = blackboard->workercnt;
		if (0 == pthread_create(&workerarg->pthid, NULL, worker_main, workerarg)) {
			blackboard->workercnt++;
		} else {
			log_error_thr_safe(MOD_DB, "error multi-rw-thread create thread index=%d,errno=%d", i, errno);
		}
	}
}



static void game_input_sync(fs_net_connection_t *conn, fs_gd_header_t *header, void *buf, size_t len)
{
	//fprintf(stderr, "sync\n");
	fs_dbi_array_iter_t iter;
	FS_DBI_ARRAY_ITER_FIRST(&iter, buf, len);
	int method = iter.tv->number.n;

	if (method == FS_GD_MULTI_W_THREAD) {
		if (!blackboard->isworking) {
			thread_flush_all_db_object();
		}
	} else if (method == FS_GD_MULTI_RW_THREAD) {
		wr_thread_flush_all_db_object();
	} else {
		serialized_event |= SEVENT_SYNC_SAVE;
	}
}

static void game_input_gamed_exit(fs_net_connection_t *conn, fs_gd_header_t *header, void *buf, size_t len)
{
	serialized_event |= SEVENT_GAMED_EXIT;
	log_error_thr_safe(MOD_DB, "recv gamed exit cmd");
}

static void safe_game_input(fs_net_connection_t *fromconn, fs_gd_header_t *header, void *buf, size_t len, void *udata)
{
	fs_net_connection_t *conn = fromconn;
	char *data = header->payload_len == 0 ? NULL : buf;
	assert(conn == game_conn);
	assert(header->payload_len == len);

        switch (header->cmd) {
		case FS_GD_CMD_DB_OBJ_NEW:
			game_input_dbo_new(conn, header, data, len);
			break;
		case FS_GD_CMD_DB_OBJ_LOAD:
			game_input_dbo_load(conn, header, data, len, 0);
			break;
		case FS_GD_CMD_DB_OBJ_LOAD_SYNC:
			game_input_dbo_load(game_conn_sync, header, data, len, 1);
			break;
		case FS_GD_CMD_DB_OBJ_UNLOAD:
			game_input_dbo_unload(conn, header, data, len);
			break;
		case FS_GD_CMD_DB_OBJ_DATA:
			game_input_dbo_all_data(conn, header, data, len);
			break;
		case FS_GD_CMD_DB_OBJ_DATA_DIRTY:
			dirty_statics.count++;
			dirty_statics.size += len;
			game_input_dbo_dirty_data(conn, header, data, len);
			break;

		case FS_GD_CMD_SYNC:
			game_input_sync(conn, header, data, len);
			//fprintf(stderr, "flush dirty statics count=%u,size=%luk\n", dirty_statics.count, dirty_statics.size/1024);
			dirty_statics_reset(&dirty_statics);
			break;
		case FS_GD_CMD_RENAME: 
			game_input_dbo_rename(conn, header, data, len);
			break;
		case FS_GD_CMD_GAMED_EXIT:
			game_input_gamed_exit(conn, header, data, len);
			break;

		case FS_GD_RPC: {
			fs_inbuf_t inbuf;	
			FS_INBUF_INIT(&inbuf, data, len);
			fs_rpc_dispatch(dbd_rpc_lpc, header->uid, &inbuf);
			break;
		}

		default:
			fprintf(stderr, "dbd input unknow cmd:%d\n", header->cmd);
		}

}

static void game_input(fs_net_connection_t *fromconn, fs_gd_header_t *header, void *buf, size_t len, void *udata)
{
	GLOBAL_WLOCK_REQUIRE;
	safe_game_input(fromconn, header, buf, len, udata);
	GLOBAL_LOCK_RELEASE;
}

/*事件入口*/
static void game_connection_recv(struct bufferevent *bufev, void *arg)
{
	READ_PACKET(fs_gd_header_t, bufev, arg, game_input, NULL);
}

/*事件入口*/
static void game_connection_error(struct bufferevent *bufev, short what, void *arg)
{
	log_error_thr_safe(MOD_DB, "game connection error:%x", what);
	serialized_event |= SEVENT_CONN_ERR;
}

static void close_service()
{
	if (is_service_open) {
		log_error(MOD_DB, "dbd close service");
		if (game_conn != NULL) {
			fs_net_connection_destroy(game_conn);
			game_conn = NULL;
		}

		if (game_conn_sync != NULL) {
			fs_net_connection_destroy(game_conn_sync);
			game_conn_sync = NULL;
		}

		is_service_open = 0;
	} 
}

static int open_service(int fd)
{
	if (!is_service_open) {
		fprintf(stderr, "open service\n");
		if (game_conn == NULL) {
			//fs_net_acceptor_t *acceptor = arg;
			int conn_fd = accept(fd, NULL, NULL);
			if (conn_fd < 0) {
				perror("accept game connection");
				return -1 ;
			}
			fs_net_set_nonblock(conn_fd);
			game_conn = fs_net_connection_new(conn_fd, game_connection_recv, NULL, 
					game_connection_error, game_max_outbuf, NULL);
			fprintf(stderr, "dbd accept async game:%d\n", conn_fd);
		} else if (game_conn_sync == NULL) {
			int sync_conn_fd = accept(fd, NULL, NULL);
			if (sync_conn_fd < 0) {
				perror("accept sync game connection");
				return -1 ;
			}
			fs_net_set_nonblock(sync_conn_fd);
			game_conn_sync = fs_net_connection_new(sync_conn_fd, game_connection_recv, NULL, 
					game_connection_error, game_max_outbuf, NULL);
			printf("dbd accept another async game for blocking:%d\n", sync_conn_fd);
		}

		if (game_conn && game_conn_sync) {
			is_service_open = 1;
		}
		return 0;
	}

	return -1;
}


static void game_accept(int fd, short evtype, void *arg)
{
	if (0 > open_service(fd)) {
		//to drain the another connection
		int conn_fd = accept(fd, NULL, NULL);
		fprintf(stderr, "deny another gamed connection\n");
		//handle the event and throw it.
		close(conn_fd);
	};
}

static int rpc_call_imp(int argc, svalue_t *argv)
{
	int gamed;
	if (fs_rpc_lpc_pack_arg(argc, argv, dbd_rpc_lpc, gamed_netbuf, &gamed)) {
		return -1;
	}

	FS_GD_SET_HEADER((fs_gd_header_t*)gamed_netbuf->head, FS_GD_RPC, gamed, FS_NET_MBUF_PAYLOAD_LEN(gamed_netbuf));
	fs_net_mbuf_send(gamed_netbuf, game_conn);
	return 0;
}

//static 
void dbd_shutdown_daemon()
{
	fs_apr_hash_destruct(&dbo_base);
	apr_terminate();
	fs_net_mbuf_free(gamed_netbuf);

	if (game_acceptor != NULL) 
		fs_net_acceptor_destroy(game_acceptor);

	event_loopexit(NULL);
}

/*事件入口*/
static void signal_event(int sig, short evtype, void *arg)
{
	log_error_thr_safe(MOD_DB, "dbd catch signal:%d", sig);
	if (sig == SIGINT) {
		serialized_event |= SEVENT_SIG_EXIT;
	} else {
		serialized_event |= SEVENT_SIG_SAVE;
	}
}

/*事件入口*/
/*这个函数是救火用的，就是要实时响应并中断当前的处理流程,即使会导致多线程冲突*/
static void emergency_interrupt(int sig)
{
	log_error_thr_safe(MOD_DB, "emergency interrupt:%d", sig);
	flush_all_db_object();
}

void static init()
{
	event_init();
	init_signal(signal_event, emergency_interrupt);
        apr_initialize();
	//fs_mysql_module_init();

	restore_dbd_proctitle();

	fs_apr_hash_construct(&dbo_base);

	if (init_blackboard()) {
		exit(1);
	};

	struct timeval period;
	period.tv_sec = DBD_CALLOUT_SEC;
	period.tv_usec = DBD_CALLOUT_USEC;
	add_timer(period, thr_safe_timeout_cb, NULL);

	int game_fd = fs_net_listen(dbd_ip, dbd_port, backlog, 0);
	if (game_fd < 0) {
		log_error(MOD_DB, "error dbd listen:%s, %d", dbd_ip, dbd_port);
		exit(1);
	}
	log_info(MOD_DB, "dbd listen:%s, %d", dbd_ip, dbd_port);
	game_acceptor = fs_net_acceptor_new(game_fd, game_accept, NULL);

	gamed_netbuf = fs_net_mbuf_new(sizeof(fs_gd_header_t));

	char *rpc_cfg = fs_config_get_string(RPC_CFG);
	dbd_rpc_lpc = fs_rpc_lpc_create(rpc_cfg, rpc_call_imp);
}

static void setup_config()
{
	char *ip = fs_config_get_string(DBD_IP);
	if (ip != NULL) {
		dbd_ip = strdup(ip);
	}

	int port = fs_config_get_int(DBD_PORT);
	if (port >= 0) {
		dbd_port = port;
	}

}

void module_init_dbd()
{
	setup_config();
	init();

	// 初始化时钟
	int sec, usec;
	sec = fs_config_get_int(SYSTEM_INTERVAL_SEC); 
	usec = fs_config_get_int(SYSTEM_INTERVAL_USEC); 
	int callout_init( int sec, int usec);
	callout_init( sec, usec );
}

void system_startup_dbd()
{
	call_script_func("/rc/dbd_main", "main", "");
	event_dispatch();
}

