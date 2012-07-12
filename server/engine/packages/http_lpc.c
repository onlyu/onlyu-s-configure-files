/*
 * Compile with:
 * cc -I/usr/local/include -o event-test event-test.c -L/usr/local/lib -levent
 */

#include <sys/queue.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "escape.h"
#include "lpc.h"
//#include "macros.h"
#include "lpc_incl.h"
#include "lpc_args_op.h"
#include "fs_lpc_op.h"

#include "http_lpc.h"

typedef struct fs_http_lpc_s {
	enum evhttp_cmd_type method;
	struct evhttp_connection *evcon;
	struct evhttp_request *req;

	svalue_t function;
	svalue_t arg;
} fs_http_lpc_t;


static void http_req_done_cb(struct evhttp_request *req, void *arg);

inline static void fs_http_lpc_comm_free(fs_http_lpc_t *http)
{
	free_svalue(&http->function, "http");
	free_svalue(&http->arg, "http");
	free(http);
}

//由于free  connection 的时候会 free request, 为了安全，这个free函数只在成功处理http request之后调用
static void fs_http_lpc_free(fs_http_lpc_t *http)
{
	//req belong to evcon after make req
	evhttp_connection_free(http->evcon);
	fs_http_lpc_comm_free(http);
}

static void fs_http_lpc_error_free(fs_http_lpc_t *http)
{
	if (http->req != NULL)
		evhttp_request_free(http->req);
	if (http->evcon != NULL) 
		evhttp_connection_free(http->evcon);
	fs_http_lpc_comm_free(http);
}

static fs_http_lpc_t *fs_http_lpc_new(char *host, unsigned short port, enum evhttp_cmd_type method, 
		svalue_t *function, svalue_t *arg)
{
	fs_http_lpc_t *http = calloc(1, sizeof(*http));
	http->method = method;
	assign_svalue(&http->function, function);
	assign_svalue(&http->arg, arg);

	http->evcon = evhttp_connection_new(host, port);
	if (http->evcon == NULL) 
		goto error;

	http->req = evhttp_request_new(http_req_done_cb, http);
	if (http->req == NULL) 
		goto error;

	return http;
error:
	fs_http_lpc_error_free(http);
	return NULL;
}

static int http_add_header(fs_http_lpc_t *http, mapping_t *header)
{
	int i;
	mapping_node_t *elt;
	svalue_t *k, *v;

	for (i = 0; i <= header->table_size; ++i) {
		for (elt = header->table[i]; elt != NULL; elt = elt->next) {
			k = elt->values;
			v = elt->values + 1;
			if (k->type != T_STRING || v->type != T_STRING)
				return -1;
			evhttp_add_header(http->req->output_headers, k->u.string, v->u.string);
		}
	} 

	return 0;
}

static struct evbuffer *encode_param(mapping_t *params)
{
	int i, first;
	mapping_node_t *elt;
	svalue_t *k, *v;
	char *encode_k, *encode_v;

	struct evbuffer *buf = evbuffer_new();
	for (first = 1, i = 0; i <= params->table_size; ++i) {
		for (elt = params->table[i]; elt != NULL; elt = elt->next) {
			k = elt->values;
			v = elt->values + 1;
			if (k->type != T_STRING || v->type != T_STRING) {
				evbuffer_free(buf);
				return NULL;
			}

			if (first) 
				first = 0;
			else 
				evbuffer_add(buf, "&", 1);

			encode_k = url_easy_escape(k->u.string, 0);
			encode_v = url_easy_escape(v->u.string, 0);
			evbuffer_add(buf, encode_k, strlen(encode_k));
			evbuffer_add(buf, "=", 1);
			evbuffer_add(buf, encode_v, strlen(encode_v));
			free(encode_k);
			free(encode_v);
		}
	} 

	return buf;
}

/*
static int http_add_params(fs_http_lpc_t *http, enum evhttp_cmd_type method, char *uri, mapping_t *params)
{
	struct evbuffer *data = encode_param(params);
	if (data == NULL)
		return -1;

	if (http->uri != NULL) {
		free(http->uri);
	}

	if (method == EVHTTP_REQ_GET) {
		size_t urisz = strlen(uri);
		http->uri = calloc(1, urisz + EVBUFFER_LENGTH(data)); //'\0' included in the data
		memcpy(http->uri, uri, urisz);
		memcpy(http->uri + urisz, EVBUFFER_DATA(data), EVBUFFER_LENGTH(data));
	} else {// method == EVHTTP_REQ_POST
		http->uri = strdup(uri);
		evbuffer_add_buffer(http->req->output_buffer, data);
	}

	printf("\ndump uri:%s, data:%s\n", http->uri, EVBUFFER_DATA(http->req->output_buffer));
	evbuffer_free(data);
	return 0;
}
*/

static void http_add_post_data(fs_http_lpc_t *http, char *data, size_t size)
{
	if (data == NULL || size == 0)
		return;

	evbuffer_add(http->req->output_buffer, data, size);
}

typedef struct fs_http_uri_s {
	char *host;
	unsigned short port;
	char *path;
} fs_http_uri_t;

typedef struct fs_http_req_info_s {
	enum evhttp_cmd_type method;
	mapping_t *header;
	char *post;
	size_t post_len;
} fs_http_req_info_t;

typedef struct fs_http_handler_s {
	svalue_t *function;
	svalue_t *arg;
} fs_http_handler_t;

static int http_request(fs_http_uri_t *uri,  fs_http_req_info_t *req_info, fs_http_handler_t *handler)
{
	fs_http_lpc_t *http = fs_http_lpc_new(uri->host, uri->port, req_info->method, handler->function, handler->arg);
	if (http == NULL) 
		return -1;

	if (http_add_header(http, req_info->header))
		goto error;

	if (req_info->method == EVHTTP_REQ_POST)
		http_add_post_data(http, req_info->post, req_info->post_len);

	//似乎libevent对head的处理有问题，所以用get的方式来模拟head.
	enum evhttp_cmd_type method = req_info->method == EVHTTP_REQ_HEAD ? EVHTTP_REQ_GET: req_info->method;
	if (evhttp_make_request(http->evcon, http->req, method, uri->path)) {
		//从libevent的代码来看，不会跑到这个错误逻辑，即使跑到了似乎也应该调用error_free.
		//但是在安全和内存泄漏选择，当然要选安全。为什么？因为安全第一。
		fs_http_lpc_free(http);
		return -1;
	}

	return 0;
error:
	fs_http_lpc_error_free(http);
	return -1;
}

static mapping_t *http_dump_header(fs_http_lpc_t *http)
{
	char *str;
        struct evkeyval *header;
	svalue_t *v;
	svalue_t key = const0u;
	svalue_t value = const0u;
	mapping_t *m = allocate_mapping(1);

        TAILQ_FOREACH(header, http->req->input_headers, next) {
		str = string_copy(header->key, "http dump header");
		set_svalue_malloced_string(&key, str);
		str = string_copy(header->value, "http dump header");
		set_svalue_malloced_string(&value, str);

		v = find_for_insert(m, &key, 0); 
		assign_svalue(v, &value);

		free_svalue(&key, "http dump header");
		free_svalue(&value, "http dump header");
        }    

	return m;
}

static void http_req_done_cb(struct evhttp_request *req, void *arg)
{
	fs_http_lpc_t *http = arg;
	int argc = 3;
	/*
	if (http->req != req) //warning, 连接后如果对方不返回数据，超时， req 参数会是 NULL
		fprintf(stderr, "http warning:http->req != req.http->req=%p,req=%p\n", http->req, req);
	*/

	//arg
	push_svalue(&http->arg);
	//code
	push_number(http->req->response_code);

	//response
	if (http->method == EVHTTP_REQ_HEAD) {
		mapping_t *m = http_dump_header(http);
		push_refed_mapping(m);
	} else {
		// http response
		//do not use the copy_and_push_string, we must check the data len. may be the response is  a binary string.
		//we only accept the char string, lpc buffer supports binary string but lpc string not.
		// we force the last char to be '\0', if the response is not char string, there is a error.
		char end = '\0';
		evbuffer_add(http->req->input_buffer, &end, sizeof(end));
		char *str = (char*)EVBUFFER_DATA(http->req->input_buffer);
		copy_and_push_string(str);
	}

	safe_call_function_pointer(http->function.u.fp, argc);

	fs_http_lpc_free(http);
}

#define FS_HTTP_METHOD_GET 0
#define FS_HTTP_METHOD_POST 1
#define FS_HTTP_METHOD_HEAD 2

#define FS_HTTP_ERROR_URI -2
#define FS_HTTP_ERROR_REQ_INFO -3
#define FS_HTTP_ERROR_HANDLER -4


inline static int uri_init(fs_http_uri_t *uri, array_t *arg)
{
	#define FS_URI_HOST_IDX 0
	#define FS_URI_PORT_IDX 1
	#define FS_URI_PATH_IDX 2

	if (arg->size != 3) {
		goto error;
	}
	if (arg->item[FS_URI_HOST_IDX].type != T_STRING) {
		goto error;
	}
	if (arg->item[FS_URI_PORT_IDX].type != T_NUMBER) {
		goto error;
	}
	if (arg->item[FS_URI_PATH_IDX].type != T_STRING) {
		goto error;
	}

	uri->host = arg->item[FS_URI_HOST_IDX].u.string;
	uri->port = arg->item[FS_URI_PORT_IDX].u.number;
	uri->path = arg->item[FS_URI_PATH_IDX].u.string;

	return 0;
error:
	return FS_HTTP_ERROR_URI;
}

inline static int req_info_init(fs_http_req_info_t *req_info, array_t *arg)
{
	#define FS_REQ_INFO_METHOD_IDX 0
	#define FS_REQ_INFO_HEADER_IDX 1
	#define FS_REQ_INFO_POST_IDX 2

	if (arg->size != 2 && arg->size != 3) {
		goto error;
	}
	if (arg->item[FS_REQ_INFO_METHOD_IDX].type != T_NUMBER) {
		goto error;
	}
	if (arg->item[FS_REQ_INFO_HEADER_IDX].type != T_MAPPING) {
		goto error;
	}

	int method = arg->item[FS_REQ_INFO_METHOD_IDX].u.number;
	switch (method) {
		case FS_HTTP_METHOD_HEAD:
			req_info->method =  EVHTTP_REQ_HEAD;
			break;
		case FS_HTTP_METHOD_POST:
			req_info->method =  EVHTTP_REQ_POST;
			break;
		default:
			req_info->method =  EVHTTP_REQ_GET;
	}
			
	req_info->header = arg->item[FS_REQ_INFO_HEADER_IDX].u.map;

	if (arg->size == 3) {
		if (arg->item[FS_REQ_INFO_POST_IDX].type != T_STRING) {
			goto error;
		}
		svalue_t *post = &arg->item[FS_REQ_INFO_POST_IDX];
		req_info->post = post->u.string;
		req_info->post_len = SVALUE_STRLEN(post);
	} else {
		req_info->post = NULL;
		req_info->post_len = 0;
	}

	return 0;
error:
	return FS_HTTP_ERROR_REQ_INFO;
}

inline static int handler_init(fs_http_handler_t *handler, array_t *arg)
{
	#define FS_HANDLER_FUNCTION_IDX 0
	#define FS_HANDLER_ARG_IDX 1 

	if (arg->size != 2) { 
		goto error;
	}
	if (arg->item[FS_HANDLER_FUNCTION_IDX].type != T_FUNCTION) {
		goto error;
	}
	//arg is mixed

	handler->function = &arg->item[FS_HANDLER_FUNCTION_IDX];
	handler->arg = &arg->item[FS_HANDLER_ARG_IDX];

	return 0;
error:
	return FS_HTTP_ERROR_HANDLER;
}

void f_fs_http_request()
{
	fs_http_uri_t uri;
	fs_http_req_info_t req_info;
	fs_http_handler_t handler;
	int ret = -1;

	SET_ALL_ARGS;

	array_t *arg_uri = ARGS(0)->u.arr;
	array_t *arg_req_info = ARGS(1)->u.arr;
	array_t *arg_handler = ARGS(2)->u.arr;

	if ((ret = uri_init(&uri, arg_uri))) 
		goto end;
	if ((ret = req_info_init(&req_info, arg_req_info))) 
		goto end;
	if ((ret = handler_init(&handler, arg_handler)))
		goto end;

	ret = http_request(&uri, &req_info, &handler);
end:
	POP_ALL_ARGS;
	push_number(ret);
}

void f_fs_url_encode_map()
{
	char end = '\0';

	SET_ALL_ARGS;

	mapping_t *param = ARGS(0)->u.map;
	struct evbuffer *data = encode_param(param);
	POP_ALL_ARGS;

	if (data == NULL) {
		push_undefined();
		return;
	}

	evbuffer_add(data, &end, sizeof(end)); 
	copy_and_push_string((char*)EVBUFFER_DATA(data));
	evbuffer_free(data);
}

void f_fs_url_encode()
{
	SET_ALL_ARGS;
	char *encode = url_easy_escape(ARGS(0)->u.string, 0);
	char *str = string_copy(encode, "http encode");
	free(encode);
	put_malloced_string(str);
}
void f_fs_url_decode()
{
	SET_ALL_ARGS;
	char *decode = url_easy_unescape(ARGS(0)->u.string, 0, NULL);
	char *str = string_copy(decode, "http decode");
	free(decode);
	put_malloced_string(str);
}

///////////////////////////////////////////////////////////////
/*
method:
uri:
query_string:
path_info
script_name

content_type:
content_length:
data:
*/

static mapping_t *keyval_to_mapping(struct evkeyvalq *keyvals)
{
        struct evkeyval *pair;
	svalue_t *v;
	mapping_t *m = allocate_mapping(1);

        TAILQ_FOREACH(pair, keyvals, next) {
		v = map_insert_str_key(m, pair->key);
		set_svalue_malloced_string(v, string_copy(pair->value, "http server filter"));
        }    

	return m;
}

static mapping_t *parse_data(const char *data)
{
	svalue_t *v;
        char *p;
	mapping_t *m = allocate_mapping(1);
	char *adata = strdup(data);

        p = adata;
        while (p != NULL && *p != '\0') {
                char *key, *value;
		char *dkey, *dvalue;
                char *argument = strsep(&p, "&");

                value = argument;
                key = strsep(&value, "=");
                if (value == NULL) {
                        break;
		}

		dkey = url_easy_unescape(key, 0, NULL);
                dvalue = url_easy_unescape(value, 0, NULL);

		v = map_insert_str_key(m, dkey);
		set_svalue_malloced_string(v, string_copy(dvalue, "http server parse data"));

                free(dkey);
                free(dvalue);
        }    

        free(adata);
	return m;
}

void http_cb(struct evhttp_request *req, void *http)
{
	svalue_t *v, *ret;
	svalue_t arg;
	mapping_t *mdata;
	char tmp[512];

	mapping_t *m = allocate_mapping(1);
	mapping_t *request =  allocate_mapping(1);
	
	//create the request-line
	switch (req->type) {
		case EVHTTP_REQ_POST:
			v = map_insert_str_key(request, "method");
			set_svalue_malloced_string(v, string_copy("POST", "http server"));
			break;
		case EVHTTP_REQ_GET:
			v = map_insert_str_key(request, "method");
			set_svalue_malloced_string(v, string_copy("GET", "http server"));
			break;
		case EVHTTP_REQ_HEAD:
		default:
			free_mapping(m);
			free_mapping(request);
			evhttp_send_error(req, 501, "Not Implemented");
			return;
	}

	v = map_insert_str_key(request, "remote_host");
	set_svalue_malloced_string(v, string_copy(req->remote_host, "http server"));

	v = map_insert_str_key(request, "uri");
	set_svalue_malloced_string(v, string_copy(req->uri, "http server"));

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "HTTP/%d.%d", req->major, req->minor);
	v = map_insert_str_key(request, "version");
	set_svalue_malloced_string(v, string_copy(tmp, "http server"));

	v = map_insert_str_key(m, "request");
	set_svalue_mapping(v, request);

	//create the header
	mapping_t *header = keyval_to_mapping(req->input_headers);
	v = map_insert_str_key(m, "header");
	set_svalue_mapping(v, header);


	//create the data
	if (req->type == EVHTTP_REQ_POST) {
		unsigned len = EVBUFFER_LENGTH(req->input_buffer);
		if (len > 0) {
			char *data = malloc(len + 1);
			memcpy(data, EVBUFFER_DATA(req->input_buffer), len);
			data[len] = '\0';
			mdata = parse_data(data);
			v = map_insert_str_key(m, "data");
			set_svalue_mapping(v, mdata);
			free(data);
		}
	} else if (req->type == EVHTTP_REQ_GET) {
		char *query_info = strchr(req->uri, '?');
		if (query_info != NULL) {
			mdata = parse_data(query_info + 1);
			v = map_insert_str_key(m, "data");
			set_svalue_mapping(v, mdata);
		}
	}

	set_svalue_mapping(&arg, m);
	ret = call_script_func("/rc/http", "HttpServerCB", "m", &arg);
	free_mapping(m);
	if (ret == NULL || IS_LPC_UNDEF(ret) || ret->type != T_STRING) {
		evhttp_send_error(req, 500, "Internal Server Error");
	} else {
		struct evbuffer *evb = evbuffer_new();
		evbuffer_add(evb, ret->u.string, SVALUE_STRLEN(ret));
		evhttp_send_reply(req, HTTP_OK, "ok", evb);
		evbuffer_free(evb);
	}

}

struct evhttp *http_server_init(struct event_base *base, char *ip, unsigned int port)
{
	struct evhttp *myhttp;

	myhttp = evhttp_new(base);
	if (evhttp_bind_socket(myhttp, ip, port) == -1) {
		evhttp_free(myhttp);
		fprintf(stderr, "http server bind socket error.host=%s,port=%d\n", ip, port);
		return NULL;
	}

	//evhttp_set_cb(myhttp, "/postit", http_post_cb, NULL);
	evhttp_set_gencb(myhttp, http_cb, myhttp);
	
	return myhttp;
}

void http_server_destroy(struct evhttp *http)
{
	evhttp_free(http);
}

