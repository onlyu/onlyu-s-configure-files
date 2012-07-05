#include "../grammar/spec.h"

//int fs_http_request(string host, int port, string uri, int method, mapping header, mapping params, function func, object obj, mixed|void);
int fs_http_request(mixed *, mixed *, mixed *);
string fs_url_encode_map(mapping);
string fs_url_encode(string);
string fs_url_decode(string);
int fs_curl_request(mapping, function, mixed);
