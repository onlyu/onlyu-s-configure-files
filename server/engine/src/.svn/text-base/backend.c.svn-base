#include "std.h"
#include "lpc_incl.h"
#include "backend.h"
#include "comm.h"
#include "replace_program.h"
#include "debug.h"
#include "swap.h"
#include "port.h"
#include "lint.h"
#include <sys/event.h>

#ifdef WIN32
	#include <process.h>
	void CDECL alarm_loop(void*);
#endif 

error_context_t *current_error_context = 0;

/*
 * The 'current_time' is updated at every heart beat.
 */
int current_time;

int heart_beat_flag = 0;
object_t *current_heart_beat;

int internal_server = 0;
//static void look_for_objects_to_swap (void);

/*
 * There are global variables that must be zeroed before any execution.
 * In case of errors, there will be a LONGJMP(), and the variables will
 * have to be cleared explicitely. They are normally maintained by the
 * code that use them.
 *
 * This routine must only be called from top level, not from inside
 * stack machine execution (as stack will be cleared).
 */
void clear_state()
{
	current_object = 0;
	command_giver = 0;
	current_interactive = 0;
	previous_ob = 0;
	current_prog = 0;
	caller_type = 0;
	reset_machine(0); /* Pop down the stack. */
} /* clear_state() */

void logon(object_t *ob)
{
	/* current_object no longer set */
	apply(APPLY_LOGON, ob, 0, ORIGIN_DRIVER);
	/* function not existing is no longer fatal */
}


/*
 * This is the backend. We will stay here for ever (almost).
 */
int eval_cost;
int forbid_login;
int forbid_login_flag;

/* Call all heart_beat() functions in all objects.  Also call the next reset,
 * and the call out.
 * We do heart beats by moving each object done to the end of the heart beat
 * list before we call its function, and always using the item at the head
 * of the list as our function to call.  We keep calling heart beats until
 * a timeout or we have done num_heart_objs calls.  It is done this way so
 * that objects can delete heart beating objects from the list from within
 * their heart beat without truncating the current round of heart beats.
 *
 * Set command_giver to current_object if it is a living object. If the object
 * is shadowed, check the shadowed object if living. There is no need to save
 * the value of the command_giver, as the caller resets it to 0 anyway.  */

typedef struct
{
	object_t *ob;
	short heart_beat_ticks;
	short time_to_heart_beat;
} heart_beat_t;

static heart_beat_t *heart_beats = 0;
static int max_heart_beats = 0;
static int heart_beat_index = 0;
static int num_hb_objs = 0;
static int num_hb_to_do = 0;

static int num_hb_calls = 0; /* starts */
static float perc_hb_probes = 100.0; /* decaying avge of how many complete */

int query_heart_beat(object_t *ob)
{
	int index;

	if (!(ob->flags &O_HEART_BEAT) )
	{
		return 0;
	}
	index = num_hb_objs;
	while (index--)
	{
		if (heart_beats[index].ob == ob)
		{
			return heart_beats[index].time_to_heart_beat;
		}
	}
	return 0;
} /* query_heart_beat() */

/* add or remove an object from the heart beat list; does the major check...
 * If an object removes something from the list from within a heart beat,
 * various pointers in call_heart_beat could be stuffed, so we must
 * check current_heart_beat and adjust pointers.  */

int set_heart_beat(object_t *ob, int to)
{
	int index;

	if (ob->flags &O_DESTRUCTED)
	{
		return 0;
	}

	if (!to)
	{
		int num;

		index = num_hb_objs;
		while (index--)
		{
			if (heart_beats[index].ob == ob)
			{
				break;
			}
		}
		if (index < 0)
		{
			return 0;
		}

		if (num_hb_to_do)
		{
			if (index <= heart_beat_index)
			{
				heart_beat_index--;
			}
			if (index < num_hb_to_do)
			{
				num_hb_to_do--;
			}
		}

		if ( (num = (num_hb_objs - (index + 1) ) ) )
		{
			memmove(heart_beats + index, heart_beats + (index + 1), num *sizeof(heart_beat_t) );
		}

		num_hb_objs--;
		ob->flags &= ~O_HEART_BEAT;
		return 1;
	}

	if (ob->flags &O_HEART_BEAT)
	{
		if (to < 0)
		{
			return 0;
		}

		index = num_hb_objs;
		while (index--)
		{
			if (heart_beats[index].ob == ob)
			{
				heart_beats[index].time_to_heart_beat = heart_beats[index].heart_beat_ticks = to;
				break;
			}
		}
		DEBUG_CHECK(index < 0, "In heart_beat list couldn't find enabled object!\n");
	}
	else
	{
		heart_beat_t *hb;

		if (!max_heart_beats)
		{
			heart_beats = CALLOCATE(max_heart_beats = HEART_BEAT_CHUNK, heart_beat_t, TAG_HEART_BEAT, "set_heart_beat: 1");
		}
		else if (num_hb_objs == max_heart_beats)
		{
			max_heart_beats += HEART_BEAT_CHUNK;
			heart_beats = RESIZE(heart_beats, max_heart_beats, heart_beat_t, TAG_HEART_BEAT, "set_heart_beat: 1");
		}

		hb = &heart_beats[num_hb_objs++];
		hb->ob = ob;
		if (to < 0)
		{
			to = 1;
		}
		hb->time_to_heart_beat = to;
		hb->heart_beat_ticks = to;
		ob->flags |= O_HEART_BEAT;
	}

	return 1;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

int heart_beat_status(outbuffer_t *ob, int verbose)
{
	char buf[20];

	if (verbose == 1)
	{
		outbuf_add(ob, "**The information of object's heartbeat:**\n");
		outbuf_add(ob, "*************************\n");
		outbuf_addv(ob, "Number of heartbeat objects: %d, starts: %d\n", num_hb_objs, num_hb_calls);

		/* passing floats to varargs isn't highly portable so let sprintf
		handle it */
		sprintf(buf, "%.2f", perc_hb_probes);
		outbuf_addv(ob, "Percentage of HB: %s\n", buf);
	}
	return (0);
} /* heart_beat_status() */

/* All destructed objects are moved into a sperate linked list,
 * and deallocated after program execution.  */
INLINE void remove_destructed_objects()
{
	object_t *ob,  *next;

	if (obj_list_replace)
	{
		replace_programs();
	}
	for (ob = obj_list_destruct; ob; ob = next)
	{
		next = ob->next_all;
		destruct2(ob);
	}
	obj_list_destruct = NULL;
} /* remove_destructed_objects() */

static double load_av = 0.0;

void update_load_av()
{
	static int last_time;
	int n;
	double c;
	static int acc = 0;

	acc++;
	if (current_time == last_time)
	{
		return ;
	}
	n = current_time - last_time;
	if (n < NUM_CONSTS)
	{
		c = consts[n];
	}
	else
	{
		c = exp( - n / 900.0);
	}
	load_av = c * load_av + acc *(1-c) / n;
	last_time = current_time;
	acc = 0;
} /* update_load_av() */

static double compile_av = 0.0;

void update_compile_av(int lines)
{
	static int last_time;
	int n;
	double c;
	static int acc = 0;

	acc += lines;
	if (current_time == last_time)
	{
		return ;
	}
	n = current_time - last_time;
	if (n < NUM_CONSTS)
	{
		c = consts[n];
	}
	else
	{
		c = exp( - n / 900.0);
	}
	compile_av = c * compile_av + acc *(1-c) / n;
	last_time = current_time;
	acc = 0;
} /* update_compile_av() */

char *query_load_av()
{
	static char buff[100];
	sprintf(buff, "%.2f cmds/s, %.2f lines/s", load_av, compile_av);
	return (buff);
} /* query_load_av() */

#ifdef F_HEART_BEATS
	array_t *get_heart_beats()
	{
		int n = num_hb_objs;
		heart_beat_t *hb = heart_beats;
		array_t *arr;

		arr = allocate_empty_array(n);
		while (n--)
		{
			arr->item[n].type = T_OBJECT;
			arr->item[n].u.ob = hb->ob;
			add_ref(hb->ob, "get_heart_beats");
			hb++;
		}
		return arr;
	}
#endif
