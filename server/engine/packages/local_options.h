// ���Ƶ�options
// -------------------------------------------
// ע��:packages���������ܳ���100
// -------------------------------------------

#include "grammar/options.h"
#include "include/cc.h"

// LPC���Դ���
#define PACKAGE_CONTRIB
#define PACKAGE_DEVELOP
#define PACKAGE_MATH
#define PACKAGE_SOCKETS
/*
#ifdef GAME_MODULE
// ������ģ��
#define PACKAGE_FS_MAIN
#define PACKAGE_FS_RPC_LPC
#endif
#ifdef DB_MODULE
#endif

#ifdef NETD_MODULE
#define PACKAGE_FS_RPC_LPC
#define PACKAGE_FS_NETD
#endif
*/
#define PACKAGE_FS_MAIN
#define PACKAGE_FS_RPC_LPC
#define PACKAGE_FS_NETD
#define PACKAGE_FS_HTTP_LPC
#define PACKAGE_FS_DBD
#define PACKAGE_FS_MYSQL
