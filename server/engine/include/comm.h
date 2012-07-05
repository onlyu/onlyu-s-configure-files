/*
 * comm.h -- definitions and prototypes for comm.c
 *
 */

#ifndef COMM_H
	#define COMM_H

	#include "command.h"

	#include <sys/time.h>
	#include <sys/event.h>

	#define CLOSING            64   /* true when closing this file descriptor  */

	typedef struct interactive_s
	{
		object_t *ob; /* points to the associated object         */
		// sentence_t *input_to; /* to be called with next input line       */
		int fd; /* file descriptor for interactive object  */
		int vfd; 
		struct in_addr real_sin_addr;
		struct sockaddr_in addr; /* socket address of interactive object    */
		#ifdef F_QUERY_IP_PORT
			int local_port; /* which of our ports they connected to    */
		#endif 
		int iflags; /* interactive flags */
		int state; /* Current telnet state.  Bingly wop       */
	} interactive_t;

	extern int total_users;

	extern int kq; 
	extern struct kevent *eventlist;

	extern interactive_t **all_users;
	extern int max_users;
#endif /* COMM_H */
