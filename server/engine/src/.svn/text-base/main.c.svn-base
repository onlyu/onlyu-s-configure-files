#define SUPPRESS_COMPILER_INLINES
#include "std.h"
#include "file_incl.h"
#include "lpc_incl.h"
#include "lex.h"
#include "file.h"
#include "backend.h"
#include "simul_efun.h"
#include "binaries.h"
#include "main.h"
#include "otable.h"
#include "comm.h"
#include "compiler.h"
#include "port.h"
#include "md.h"
#include "main.h"
#include "compile_file.h"
#include "object.h"
#include "stack.h"

#define TRAP_CRASHES

#define LIB_PATH_SIZE   1024

port_def_t external_port[5];

int comp_flag = 0; /* Trace compilations */
int run_flag = 0;
int auth_flag = 0; /* Trace compilations */

int max_cost;

int boot_time;
int max_array_size;
int max_buffer_size;
int max_string_length;
char *reserved_area; /* reserved for MALLOC() */

char game_lib_path[LIB_PATH_SIZE];       /*游戏目录的绝对路径*/
char game_lib_bin_path[LIB_PATH_SIZE];   /*游戏目录的绝对路径*/

svalue_t const0, const1, const0u;

double consts[NUM_CONSTS];
char *config_file = NULL;

/* -1 indicates that we have never had a master object.  This is so the
 * simul_efun object can load before the master. */
object_t *master_ob = 0;

#ifdef SIGNAL_FUNC_TAKES_INT
#define SIGPROT (int)
#define PSIG(z) z (int  sig)
#else 
#define SIGPROT (void)
#define PSIG(z) z()
#endif 

static void CDECL sig_fpe SIGPROT;
static void CDECL sig_cld SIGPROT;

#ifdef TRAP_CRASHES
static void CDECL sig_usr1 SIGPROT;
static void CDECL sig_usr2 SIGPROT;
static void CDECL sig_term SIGPROT;
static void CDECL sig_int SIGPROT;

#ifndef DEBUG
//static void CDECL sig_segv SIGPROT;
static void CDECL sig_hup SIGPROT, CDECL sig_abrt SIGPROT,  CDECL sig_ill SIGPROT, CDECL sig_bus SIGPROT, CDECL sig_iot SIGPROT;
#endif /* DEBUG */
#endif /* TRAP_CRASHES */


#ifdef DEBUG_MACRO
/* used by debug.h: please leave this in here -- Tru (you can change its value if you like).  */
int debug_level = 32768 | 512;
#endif /* DEBUG_MACRO */

void compile_all_file(char *dir, int depth);
int maxdepth = 10;

char *format_time(time_t tm)
{
	static char strtime[30];
	strftime(strtime, 30, "%Y-%m-%d %H:%M:%S", localtime(&tm) ); //strftime会添加\0
	return strtime;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int vm_init()
{
	printf("Please wait while initializing internal tables....\n");

#ifdef WRAPPEDMALLOC
	wrappedmalloc_init();
#endif /* WRAPPEDMALLOC */
#ifdef DEBUGMALLOC
	MDinit();
#endif 

	boot_time = get_current_time();
	// 初始化Efun用的當前時間
	current_time = boot_time;

	const0.type = T_NUMBER;
	const0.u.number = 0;
	const1.type = T_NUMBER;
	const1.u.number = 1;

	/* const0u used by undefinedp() */
	const0u.type = T_NUMBER;
	const0u.subtype = T_UNDEFINED;
	const0u.u.number = 0;

	fake_prog.program_size = 0;

	// 初始化字符串
	init_strings();
	// 初始化lpc object table
	init_otable();
	// 初始化保留字
	init_identifiers();
	// 初始化編譯器
	init_locals();

	max_cost = RUN_COST;
	eval_cost = max_cost;

	max_array_size = MAX_ARRAY_SIZE;
	max_buffer_size = MAX_BUFFER_SIZE;
	max_string_length = MAX_STRING_LENGTH;

	return 0;
}

void vm_startup(char *libpath)
{
	error_context_t econ;

	// 初始化目錄
	if (chdir(libpath) ==  - 1)
	{
		/*   Bad mudlib directory: %s\n            */
		fprintf(stderr, "Error:Bad lib directory: %s\n", libpath);
		exit( - 1);
	}

	memset(game_lib_path, '\0', sizeof(game_lib_path));
	if (getcwd(game_lib_path, LIB_PATH_SIZE) == NULL) {
		fprintf(stderr, "Error:Bad lib directory: %s\n", libpath);
		exit(-1);
	}

	snprintf(game_lib_bin_path, sizeof(game_lib_bin_path), "%s/bin", game_lib_path);

	set_inc_list(INCLUDE_DIRS);

	init_num_args();
	reset_machine(1);

	add_predefines(game_lib_path);

	save_context(&econ);
	if (SETJMP(econ.context) )
	{
		/* The simul_efun (%s) and master (%s) objects must be loadable.\n"*/
		debug_message("The simul_efun (%s) objects must be loadable.\n", SIMUL_EFUN);
		exit( - 1);
	}
	else
	{
		init_simul_efun(SIMUL_EFUN);
	}
	pop_context(&econ);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

/*
static mapping_t *load_config(char *config_file)
{
	struct stat st;

	if (stat(config_file, &st) ==  - 1)
	{
		printf("配置文件[%s]不存在\n", config_file);
		exit(-1);
	} 
	else if (S_IFDIR &st.st_mode)
	{
		printf("配置文件[%s]是一个目录\n", config_file);
		exit(-1);
	}

	return NULL;
}
*/

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//extern void system_startup(void);
extern void module_init_gamed(void);
extern void module_init_netd(void);
extern void module_init_dbd(void);

extern void system_startup_gamed(void);
extern void system_startup_netd(void);
extern void system_startup_dbd(void);

extern char * fs_config_get_string(char *key);
extern int fs_config_get_int(char *key);
extern void load_config_file(char *filename);

int main(int argc, char **argv)
{
	error_context_t econ;
	int ch;
	char *runfile = NULL;
	char *libpath = NULL;

	//不同进程的入口函数
	void (*module_init)(void);
	void (*startup)(void);

	module_init = module_init_gamed;
	startup = system_startup_gamed;

	//while ((ch = getopt(argc, argv, "c:br:")) != -1)
	while ( (ch = getopt(argc, argv, "f:andbr:l:") ) !=  - 1)
	{
		printf("optind: %d\n", optind);
		switch (ch)
		{
			case 'l':
				// 初始化配置文件
				libpath = optarg;
				break;
			case 'f':
				// 初始化config_file
				config_file = optarg;
				break;
			case 'b':
				comp_flag = 1;
				break;
			case 'r':
				run_flag = 1;
				runfile = optarg;
				break;
			case 'a': 
				auth_flag = 1;		
			case 'n':
				module_init = module_init_netd;
				startup = system_startup_netd;
				break;
			case 'd':
				module_init = module_init_dbd;
				startup = system_startup_dbd;
				break;
			default:
				printf("未知参数:%c\n", ch);
				break;
		}
	}

	if (!libpath) {
		libpath = QTZ_ServerEngine_LIB;
	}


	// 初始化
#ifdef BINARIES               
	init_binaries(argc, argv);
#endif 

	// 初始化虚拟机, 这样配置文件可以存为一个map
	vm_init();
	vm_startup(libpath);


// 注册信号处理
//取消信号处理
#define signal(sig, handler) (void)(handler);

#ifdef SIGFPE
	signal(SIGFPE, sig_fpe);
#endif 
#ifdef TRAP_CRASHES
#ifdef SIGUSR1
	signal(SIGUSR1, sig_usr1);
#endif 
#ifdef SIGUSR2
	signal(SIGUSR2, sig_usr2);
#endif 
	signal(SIGTERM, sig_term);
	signal(SIGINT, sig_int);
#ifndef DEBUG
#if defined(SIGABRT) && !defined(LATTICE)
	signal(SIGABRT, sig_abrt);
#endif 
#ifdef SIGIOT
	signal(SIGIOT, sig_iot);
#endif 
#ifdef SIGHUP
	signal(SIGHUP, sig_hup);
#endif 
#ifdef SIGBUS
	signal(SIGBUS, sig_bus);
#endif 
#ifndef LATTICE
	signal(SIGILL, sig_ill);
#endif 
#endif //DEBUG
#endif 
#ifndef WIN32
#ifdef USE_BSD_SIGNALS
	signal(SIGCHLD, sig_cld);
#else 
	signal(SIGCLD, sig_cld);
#endif 
#endif 

	module_init();

	// 编译
	if (comp_flag)
	{
		save_context(&econ);
		if (SETJMP(econ.context) )
		{
			debug_message("compile error!!!\n");
			exit( - 1);
		}

		call_script_func("/rc/main", "pre_init", "");
		compile_all_file("/", 0);
		pop_context(&econ);
		debug_message("编译代码完毕\n");

		return 0;
	}

	if (runfile != NULL)
	{
		printf("Want To run file %s \n ", runfile);
		
		//call_script_func("/rc/main", "pre_init", "");
		call_script_func(runfile, "main", "");

		return 0;
	}

	//system_startup();
	startup();
	return 0;
}


void compile_all_file(char *path, int depth)
{
	DIR *dirp;
	struct dirent *dp;
	char realpath[256];
	char pathname[256];

	if (strcmp(path, "/") == 0 || strcmp(path, game_lib_path) == 0)
	{
		path = "";
		depth = 0;
	} depth += 1;
	if (depth >= maxdepth)
	{
		return ;
	}
	strlcpy(realpath, game_lib_path, sizeof(realpath) );
	strlcat(realpath, "/", sizeof(realpath) );
	strlcat(realpath, path, sizeof(realpath) );

	dirp = opendir(realpath);
	if (dirp == NULL)
	{
		printf("error compile path: %s\n", path);
		return ;
	}
	while ( (dp = readdir(dirp) ) != NULL)
	{
		if (strcmp(dp->d_name, "..") == 0 || strcmp(dp->d_name, ".") == 0)
		{
			continue;
		}
		strlcpy(pathname, path, sizeof(pathname) );
		strlcat(pathname, "/", sizeof(pathname) );
		strlcat(pathname, dp->d_name, sizeof(pathname) );
		if (dp->d_type == DT_DIR || dp->d_type == DT_LNK)
		{
			compile_all_file(pathname, depth);
		}
		else
		{
			if (dp->d_namlen >= 2 && (dp->d_name[dp->d_namlen - 1] == 'c' || dp->d_name[dp->d_namlen - 1] == 'C') && dp->d_name[dp->d_namlen - 2] == '.')
			{
				find_object(pathname);
			}
		}
	}
	(void)closedir(dirp);
	return ;

}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#ifdef DEBUGMALLOC
char *int_string_copy(char *str, char *desc)
#else 
char *int_string_copy(char *str)
#endif 
{
	char *p;
	int len;

	/*    Null string passed to string_copy.\n           */
	DEBUG_CHECK(!str, "Null string passed to string_copy.\n");
	len = strlen(str);
	if (len > max_string_length)
	{
		len = max_string_length;
		p = new_string(len, desc);
		(void)strncpy(p, str, len);
		p[len] = '\0';
	}
	else
	{
		p = new_string(len, desc);
		(void)strncpy(p, str, len + 1);
	}
	return p;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#ifdef DEBUGMALLOC
char *int_string_unlink(char *str, char *desc)
#else 
char *int_string_unlink(char *str)
#endif 
{
	malloc_block_t *mbt,  *newmbt;

	mbt = ( (malloc_block_t*)str) - 1;
	mbt->ref--;

	if (mbt->size == USHRT_MAX)
	{
		int l = strlen(str + USHRT_MAX) + USHRT_MAX; /* ouch */

		newmbt = (malloc_block_t*)DXALLOC(l + sizeof(malloc_block_t) + 1, TAG_MALLOC_STRING, desc);
		memcpy( (char*) (newmbt + 1), (char*) (mbt + 1), l + 1);
		newmbt->size = USHRT_MAX;
		ADD_NEW_STRING(USHRT_MAX, sizeof(malloc_block_t) );
	}
	else
	{
		newmbt = (malloc_block_t*)DXALLOC(mbt->size + sizeof(malloc_block_t) + 1, TAG_MALLOC_STRING, desc);
		memcpy( (char*) (newmbt + 1), (char*) (mbt + 1), mbt->size + 1);
		newmbt->size = mbt->size;
		ADD_NEW_STRING(mbt->size, sizeof(malloc_block_t) );
	}
	newmbt->ref = 1;
	CHECK_STRING_STATS;

	return (char*) (newmbt + 1);
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int _Touch(const char *PathFile)
{
	struct stat st;
	char TempDir[strlen(PathFile) + 1];
	const char *Start,  *Index;
	int fd;
	if (stat(PathFile, &st) !=  - 1)
	{
		return 1;
	}

	Start = PathFile;
	Index = NULL;
	while ( (Index = strchr(Start, '/') ) != NULL)
	{
		strlcpy(TempDir, PathFile, Index - PathFile + 1);
		Start = Index + 1;
		mkdir(TempDir, 0755);
	} 

	fd = open(PathFile, O_WRONLY | O_CREAT, 0644);
	if (fd !=  - 1)
	{
		close(fd);
		return 1;
	}
	else
	{
		return 0;
	}
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

void debug_message(char *fmt, ...)
{
	static int append = 0;
	static char deb_buf[100];
	static char *deb = deb_buf;
	time_t tm;
	va_list args;
	FILE *fp = NULL;
	V_DCL(char *fmt);

	if (!append)
	{
		/*
		 * check whether config file specified this option
		 */
		if (strlen(DEBUG_LOG_FILE) )
		{
			snprintf(deb, sizeof(deb_buf), "%s/%s", LOG_DIR, DEBUG_LOG_FILE);
		}
		else
		{
			snprintf(deb, sizeof(deb_buf), "%s/debug.log", LOG_DIR);
		}
		while (*deb == '/')
		{
			deb++;
		}
	}
	_Touch(deb);
	fp = fopen(deb, append ? "a" : "w");

	/*
	 * re-use stdout's file descriptor if system or process runs out
	 *
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
	} else {
		append = 1;
	}

	if (!fp)
	{
		/* darn.  We're in trouble */
		perror(deb);
		abort();
	}
	time(&tm);

	fprintf(fp, "%s\t", format_time(tm) );
	V_START(args, fmt);
	V_VAR(char *, fmt, args);
	vfprintf(fp, fmt, args);
	fflush(fp);
	va_end(args);
	V_START(args, fmt);
	V_VAR(char *, fmt, args);
	vfprintf(stderr, fmt, args);
	fflush(stderr);
	va_end(args);

	/*
	 * don't close stdout
	 */
	if (append != 2)
	{
		(void)fclose(fp);
	}

	/*
	 * append to debug.log next time thru
	 */
	if (!append)
	{
		append = 1;
	}
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int slow_shut_down_to_do = 0;

char *xalloc(int size)
{
	char *p;
	static int going_to_exit;

	if (going_to_exit)
	{
		exit(3);
	}
#ifdef DEBUG
	if (size == 0) /*      Tried to allocate 0 bytes.         */
	{
		fatal("Error: Tried to allocate 0 bytes.\n");
	}
#endif 
	p = (char*)DMALLOC(size, TAG_MISC, "main.c: xalloc");
	if (p == 0)
	{
		if (reserved_area)
		{
			FREE(reserved_area);
			/*   Temporarily out of MEMORY. Freeing reserve.\n            */

			p = "Error:Temporarily out of MEMORY. Freeing reserve.\n";
			write(1, p, strlen(p) );
			reserved_area = 0;
			slow_shut_down_to_do = 6;
			return xalloc(size); /* Try again */
		}
		going_to_exit = 1;
		/*     Totally out of MEMORY.          */
		fatal("Totally out of MEMORY:.\n");
	}
	return p;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

static void CDECL PSIG(sig_cld)
{
#ifndef WIN32
	int status;
	(void)sig;
#ifdef USE_BSD_SIGNALS
	while (wait3(&status, WNOHANG, NULL) > 0)
		;
#else 
	wait(&status);
	signal(SIGCLD, sig_cld);
#endif 
#endif 
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------


static void CDECL PSIG(sig_fpe)
{
	fatal("Floating-point Exception!"); 
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

#ifdef TRAP_CRASHES

/* send this signal when the machine is about to reboot.  The script
 * which restarts the MUD should take an exit code of 1 to mean don't restart
 */

static void CDECL PSIG(sig_usr1)
{
	/*       Host machine shutting down        */
	push_constant_string("Host machine is shutting down");
	push_undefined();
	push_undefined();
	apply_master_ob(APPLY_CRASH, 3);
	/*   Received SIGUSR1, calling exit(-1)\n           */
	debug_message("OS Received SIGUSR1, calling exit(-1)\n");
	exit( - 1);
}

/* Abort evaluation */
static void CDECL PSIG(sig_usr2)
{
	debug_message("OS Received SIGUSR2\n");
	/*
	   int i;
	   char sMsg[] = "try to reboot.";

	   eval_cost = 1;
	   for (i = 0; i < max_users; i++) {
		   if (all_users[i] && !(all_users[i]->iflags & CLOSING)){
		   TellUserFd(all_users[i]->vfd, sMsg, sizeof(sMsg), 255);
	   }
		}
	 */
}

/*
 * Actually, doing all this stuff from a signal is probably illegal
 */
static void CDECL PSIG(sig_term)
{
	/*     Process terminated          */
	debug_message("Process terminated, calling shutdownOS(-99)\n");
	OS_is_being_shut_down = 1;

	shutdownOS( - 99);
}

static void CDECL PSIG(sig_int)
{
	/*   Process interrupted            */
	debug_message("Process terminated, calling shutdownOS(-100)\n");
	OS_is_being_shut_down = 1;
	shutdownOS( - 100);
}

#ifndef DEBUG
#if 0
static void CDECL PSIG(sig_segv)
{
	/*   Segmentation fault            */
	debug_message("\nOOPS!! Error:Segmentation falut :( ");
	exit(-9);
}
#endif 

static void CDECL PSIG(sig_bus)
{
	/*   Bus error            */
	debug_message("\nOOPS!! Error:Bus error :( ");
	exit(-8);
}

static void CDECL PSIG(sig_ill)
{
	/*   Illegal instruction            */
	fatal("Error:Illegal instruction");
}

static void CDECL PSIG(sig_hup)
{
	/*      Hangup!         */
	fatal("OS Hangup!");
}

static void CDECL PSIG(sig_abrt)
{
	/*   Aborted            */
	fatal("OS Aborted");
}

static void CDECL PSIG(sig_iot)
{
	/*      Aborted(IOT)         */
	fatal("OS Aborted(IOT)");
}
#endif /* !DEBUG */

#endif /* TRAP_CRASHES */
