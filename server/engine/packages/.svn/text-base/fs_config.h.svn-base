
#ifndef __FS_CONFIG_H__
#define __FS_CONFIG_H__

#include "lpc_incl.h"
#include "object.h"
#include "fs_lpc_op.h"

#define CONFIG_OBJECT "/rc/config.c"

#define MAX_ONLINE_KEY "MAX_ONLINE"
#define MAX_LOGIN_KEY  "MAX_LOGIN"
#define SYSTEM_INTERVAL_SEC "SYSTEM_INTERVAL_SEC"
#define SYSTEM_INTERVAL_USEC "SYSTEM_INTERVAL_USEC"
#define RPC_CFG "RPC_CFG"
#define NETD_IP "NETD_IP"
#define NETD_PORT "NETD_PORT"
#define NETD_OUTER_IP "NETD_OUTER_IP"

#define NETD_OUTER_PORT "NETD_OUTER_PORT"

#define NETD_HOST_PORT "NETD_HOST_PORT"
#define NETD_AUTH_PORT "NETD_AUTH_PORT"

#define HOST_ID "HOST_ID"

#define DBD_IP "DBD_IP"
#define DBD_PORT "DBD_PORT"
#define FS_PATH "FS_PATH"
#define ENGINE_NAME "ENGINE_NAME"
#define SERVER_INI "SERVER_INI"

#define HTTPD_PORT "HTTPD_PORT"

char * fs_config_get_string(char *key);
int fs_config_get_int(char *key);
// void load_config_file(char *filename);
svalue_t *get_config_svalue();
int fs_config_get_host_hop(int hid, char **ip, unsigned *port);

#endif // __FS_CONFIG_H__
