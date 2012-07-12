/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 *
 * Example application source code using the multi socket interface to
 * download many files at once.
 *
 * Written by Jeff Pohlmeyer

Requires libevent and a (POSIX?) system that has mkfifo().

This is an adaptation of libcurl's "hipev.c" and libevent's "event-test.c"
sample programs.

When running, the program creates the named pipe "hiper.fifo"

Whenever there is input into the fifo, the program reads the input as a list
of URL's and creates some new easy handles to fetch each URL via the
curl_multi "hiper" API.


Thus, you can try a single URL:
  % echo http://www.yahoo.com > hiper.fifo

Or a whole bunch of them:
  % cat my-url-list > hiper.fifo

The fifo buffer is handled almost instantly, so you can even add more URL's
while the previous requests are still being downloaded.

Note:
  For the sake of simplicity, URL length is limited to 1023 char's !

This is purely a demo app, all retrieved data is simply discarded by the write
callback.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/poll.h>
#include <curl/curl.h>
#include <event.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "lpc.h"
//#include "macros.h"
#include "lpc_incl.h"
#include "lpc_args_op.h"
#include "fs_lpc_op.h"
#include "fs_curl.h"

#define MSG_OUT stdout /* Send info to stdout, change to stderr if you want */


/* Global information, common to all connections */
typedef struct _GlobalInfo {
  struct event timer_event;
  CURLM *multi;
  int still_running;
} GlobalInfo;

static GlobalInfo *g_info = NULL;

typedef struct {
	void *data;
	size_t data_len;
	size_t data_alloc;
} fs_string_t;

void fs_string_init(fs_string_t *str)
{
	str->data = NULL;
	str->data_len = 0;
	str->data_alloc = 0;
}

void fs_string_deinit(fs_string_t *str)
{
	if (str->data) {
		free(str->data);
	}
}

fs_string_t * fs_string_new()
{
	fs_string_t *str = calloc(1, sizeof(fs_string_t));
	fs_string_init(str);
	return str;
}

void fs_string_free(fs_string_t *str)
{
	fs_string_deinit(str);
	free(str);
}

void fs_string_append(fs_string_t *str, void *data, size_t len)
{
	size_t need = str->data_len + len;
	if (need > str->data_alloc) {
		size_t expandto = str->data_alloc * 2;
		size_t newalloc = expandto > need ? expandto : need;
		str->data = realloc(str->data, newalloc);
		str->data_alloc = newalloc;
	}
	memcpy(str->data + str->data_len, data, len);
	str->data_len += len;
}

/* Information associated with a specific easy handle */
typedef struct _ConnInfo {
  CURL *easy;
  GlobalInfo *global;
  char error[CURL_ERROR_SIZE];

	struct curl_slist *reqheader;
	fs_string_t *header;
	fs_string_t *content;
	svalue_t function;
	svalue_t udata;
} ConnInfo;

static void free_conn(ConnInfo *conn)
{
	curl_multi_remove_handle(g_info->multi, conn->easy);
	curl_easy_cleanup(conn->easy);

	if (conn->reqheader) {
		curl_slist_free_all(conn->reqheader);
	}

	if (conn->header) {
		fs_string_free(conn->header);
	}
	if (conn->content) {	
		fs_string_free(conn->content);
	}
	free_svalue(&conn->function, "curl");
	free_svalue(&conn->udata, "curl");

	free(conn);
}

/* Information associated with a specific socket */
typedef struct _SockInfo {
  curl_socket_t sockfd;
  CURL *easy;
  int action;
  long timeout;
  struct event ev;
  int evset;
  GlobalInfo *global;
} SockInfo;



/* Update the event timer after curl_multi library calls */
static int multi_timer_cb(CURLM *multi, long timeout_ms, GlobalInfo *g)
{
  struct timeval timeout;
  (void)multi; /* unused */

  timeout.tv_sec = timeout_ms/1000;
  timeout.tv_usec = (timeout_ms%1000)*1000;
  fprintf(MSG_OUT, "multi_timer_cb: Setting timeout to %ld ms\n", timeout_ms);
  evtimer_add(&g->timer_event, &timeout);
  return 0;
}

/* Die if we get a bad CURLMcode somewhere */
static void mcode_or_die(const char *where, CURLMcode code)
{
  if ( CURLM_OK != code ) {
    const char *s;
    switch (code) {
      case     CURLM_CALL_MULTI_PERFORM: s="CURLM_CALL_MULTI_PERFORM"; break;
      case     CURLM_BAD_HANDLE:         s="CURLM_BAD_HANDLE";         break;
      case     CURLM_BAD_EASY_HANDLE:    s="CURLM_BAD_EASY_HANDLE";    break;
      case     CURLM_OUT_OF_MEMORY:      s="CURLM_OUT_OF_MEMORY";      break;
      case     CURLM_INTERNAL_ERROR:     s="CURLM_INTERNAL_ERROR";     break;
      case     CURLM_UNKNOWN_OPTION:     s="CURLM_UNKNOWN_OPTION";     break;
      case     CURLM_LAST:               s="CURLM_LAST";               break;
      default: s="CURLM_unknown";
        break;
    case     CURLM_BAD_SOCKET:         s="CURLM_BAD_SOCKET";
      fprintf(MSG_OUT, "ERROR: %s returns %s\n", where, s);
      /* ignore this error */
      return;
    }
    fprintf(MSG_OUT, "ERROR: %s returns %s\n", where, s);
    exit(code);
  }
}



/* Check for completed transfers, and remove their easy handles */
static void check_multi_info(GlobalInfo *g)
{
  char *eff_url;
  CURLMsg *msg;
  int msgs_left;
  ConnInfo *conn;
  CURL *easy;
  CURLcode res;

  fprintf(MSG_OUT, "REMAINING: %d\n", g->still_running);
  while ((msg = curl_multi_info_read(g->multi, &msgs_left))) {
    if (msg->msg == CURLMSG_DONE) {
      easy = msg->easy_handle;
      res = msg->data.result;
      curl_easy_getinfo(easy, CURLINFO_PRIVATE, (char**)&conn);
      curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);
      fprintf(MSG_OUT, "DONE: %s => (%d) %s\n", eff_url, res, conn->error);

	int argc = 4;
	char end = '\0';
	long response_code;
	curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &response_code);
	push_svalue(&conn->udata);
	push_number(response_code);
	fs_string_append(conn->header, &end, sizeof(end));
	fs_string_append(conn->content, &end, sizeof(end));
	copy_and_push_string(conn->header->data);
	copy_and_push_string(conn->content->data);
	safe_call_function_pointer(conn->function.u.fp, argc);
	free_conn(conn);
    }
  }
}



/* Called by libevent when we get action on a multi socket */
static void event_cb(int fd, short kind, void *userp)
{
  GlobalInfo *g = (GlobalInfo*) userp;
  CURLMcode rc;

  int action =
    (kind & EV_READ ? CURL_CSELECT_IN : 0) |
    (kind & EV_WRITE ? CURL_CSELECT_OUT : 0);

  rc = curl_multi_socket_action(g->multi, fd, action, &g->still_running);
  mcode_or_die("event_cb: curl_multi_socket_action", rc);

  check_multi_info(g);
  if ( g->still_running <= 0 ) {
    fprintf(MSG_OUT, "last transfer done, kill timeout\n");
    if (evtimer_pending(&g->timer_event, NULL)) {
      evtimer_del(&g->timer_event);
    }
  }
}



/* Called by libevent when our timeout expires */
static void timer_cb(int fd, short kind, void *userp)
{
  GlobalInfo *g = (GlobalInfo *)userp;
  CURLMcode rc;
  (void)fd;
  (void)kind;

  rc = curl_multi_socket_action(g->multi,
                                  CURL_SOCKET_TIMEOUT, 0, &g->still_running);
  mcode_or_die("timer_cb: curl_multi_socket_action", rc);
  check_multi_info(g);
}



/* Clean up the SockInfo structure */
static void remsock(SockInfo *f)
{
  if (f) {
	close(f->sockfd);
    if (f->evset)
      event_del(&f->ev);
    free(f);
  }
}



/* Assign information to a SockInfo structure */
static void setsock(SockInfo*f, curl_socket_t s, CURL*e, int act, GlobalInfo*g)
{
  int kind =
     (act&CURL_POLL_IN?EV_READ:0)|(act&CURL_POLL_OUT?EV_WRITE:0)|EV_PERSIST;

  f->sockfd = s;
  f->action = act;
  f->easy = e;
  if (f->evset)
    event_del(&f->ev);
  event_set(&f->ev, f->sockfd, kind, event_cb, g);
  f->evset=1;
  event_add(&f->ev, NULL);
}



/* Initialize a new SockInfo structure */
static void addsock(curl_socket_t s, CURL *easy, int action, GlobalInfo *g) {
  SockInfo *fdp = calloc(sizeof(SockInfo), 1);

  fdp->global = g;
  setsock(fdp, s, easy, action, g);
  curl_multi_assign(g->multi, s, fdp);
}

/* CURLMOPT_SOCKETFUNCTION */
static int sock_cb(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp)
{
  GlobalInfo *g = (GlobalInfo*) cbp;
  SockInfo *fdp = (SockInfo*) sockp;
  const char *whatstr[]={ "none", "IN", "OUT", "INOUT", "REMOVE" };

  fprintf(MSG_OUT,
          "socket callback: s=%d e=%p what=%s ", s, e, whatstr[what]);
  if (what == CURL_POLL_REMOVE) {
    fprintf(MSG_OUT, "\n");
    remsock(fdp);
  }
  else {
    if (!fdp) {
      fprintf(MSG_OUT, "Adding data: %s\n", whatstr[what]);
      addsock(s, e, what, g);
    }
    else {
      fprintf(MSG_OUT,
              "Changing action from %s to %s\n",
              whatstr[fdp->action], whatstr[what]);
      setsock(fdp, s, e, what, g);
    }
  }
  return 0;
}


/* CURLOPT_HEADERFUNCTION */
size_t write_header_cb(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  ConnInfo *conn = (ConnInfo*) data;

	fs_string_append(conn->header, ptr, realsize);

  return realsize;
}


/* CURLOPT_WRITEFUNCTION */
static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  ConnInfo *conn = (ConnInfo*) data;
	//todo check max
	fs_string_append(conn->content, ptr, realsize);

  return realsize;
}


/* CURLOPT_PROGRESSFUNCTION */
/*
static int prog_cb (void *p, double dltotal, double dlnow, double ult,
                    double uln)
{
  ConnInfo *conn = (ConnInfo *)p;
  (void)ult;
  (void)uln;

  fprintf(MSG_OUT, "Progress: %s (%g/%g)\n", conn->url, dlnow, dltotal);
  return 0;
}
*/

static int set_opt(ConnInfo *conn, mapping_t *opt)
{
	int i, j;
	mapping_node_t *elt;
	svalue_t *k, *v;

	if (opt->count == 0) {
		return -1;
	}

	for (i = 0; i <= opt->table_size; ++i) {
		for (elt = opt->table[i]; elt != NULL; elt = elt->next) {
			k = elt->values;
			v = elt->values + 1;
			if (k->type != T_STRING) {
				fprintf(stderr, "opt key should be string\n");
				return -1;
			}
			//只简单实现了以下几个opt
			if (strcmp(k->u.string, "CURLOPT_URL") == 0) {
				if (v->type != T_STRING) {
					fprintf(stderr, "CURLOPT_URL should be string\n");
					return -1;
				}
				curl_easy_setopt(conn->easy, CURLOPT_URL, v->u.string);
			} else if (strcmp(k->u.string, "CURLOPT_POSTFIELDS") == 0) {
				if (v->type != T_STRING) {
					fprintf(stderr, "CURLOPT_POSTFIELDS should be string\n");
					return -1;
				}
				curl_easy_setopt(conn->easy, CURLOPT_POSTFIELDS, v->u.string);
				curl_easy_setopt(conn->easy, CURLOPT_POSTFIELDSIZE, SVALUE_STRLEN(v));
			} else if (strcmp(k->u.string, "CURLOPT_HTTPHEADER") == 0) {
				if (v->type != T_ARRAY) {
					fprintf(stderr, "CURLOPT_HTTPHEADER should be array\n");
					return -1;
				}
				struct curl_slist *header = NULL;
				for (j = 0; j < v->u.arr->size; j++) {
					if (v->u.arr->item[j].type != T_STRING) {
						if (header) {
							curl_slist_free_all(header);
						}
						fprintf(stderr, "CURLOPT_HTTPHEADER header should be string\n");
						return -1;
					}
					header = curl_slist_append(header, v->u.arr->item[j].u.string);
				}
				if (header) {
					conn->reqheader = header;
					curl_easy_setopt(conn->easy, CURLOPT_HTTPHEADER, conn->reqheader);
				}
			} else if (strcmp(k->u.string, "CURLOPT_SSL_VERIFYPEER") == 0) {
				if (v->type != T_NUMBER) {
					fprintf(stderr, "CURLOPT_SSL_VERIFYPEER  header should be number\n");
					return -1;
				}
				curl_easy_setopt(conn->easy, CURLOPT_SSL_VERIFYPEER, (long)v->u.number);
			} else if (strcmp(k->u.string, "CURLOPT_SSL_VERIFYHOST") == 0) {
				if (v->type != T_NUMBER) {
					fprintf(stderr, "CURLOPT_SSL_VERIFYHOST  header should be number\n");
					return -1;
				}
				curl_easy_setopt(conn->easy, CURLOPT_SSL_VERIFYHOST, (long)v->u.number);
			}
		}
	}

	return 0;
}

/* Create a new easy handle, and add it to the global curl_multi */
static int new_conn(mapping_t *opt, svalue_t *function, svalue_t *udata)
{
  ConnInfo *conn;
  CURLMcode rc;
  GlobalInfo *g = g_info;

  conn = calloc(1, sizeof(ConnInfo));
  conn->error[0]='\0';

  conn->easy = curl_easy_init();
  if (!conn->easy) {
    fprintf(MSG_OUT, "curl_easy_init() failed, exiting!\n");
    exit(2);
  }
  conn->global = g;
	

  //conn->url = strdup(url);
  //curl_easy_setopt(conn->easy, CURLOPT_URL, conn->url);
  curl_easy_setopt(conn->easy, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(conn->easy, CURLOPT_WRITEDATA, conn);
  curl_easy_setopt(conn->easy, CURLOPT_HEADERFUNCTION, write_header_cb);
	curl_easy_setopt(conn->easy, CURLOPT_HEADERDATA, conn);
  curl_easy_setopt(conn->easy, CURLOPT_ERRORBUFFER, conn->error);
  curl_easy_setopt(conn->easy, CURLOPT_PRIVATE, conn);
  //curl_easy_setopt(conn->easy, CURLOPT_VERBOSE, 1L);
  //curl_easy_setopt(conn->easy, CURLOPT_NOPROGRESS, 0L);
  //curl_easy_setopt(conn->easy, CURLOPT_PROGRESSFUNCTION, prog_cb);
  //curl_easy_setopt(conn->easy, CURLOPT_PROGRESSDATA, conn);
	
	conn->reqheader = NULL;
	conn->function = const0u;
	conn->udata = const0u;
	conn->header = fs_string_new();
	conn->content = fs_string_new();
	assign_svalue(&conn->function, function);
	assign_svalue(&conn->udata, udata);
	if (0 == set_opt(conn, opt)) {
		fprintf(MSG_OUT, "Adding easy %p to multi %p \n", conn->easy, g->multi);
		rc = curl_multi_add_handle(g->multi, conn->easy);
		mcode_or_die("new_conn: curl_multi_add_handle", rc);
		/* note that the add_handle() will set a time-out to trigger very soon so
		that the necessary socket_action() call will be called by this app */
		return 0;
	} else {
		free(conn);
		return -1;
	}
}

int fs_curl_init()
{
	if (g_info == NULL) {
		g_info = calloc(1, sizeof(GlobalInfo));
		g_info->multi = curl_multi_init();
		evtimer_set(&g_info->timer_event, timer_cb, g_info);

		/* setup the generic multi interface options we want */
		curl_multi_setopt(g_info->multi, CURLMOPT_SOCKETFUNCTION, sock_cb);
		curl_multi_setopt(g_info->multi, CURLMOPT_SOCKETDATA, g_info);
		curl_multi_setopt(g_info->multi, CURLMOPT_TIMERFUNCTION, multi_timer_cb);
		curl_multi_setopt(g_info->multi, CURLMOPT_TIMERDATA, g_info);

		/* we don't call any curl_multi_socket*() function yet as we have no handles
		added! */
		return 1;
	} else {
		return 0;
	}
}

void fs_curl_deinit()
{
	if (g_info != NULL) {
		curl_multi_cleanup(g_info->multi);
		free(g_info);
	}
}

void f_fs_curl_request()
{
	int ret;
	SET_ALL_ARGS;
	mapping_t *opt = ARGS(0)->u.map;
	svalue_t *function = ARGS(1);
	svalue_t *udata = ARGS(2);
	ret = new_conn(opt, function, udata);
	POP_ALL_ARGS;
	push_number(ret);
}

