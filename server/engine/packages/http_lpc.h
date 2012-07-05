

#include <event.h>
#include <evhttp.h>
struct evhttp *http_server_init(struct event_base *base, char *ip, unsigned int port);
void http_server_destroy(struct evhttp *http);
