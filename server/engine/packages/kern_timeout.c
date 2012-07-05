/*	$OpenBSD: kern_timeout.c,v 1.25 2006/07/19 20:25:08 miod Exp $	*/
/*
 * Copyright (c) 2001 Thomas Nordin <nordin@openbsd.org>
 * Copyright (c) 2000-2001 Artur Grabowski <art@openbsd.org>
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */
#include <stdlib.h>

#include "timeout.h"
#include "queue.h"

/*
 * Timeouts are kept in a hierarchical timing wheel. The to_time is the value
 * of the global variable "ticks" when the timeout should be called. There are
 * four levels with 256 buckets each. See 'Scheme 7' in
 * "Hashed and Hierarchical Timing Wheels: Efficient Data Structures for
 * Implementing a Timer Facility" by George Varghese and Tony Lauck.
 */
#define BUCKETS 1024
#define WHEELSIZE 256
#define WHEELMASK 255
#define WHEELBITS 8

struct circq timeout_wheel[BUCKETS];	/* Queues of timeouts */
struct circq timeout_todo;		/* Worklist */

#define MASKWHEEL(wheel, time) (((time) >> ((wheel)*WHEELBITS)) & WHEELMASK)

#define BUCKET(rel, abs)						\
    (timeout_wheel[							\
	((rel) <= (1 << (2*WHEELBITS)))				\
	    ? ((rel) <= (1 << WHEELBITS))				\
		? MASKWHEEL(0, (abs))					\
		: MASKWHEEL(1, (abs)) + WHEELSIZE			\
	    : ((rel) <= (1 << (3*WHEELBITS)))				\
		? MASKWHEEL(2, (abs)) + 2*WHEELSIZE			\
		: MASKWHEEL(3, (abs)) + 3*WHEELSIZE])

#define MOVEBUCKET(wheel, time)						\
    CIRCQ_APPEND(&timeout_todo,						\
        &timeout_wheel[MASKWHEEL((wheel), (time)) + (wheel)*WHEELSIZE])

/*
 * Circular queue definitions.
 */

#define CIRCQ_INIT(elem) do {                   \
        (elem)->next = (elem);                  \
        (elem)->prev = (elem);                  \
} while (0)

#define CIRCQ_INSERT(elem, list) do {           \
        (elem)->prev = (list)->prev;            \
        (elem)->next = (list);                  \
        (list)->prev->next = (elem);            \
        (list)->prev = (elem);                  \
} while (0)

#define CIRCQ_APPEND(fst, snd) do {             \
        if (!CIRCQ_EMPTY(snd)) {                \
                (fst)->prev->next = (snd)->next;\
                (snd)->next->prev = (fst)->prev;\
                (snd)->prev->next = (fst);      \
                (fst)->prev = (snd)->prev;      \
                CIRCQ_INIT(snd);                \
        }                                       \
} while (0)

#define CIRCQ_REMOVE(elem) do {                 \
        (elem)->next->prev = (elem)->prev;      \
        (elem)->prev->next = (elem)->next;      \
	_Q_INVALIDATE((elem)->prev);		\
	_Q_INVALIDATE((elem)->next);		\
} while (0)

#define CIRCQ_FIRST(elem) ((elem)->next)

#define CIRCQ_EMPTY(elem) (CIRCQ_FIRST(elem) == (elem))

/*
 * Some of the "math" in here is a bit tricky.
 *
 * We have to beware of wrapping ints.
 * We use the fact that any element added to the queue must be added with a
 * positive time. That means that any element `to' on the queue cannot be
 * scheduled to timeout further in time than INT_MAX, but to->to_time can
 * be positive or negative so comparing it with anything is dangerous.
 * The only way we can use the to->to_time value in any predictable way
 * is when we calculate how far in the future `to' will timeout -
 * "to->to_time - ticks". The result will always be positive for future
 * timeouts and 0 or negative for due timeouts.
 */
int ticks = 0;		

void
timeout_startup(void)
{
	int b;

	CIRCQ_INIT(&timeout_todo);
	for (b = 0; b < BUCKETS; b++)
		CIRCQ_INIT(&timeout_wheel[b]);
}

void _timeout_set(struct timeout *new, void (*fn)(struct timeout *, void *), void *arg)
{
	new->to_func = fn;
	new->to_arg = arg;
	// 设置timeout* new的状态为初始化完毕
	new->to_flags = TIMEOUT_INITIALIZED;
}


void _timeout_add(struct timeout *new, int to_ticks)
{
	int old_time;

#ifdef DIAGNOSTIC
	if (!(new->to_flags & TIMEOUT_INITIALIZED))
		panic("timeout_add: not initialized");
	if (to_ticks < 0)
		panic("timeout_add: to_ticks (%d) < 0", to_ticks);
#endif

	/* Initialize the time here, it won't change. */
	old_time = new->to_time;

	// 设置执行时间
	new->to_time = to_ticks + ticks;

	// new有可能已经执行过，清它的触发标记
	new->to_flags &= ~TIMEOUT_TRIGGERED;

	/*
	 * If this timeout already is scheduled and now is moved
	 * earlier, reschedule it now. Otherwise leave it in place
	 * and let it be rescheduled later.
	 */
	if (new->to_flags & TIMEOUT_ONQUEUE) {
		// timeout 已经存在于 queue 中，此时将 timeout 的时间提前了
		// 则立即 re-arrange 'struct timeout new' 的位置
		if (new->to_time - ticks < old_time - ticks) {
			CIRCQ_REMOVE(&new->to_list);
			CIRCQ_INSERT(&new->to_list, &timeout_todo);
		}
	} else {
		new->to_flags |= TIMEOUT_ONQUEUE;
		CIRCQ_INSERT(&new->to_list, &timeout_todo);
	
	}
}

void _timeout_del(struct timeout *to)
{
	if (to->to_flags & TIMEOUT_ONQUEUE) {
		CIRCQ_REMOVE(&to->to_list);
		to->to_flags &= ~TIMEOUT_ONQUEUE;
	}
	to->to_flags &= ~TIMEOUT_TRIGGERED;
}

/*
 * This is called from hardclock() once every tick.
 * We return !0 if we need to schedule a softclock.
 */
int
timeout_hardclock_update(void)
{
	int ret;


	ticks++;

	MOVEBUCKET(0, ticks);
	if (MASKWHEEL(0, ticks) == 0) {
		MOVEBUCKET(1, ticks);
		if (MASKWHEEL(1, ticks) == 0) {
			MOVEBUCKET(2, ticks);
			if (MASKWHEEL(2, ticks) == 0)
				MOVEBUCKET(3, ticks);
		}
	}

	ret = !CIRCQ_EMPTY(&timeout_todo);

	return (ret);
}

extern int current_time; 
void softclock(void)
{
	struct timeout *to;
	void (*fn)(struct timeout *, void *);
	void *arg;

	while (!CIRCQ_EMPTY(&timeout_todo)) {

		to = (struct timeout *)CIRCQ_FIRST(&timeout_todo); /* XXX */


		CIRCQ_REMOVE(&to->to_list);

		/* If due run it, otherwise insert it into the right bucket. */
		if (to->to_time - ticks > 0) {
			CIRCQ_INSERT(&to->to_list,
			    &BUCKET((to->to_time - ticks), to->to_time));
		} else {
#ifdef DEBUG
			if (to->to_time - ticks < 0)
				printf("timeout delayed %d\n", to->to_time -
				    ticks);
#endif
			to->to_flags &= ~TIMEOUT_ONQUEUE;
			to->to_flags |= TIMEOUT_TRIGGERED;

			fn = to->to_func;
			arg = to->to_arg;

			fn(to, arg);

			//這裡不釋放，因為to和arg是外面生成的，這裡不能管理釋放
			//释放 to和arg
			//free(to);
			//free(arg);
		}
	}
}

