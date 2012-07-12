#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "fs_log.h"
#include "config.h"

#define MAX_TIME_STR_LEN  30
#define MAX_LOG_BUF_LEN   256

static char g_strLogBuf[MAX_LOG_BUF_LEN];
static char g_eLogSwitch = LOG_INFO;
static char g_eLogMod = MOD_ALL;

pthread_mutex_t dbd_log_lock = PTHREAD_MUTEX_INITIALIZER;

inline const char *_level_string(LOG_LEVEL_E e_level)
{
    switch (e_level)
    {
        case LOG_DEBUG:
            return "debug:";
        case LOG_INFO:
            return "info:";
		case LOG_WARNING:
			return "warn:";
        case LOG_ERROR:
			return "error:";
        case LOG_FATAL:
			return "fatal:";
        default:
			break;
    }   
	return "";
}

void fs_debug_printf(LOG_MODULE_E e_mod, LOG_LEVEL_E e_level, const char *strfile, int line, const char *pformat, ...)
{
    va_list args;
    time_t t;
    char strtime[MAX_TIME_STR_LEN];
    const char *filename = NULL;
    const char *strtmp = NULL;
	const char *strlevel;

	static int append = 0;
	static char deb_buf[100];
	static char *deb = deb_buf;
	FILE *fp = NULL;
    
    if (((e_mod <= MOD_ALL ) || (e_mod >= MOD_NONE))
        || (NULL == pformat))
    {
        return;        
    }    

    if ((g_eLogSwitch != LOG_ALL) && (e_level < g_eLogSwitch)) {
        return;
    }
	if (g_eLogMod && (e_mod != g_eLogMod)) {
		return;
	}

    (void)time(&t);
	strftime(strtime, sizeof(strtime), "%Y-%m-%d %H:%M:%S", localtime(&t) ); 

    strtmp = strrchr(strfile, '/');

    if (NULL == strtmp) {
        filename = strfile;
    } else {
        filename = strtmp + 1;
    }  

	strlevel = _level_string(e_level);

    va_start(args, pformat);
    (void)vsnprintf(g_strLogBuf, MAX_LOG_BUF_LEN, pformat, args);
    va_end(args);    

	if (!append)
	{
		snprintf(deb, sizeof(deb_buf), "%s/fs_log.log", LOG_DIR);
		while (*deb == '/')
		{
			deb++;
		}
	}
	int _Touch(const char *PathFile);

	_Touch(deb);
	fp = fopen(deb, append ? "a" : "w");
	/*
	 * re-use stdout's file descriptor if system or process runs out
	 * OS/2 doesn't have ENFILE.
	 */
	if (!fp && (errno == EMFILE 
#ifdef ENFILE
				|| errno == ENFILE 
#endif 
		   ) )
	{
		fp = freopen(deb, append ? "a" : "w", stdout);
		append = 2;
	}
	else {
		append = 1;
	}

	if (!fp) {
		perror(deb);
		return;
		// abort();
	}

    fprintf(fp, "[%s] [%s:%d] [mod=%d] %s %s\n", strtime, filename, line, e_mod, strlevel, g_strLogBuf);
	fflush(fp);
    fprintf(stderr, "[%s] [%s:%d] [mod=%d] %s %s\n", strtime, filename, line, e_mod, strlevel, g_strLogBuf);
	fflush(stderr);

	/*
	 * don't close stdout
	 */
	if (append != 2) {
		(void)fclose(fp);
	}

	/*
	 * append to debug.log next time thru
	 */
	if (!append) {
		append = 1;
	}

    return;     
}

void fs_set_log_switch_level(LOG_LEVEL_E e_switch_level)
{
	g_eLogSwitch = e_switch_level;
}

void fs_set_log_switch_module(LOG_MODULE_E e_switch_module)
{
	g_eLogMod = e_switch_module;
}

