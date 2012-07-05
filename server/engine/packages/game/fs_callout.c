// 定时器
// ----------------------
// 使用kern_timeout.c提供的
// 定时器调度机制
// ----------------------
#include "lpc_incl.h"
#include "eoperators.h"

#include "lpc_args_op.h"

#include "timeout.h"
#include "fs_callout.h"
#include "fs_main.h"


// 心跳的最小间隔
static long interval = 0;
static char inited = 0;


extern int current_time;
extern int get_current_time();

fs_pending_callout_t* all_callout = NULL;

// call_out管理，以便remove
static struct timeval last_update_time; 

// 最大补帧数为1
#define MAX_ADD_TICK (1)

void update_clock(void* arg)
{
	// gettimeofday
	struct timeval now;
	long delta;
	int addTick;

	// 更新当前时间
	current_time = get_current_time();

	gettimeofday(&now, NULL );

	delta = 1000000*(now.tv_sec - last_update_time.tv_sec) + (now.tv_usec - last_update_time.tv_usec);
	addTick = 0;

	// 保证最多补一帧，防止出问题
	while( delta > interval && addTick <= MAX_ADD_TICK)
	{ 
		// 判断是否有timeout要执行
		if ( timeout_hardclock_update() )
		{
			softclock();
		}
		
		last_update_time.tv_sec += (interval/1000000);
		last_update_time.tv_usec += (interval%1000000);
		
		delta -= interval;

		addTick ++;
	}

	// 如果补帧之后,还是差别太大，直接将last_update_time，设置到当前时间
	if(delta > interval)
	{
		last_update_time.tv_sec = now.tv_sec;
		last_update_time.tv_usec = now.tv_usec;
		
		debug_message("WARNING:frame interval is[%ld]us, big than a tick.\n", delta);
	}
}

int callout_init( int sec, int usec)
{
	struct timeval period;

	if (inited) 
	{
		debug_message( "WARING:callout system is be initialized.\n" );
		put_number(0);
		return 0;
	}
	
	timeout_startup();

	period.tv_sec = sec;
	period.tv_usec = usec;

	// 心跳间隔
	interval = ((long)sec) * 1000000 + ((long)usec);

	gettimeofday(&last_update_time, NULL );

	// 
	add_timer(period, update_clock, (void *)NULL);

	debug_message("system is be initialized: interval=[%ld]us\n", interval);
	return 1;
}

// 初始化一个call_out
fs_pending_callout_t * new_call()
{
	fs_pending_callout_t * ret;

	ret = MALLOC( sizeof( fs_pending_callout_t ));
	
	ret->del_flg = 0;

	return ret;
}

/*
 * Free a call out structure.
 */
static void free_called_call (fs_pending_callout_t*  call_out)
{

	if (call_out->ob) 
	{
		free_string(call_out->function.s);
		free_object(call_out->ob, "free_call");
	} 
	else 
	{
		free_funp(call_out->function.f);
	}

	call_out->function.s = 0;
	call_out->ob = 0;

	FREE( call_out );
}

void free_call (fs_pending_callout_t*  call_out)
{
	if (call_out->vs)
	{
		free_array(call_out->vs);
		call_out->vs = NULL;
	}
	free_called_call(call_out);
}

// 初始化一个timeout
struct timeout * new_timeout()
{
	struct timeout * data;
	data = (struct timeout *)MALLOC(sizeof(struct timeout));

	bzero( data, sizeof(struct timeout) );
	return data;
}

// 刪除一個timeout
void free_timeout(struct timeout * data)
{
	FREE( data );
}

extern int eval_cost;
extern int max_cost;

// 執行一個call_out
static void run_lpc_func( struct timeout * timeout, void * args )
{
	fs_pending_callout_t * call_out;
	error_context_t econ;
	
	call_out = (fs_pending_callout_t *)args;

	// 目标call_out被删除
	if ( call_out->del_flg)
	{
		// 从列表中释放
		HASH_DEL(all_callout, call_out);

		free_call( call_out );
		free_timeout(timeout);
		return;
	}

	// 對象被Destructed
	if ( call_out->ob && call_out->ob->flags & O_DESTRUCTED)
	{
		// 从列表中释放
		HASH_DEL(all_callout, call_out);

		free_call( call_out );
		free_timeout(timeout);
		return;
	}

	// 
	if (SETJMP(econ.context)) 
	{
		restore_context(&econ);
	} 
	else 
	{
		object_t *ob;
		int extra;

		ob = call_out->ob;

		command_giver = 0;

		// 有参数
		if (call_out->vs) 
		{
			array_t *vec = call_out->vs;
			// 將svp指向數組的最後一個元素
			svalue_t *svp = vec->item + vec->size;

			// 校驗數組中的對象都有效
			while (svp-- > vec->item) 
			{
				if (svp->type == T_OBJECT && (svp->u.ob->flags & O_DESTRUCTED)) 
				{
					free_object(svp->u.ob, "call_out");
					*svp = const0;
				}
			}

			/* cop->vs is ref one */
			extra = call_out->vs->size;
			transfer_push_some_svalues(call_out->vs->item, extra);
			// 释放参数列表 
			free_empty_array(call_out->vs);
			call_out->vs = NULL;
		} 
		else
		{
			extra = 0;
		}

		// 重置eval_cost
		eval_cost = max_cost;

		if (call_out->ob) 
		{
			// 如果是用函數名做調用
			// 校验参数名是否正确 
			if (call_out->function.s[0] == APPLY___INIT_SPECIAL_CHAR)
			{
				error("The function name is illegal!\n");
			}

			safe_apply(call_out->function.s, call_out->ob, extra, ORIGIN_CALL_OUT);
		} 
		else 
		{
			safe_call_function_pointer(call_out->function.f, extra);
		}
	}

	// 从列表中释放
	HASH_DEL(all_callout, call_out);

	// 释放 
	free_called_call(call_out);
	timeout->to_arg = NULL;
	free_timeout(timeout);
}

// 自增的call_out_handler
static int call_out_handler = 1;
// int call_out( 1, "", ... );
// int call_out( 1, (::), ... );
void f_call_out()
{
	// 取出参数
	svalue_t *arg = sp - st_num_arg + 1;
	int num = st_num_arg - 2;
	struct timeout * data;
	int ret, ticks;
	fs_pending_callout_t * call_out;

	// 校验参数

	// 当前对象没有被销毁
	if (!(current_object->flags & O_DESTRUCTED)) 
	{
		svalue_t *  fun;
		int delay; 

		call_out = new_call();
		if ( call_out == NULL)
		{
			// 内存分配失败
			debug_message( "ERROR:callout memory allocate failed\n" );
			// TODO:是否exit?????

			// 计算栈信息
			sp -= num + 1;

			free_svalue(sp, "call_out");
			put_number(0);
			return;
		}

		data = new_timeout();
		if ( data == NULL)
		{
			// 内存分配失败
			debug_message( "ERROR:timeout memory allocate failed\n" );
			// TODO:是否exit?????

			// 计算栈信息
			sp -= num + 1;

			free_svalue(sp, "call_out");
			put_number(0);
			return;
		}

		// 设值
		fun = &arg[0];

		if (fun->type == T_STRING)
		{
			call_out->function.s = make_shared_string(fun->u.string);
			call_out->ob = current_object;
			add_ref(current_object, "call_out");
		}
		else
		{
			call_out->function.f = fun->u.fp;
			fun->u.fp->hdr.ref++;
			call_out->ob = NULL;
		}

		// 构建参数
		if (num > 0) 
		{
			call_out->vs = allocate_empty_array(num);
			memcpy(call_out->vs->item, arg + 2, sizeof(svalue_t) * num);
		} 
		else
		{
			call_out->vs = NULL;
		}

		delay = arg[1].u.number;

		// 计算心跳ticks
		ticks = (int)((long)delay * 1000000 / interval); 

		data->to_arg = call_out;

		// 赋值
		ret = call_out_handler ++;

		call_out->handler = ret;

		// 
		HASH_ADD_INT(all_callout, handler, call_out);

		_timeout_set(data, &run_lpc_func, call_out);
		_timeout_add(data, ticks);
		
		sp -= num + 1;
	} 
	else 
	{
		// 对象被销毁，返回0
		ret = 0;
		pop_n_elems(num);
		sp--;
	}

	/* the function */
	free_svalue(sp, "call_out");
	put_number(ret);
	return;
}

void f_remove_call_out( )
{
	SET_ALL_ARGS
	int handler = ARGS(0)->u.number;
	fs_pending_callout_t* callout; 
	POP_ALL_ARGS	

	HASH_FIND_INT( all_callout, &handler, callout );

	if ( callout == NULL )
	{
		push_number(-1);
		return;
	}

	// 删除
	callout->del_flg = 1;

	//  
	push_number(1);
}
// ------------------------------------------------------
