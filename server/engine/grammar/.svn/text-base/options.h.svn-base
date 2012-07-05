#ifndef _OPTIONS_H_
#define _OPTIONS_H_

/* 
 * YOU PROBABLY DO NOT WANT TO MODIFY THIS FILE.
 *
 * Do 'cp options.h local_options' and edit that instead.  local_options,
 * if it exists, overrides this file.
 *
 * The advantage is that when you upgrade to a newer MudOS driver, you can
 * simply copy your local_options file into the src directory.  The build
 * process will warn you if new options have been added that you should
 * choose settings for.
 */

/****************************************************************************
 * EVERY time you change ANYTHING in this file, RECOMPILE from scratch.     *
 * (type "make clean" then "make" on a UNIX system) Failure to do so may    *
 * cause the driver to behave oddly.                                        *
 ****************************************************************************/

/* NOTES:
 * Many of the configurable options are now set via the configuration file
 *  that is specified as the first argument to the driver.
 * See port.h for those #defines related to portability (compatibility) if
 *  you have problems compiling on your system.
 * Removing an efun from func_spec.c usually removes most, if not all,
 *  of the code associated with it.
 * Note that anything defined in this file is also visible to LPC files
 * surrounded by __.  So #define FOO in this file defines __FOO__ for
 * all LPC files.  This allows code like:
 *
 * #ifdef __SENSIBLE_MODIFIERS__
 * ...
 */

/****************************************************************************
 *                              MALLOC                                      *
 *                             --------                                     *
 * For performance reasons, LP drivers have a variety of memory allocation  *
 * packages.  If you don't care, use the default one on your system:        *
 * #define SYSMALLOC, #undef the others.                                    *
 ****************************************************************************/

/* You must choose exactly one of these malloc packages:
 *     ~~~~
 * SYSMALLOC:
 *   * Built-in system malloc.
 *   * 内置系统分配。
 *   * No statistics.
 *   * 没有统计状态。
 *   * SYSMALLOC incurs no additional CPU or memory overhead.
 *   * SYSMALLOC 将导致没有额外的处理器或内存开销。
 *
 * SMALLOC:
 *   * Satoria's smalloc.
 *   * Satoria 的内存分配方式。
 *   * Statistics available. (see wrappers and DO_MSTATS)
 *   * 允许统计状态。（参见 wrappers 及 DO_MSTATS 选项）
 *   * Faster than most system mallocs with modest ammount of memory overhead.
 *   * 比大多数系统内存管理的速度更快，而且有适度的内存开销。
 *   * Can fall back onto system malloc if sbrk() not ok.
 *   * 在 sbrk() 函数失败时，可以返回系统内存管理状态。
 *
 * BSDMALLOC:
 *   * BSD (Berkeley Software Distributions) malloc.
 *   * BSD 的内存分配方式。
 *   * Statistics available. (see wrappers and DO_MSTATS)
 *   * 允许统计状态。（参见 wrappers 及 DO_MSTATS 选项）
 *   * Faster than SMALLOC but more memory overhead.
 *   * 比 SMALLOC 的速度更快，而且有更多的内存开销。
 *   * Requires sbrk().
 *   * 需要 sbrk 函数。
 */
//#define SYSMALLOC
#undef SYSMALLOC
#undef SMALLOC
//#undef BSDMALLOC
#define BSDMALLOC

/* 你必须选择以下任意一个或者完全不选择以下内存管理选项。这些选项与以上内存分配
 * 选项密切相关。
 *
 * WRAPPEDMALLOC:
 *   * 有限统计。
 *   * 有限额外处理器开销及无额外内存开销。
 *
 * DEBUGMALLOC:
 *   * 统计内存被分配数量（比 WRAPPEDMALLOC 状态好）。
 *   * 完美的处理器及内存开销。
 */
#undef WRAPPEDMALLOC
#undef DEBUGMALLOC

/* 以下选项将增加某种配件到内存管理之中：*/
#undef SBRK_OK
#undef DO_MSTATS
#undef DEBUGMALLOC_EXTENSIONS
#undef CHECK_MEMORY

#undef HAS_STATUS_TYPE
#undef SANE_EXPLODE_STRING
#undef REVERSIBLE_EXPLODE_STRING
#undef CAST_CALL_OTHERS
#define NONINTERACTIVE_STDERR_WRITE
#undef OLD_RANGE_BEHAVIOR

/* COMPRESS_FUNCTION_TABLES: 使函数表占用较少的内存，而函数调用的开销只有轻微的
 * 增加。
 * 
 * 兼容情况：速度开销几乎可以忽略。
 */
#undef COMPRESS_FUNCTION_TABLES

/*
 * 定义该项只是为了将系统的 crypt() 函数以 Fermat@Equilibria 的 MD5 取代。这样
 * 在所有的体系中给出同样的值可以有更大的利益，而且比 UNIX 正常的 crypt() 函数
 * 更加强壮。
 */
#undef CUSTOM_CRYPT

/*
  目前有如下特点：
 * 
 * . m_indices() 及 m_values() 分别对 keys() 及 values() 而言是同义函数。
 * . map_delete() 返回它的首个参数。
 * . inherit_list() 意味着 deep_inherit_list()，而不是 shallow_inherit_list()。
 * . heart_beat_info() 是 heart_beats() 的同义函数。
 */
#undef COMPAT_32

/*
 * 保持关于分配字串等的统计表。它能被 QTZ_ServerEngine_status() 超越函数所查看。如果该项被关
 * 闭，QTZ_ServerEngine_status() 及 memory_info() 将忽略分配字串，但字串操作运行得更快。
 */
#undef STRING_STATS

/*
 * 保持关于分配数组等的统计表。它能被 QTZ_ServerEngine_status() 超越函数所查看。如果该项被关
 * 闭，QTZ_ServerEngine_status() 及 memory_info() 将忽略分配数组，但数组操作运行得更快。
 */
#define ARRAY_STATS

/* LOG_CATCHES: define this to cause errors that are catch()'d to be
 *   sent to the debug log anyway.
 *
 * On by default, because newer libs use catch() a lot, and it's confusing
 * if the errors don't show up in the logs.
 */
#define LOG_CATCHES

/* ARGUMENTS_IN_TRACEBACK: prints out function call arguments in error
 *   tracebacks, to aid in debugging.  Note: it prints the values of
 *   the arguments at the time of the error, not when the function
 *   was called.  It looks like this:
 *
 * Failed to load file: read_buffer
 * program: command/update.c, object: command/update line 15
 * '    commandHook' in '        clone/user.c' ('        clone/user#1')line 72
 * arguments were ("/read_buffer.c")
 * '           main' in '    command/update.c' ('      command/update')line 15
 * arguments were ("/read_buffer.c")
 *
 * The only down side is some people like their logs shorter
 */
// #define ARGUMENTS_IN_TRACEBACK


/* LOCALS_IN_TRACEBACK: similar to ARGUMENTS_IN_TRACEBACK, but for local
 *   variables.  The output looks more or less like:
 *
 * locals: 1, "local_value"
 *
 * Same as above.  Tends to produce even longer logs, but very useful for
 * tracking errors.
 */
// #define LOCALS_IN_TRACEBACK

#ifndef LATTICE
#define CONFIG_FILE_DIR "/home/QTZE/script"
#else
#define CONFIG_FILE_DIR "./etc"
#endif

/* DEFAULT_PRAGMAS: 缺省的 PRAGMAS。例如：
 *
 * #define DEFAULT_PRAGMAS PRAGMA_STRICT_TYPES + PRAGMA_SAVE_TYPES
 *
 * 将使每一个 LPC 文件运行起来就象有以下两行一样：
 * #pragma strict_types
 * #pragma save_types
 *
 * 如果没有缺省值，则使用：
 * #define DEFAULT_PRAGMAS 0
 *
 * 如果你不明白它们的意思，0 是最好的选择。
 *
 * 支持的选值：
 * PRAGMA_STRICT_TYPES: 强制严格的类型检查。
 * PRAGMA_WARNINGS:     发现危险的代码时，提出警告。
 * PRAGMA_SAVE_TYPES:   保存函数参数的类型以检查调用函数。
 * PRAGMA_SAVE_BINARY:  保存本文件的编译二进制版本，以便于在需要时能更快载入
 * PRAGMA_OPTIMIZE:     进行第二次扫描以进一步优化代码。
 * PRAGMA_ERROR_CONTEXT:包括一些文本显示行中何处编辑发出错误。
 */
//#define DEFAULT_PRAGMAS PRAGMA_SAVE_BINARY
//#define DEFAULT_PRAGMAS PRAGMA_WARNINGS|PRAGMA_STRICT_TYPES|PRAGMA_SAVE_TYPES|PRAGMA_SAVE_BINARY|PRAGMA_OPTIMIZE|PRAGMA_ERROR_CONTEXT
#define DEFAULT_PRAGMAS PRAGMA_WARNINGS|PRAGMA_STRICT_TYPES|PRAGMA_SAVE_BINARY|PRAGMA_OPTIMIZE|PRAGMA_ERROR_CONTEXT


#define SAVE_EXTENSION ".o"

#define STRIP_BEFORE_PROCESS_INPUT
#define TRAP_CRASHES

#define CALLOUT_HANDLES

/* PROFILE_FUNCTIONS: define this to be able to measure the CPU time used by
 *   all of the user-defined functions in each LPC object.  Note: defining
 *   this adds three long ints (12 bytes on 32-bit machines) to the function
 *   header structs.  Also note that the resolution of the getrusage() timer
 *   may not be high enough on some machines to give non-zero execution
 *   times to very small (fast) functions.  In particular if the clock
 *   resolution is 1/60 of a second, then any time less than approxmately 15k
 *   microseconds will resolve to zero (0).
 */
#undef PROFILE_FUNCTIONS

#define NO_SHADOWS
#define BINARIES
#define NO_ANSI

#undef OPCPROF
#undef OPCPROF_2D
#undef DROP_CORE
#undef THIS_PLAYER_IN_CALL_OUT
#undef FLUSH_OUTPUT_IMMEDIATELY
#undef PRIVS
#undef RESTRICTED_ED
#undef INTERACTIVE_CATCH_TELL
#undef RECEIVE_SNOOP
#undef NO_BUFFER_TYPE
#undef ARRAY_RESERVED_WORD
#undef PACKAGE_QTZ_STATS
#undef PACKAGE_SOCKETS
#undef QTZ_ServerEngineLIB_ERROR_HANDLER

/* HEARTBEAT_INTERVAL: define heartbeat interval in microseconds (us).
 *   1,000,000 us = 1 second.  The value of this macro specifies
 *   the frequency with which the heart_beat method will be called in
 *   those LPC objects which have called set_heart_beat(1).
 *
 * [NOTE: if ualarm() isn't available, alarm() is used instead.  Since
 *  alarm() requires its argument in units of a second, we map 1 - 1,000,000 us
 *  to an actual interval of one (1) second and 1,000,001 - 2,000,000 maps to
 *  an actual interval of two (2) seconds, etc.]
 */
#define HEARTBEAT_INTERVAL 2000000

/* LARGEST_PRINTABLE_STRING: defines the size of the vsprintf() buffer in
 *   comm.c's add_message(). Instead of blindly making this value larger,
 *   QTZ_ServerEnginelib should be coded to not send huge strings to users.
 */
#define LARGEST_PRINTABLE_STRING 10240

/* MESSAGE_BUFFER_SIZE: determines the size of the buffer for output that
 *   is sent to users.
 */
#define MESSAGE_BUFFER_SIZE 4096

/* APPLY_CACHE_BITS: defines the number of bits to use in the call_other cache
 *   (in interpret.c).  Somewhere between six (6) and ten (10) is probably
 *   sufficient for small QTZ_ServerEngines.
 */
#define APPLY_CACHE_BITS 11

/* CACHE_STATS: define this if you want call_other (apply_low) cache 
 * statistics.  Causes HAS_CACHE_STATS to be defined in all LPC objects.
 */
#undef CACHE_STATS

/* RUNTIME_LOADING: On systems which support it, it allows LPC->C compilation
 * 'on the fly' without having to recompile the driver.
 *
 * Note: This currently only works on machines that have the dlopen() system
 * call.  SunOS and IRIX do, as do a number of others.  AIX and Ultrix don't.
 * Linux does if you are using ELF.
 */
#undef RUNTIME_LOADING

/* TRACE_CODE: define this to enable code tracing (the driver will print
 *   out the previous lines of code to an error) eval_instruction() runs about
 *   twice as fast when this is not defined (for the most common eoperators).
 */
#undef TRACE_CODE

/* HEART_BEAT_CHUNK: The number of heart_beat chunks allocated at a time.
 * A large number wastes memory as some will be sitting around unused, while
 * a small one wastes more CPU reallocating when it needs to grow.  Default
 * to a middlish value.
 */
#define HEART_BEAT_CHUNK      32

/* Some maximum string sizes
 */
#define SMALL_STRING_SIZE     100
#define LARGE_STRING_SIZE     1000
#define COMMAND_BUF_SIZE      2000

/* Number of levels of nested datastructures allowed -- this limit prevents
 * crashes from occuring when saving objects containing variables containing
 * recursive datastructures (with circular references).
 */
#define MAX_SAVE_SVALUE_DEPTH 25

/* Miscellaneous config options that should probably be in the runtime
 * config file.
 */
/* MAX_LOCAL: maximum number of local variables allowed per LPC function */
#define CFG_MAX_LOCAL_VARIABLES		100

#define CFG_EVALUATOR_STACK_SIZE 	20000
#define CFG_COMPILER_STACK_SIZE		400
#define CFG_MAX_CALL_DEPTH		50
/* This must be one of 4, 16, 64, 256, 1024, 4096 */
#define CFG_LIVING_HASH_SIZE		256

/* NEXT_MALLOC_DEBUG: define this if using a NeXT and you want to enable
 *   the malloc_check() and/or malloc_debug() efuns.  Run the 'man malloc_debug'
 *   command on the NeXT to find out what the arguments to malloc_debug(int)
 *   mean.  The malloc_check() efun calls the NeXT NXMallocCheck() system
 *   call which does a consistency check on malloc's data structures (this
 *   consistency check is done at each malloc() and free() for certain
 *   malloc_debug() levels).  A non-zero return value indicates there was
 *   a consistency problem.  For those NeXT users wanting a bit more
 *   performance out of malloc, try defining NEXT_MALLOC_DEBUG and calling the
 *   malloc_debug(-1) efun (with an arg of -1).  This will turn all
 *   malloc debugging off and call malloc_singlethreaded() which the NeXT
 *   malloc man page claims can make NeXT system malloc 10% to 15% faster.
 *
 * [NOTE: This #define has no affect on the driver if not using the
 *  NeXTSTEP OS.]
 *
 * Warning: if you use a NeXT and define NEXT_MALLOC_DEBUG, be sure to
 *          protect the use of the malloc_check() and malloc_debug() efuns
 *          since setting certain debug levels can cause malloc() and free()
 *          to become _very_ slow (protect efuns by using simul_efuns and
 *          valid_override).
 *
 * [NOTE: malloc_debug(6) is a good compromise between efficiency and
 *  completeness of malloc debugging (malloc/free will be about half as fast).]
 */
#undef NEXT_MALLOC_DEBUG
#endif
