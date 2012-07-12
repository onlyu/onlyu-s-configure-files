
#include "macros.h"
#include <var_prop.h>
#include <log.h>


#define HTTP_MAX_LEN (1024 * 128)

#define HTTP_METHOD_GET 0
#define HTTP_METHOD_POST 1
#define HTTP_METHOD_HEAD 2

static object logger;

mapping post_header = (["Host":"somehost", "Connection":"close", "Content-Type": "application/x-www-form-urlencoded"]);
mapping get_header = (["Host":"somehost", "Connecton": "close",]);
mixed *get_req_info = ({HTTP_METHOD_GET, get_header});
mixed *head_req_info = ({HTTP_METHOD_HEAD, get_header});

void conver_param_type(mixed k, mixed v, mapping m)
{
	string str = "";
	if (mapp(v) || arrayp(v)) {
		m["" + k] = lpc2pystr(v);
	} else {
		m["" + k] = "" + v;
	}
}

string encode_param(mapping param)
{
	mapping m = ([]);
	map_mapping(param, (:conver_param_type:), m);
	return fs_url_encode_map(m);
}

int Get(string host, int port, string url, mapping param, function func, mixed arg)
{
	string data = encode_param(param);
	mixed *uri = ({host, port, url + data});
	mixed *cb = ({func, arg});
	//debug_message(sprintf("http get %O\n", uri));
	return fs_http_request(uri, get_req_info, cb);
}

int Head(string host, int port, string url, mapping param, function func, mixed arg)
{
	string data = encode_param(param);
	mixed *uri = ({host, port, url + data});
	mixed *cb = ({func, arg});
	return fs_http_request(uri, head_req_info, cb);
}

int Post(string host, int port, string url, mapping param, function func, mixed arg)
{
	string data = encode_param(param);
	mixed *uri = ({host, port, url});
	mixed *req_info = ({HTTP_METHOD_POST, post_header, data});
	mixed *cb = ({func, arg});

	return fs_http_request(uri, req_info, cb);
}

void TestHttpCB(mixed arg, int code, string response)
{
	debug_message(sprintf("code:%d,reponse:%s\n", code, response));
}

void TestHeadCB(mixed arg, int code, mapping headers)
{
	//debug_message(sprintf("http arg:%O,code:%d,reponse:%s\n", arg, code, response));
	debug_message(sprintf("code:%d,headers:%O\n", code, headers));
}

void Test()
{
	string host = "192.168.0.5";
	int port = 6000;
	string uri = "/fcgi/";
	mapping params = (["action":"query_ums", "ums":1]);
	function func = (:TestHttpCB:);
	function headfunc = (:TestHeadCB:);

	string gargstr = "hello arg get get";
	mixed *gargarr = ({"get", "get", 1, 2, 3});
	mapping gargmap = (["getkey":"getvalue"]);

	string pargstr = "hello arg post post";
	mixed *pargarr = ({"post", "post", 1, 2, 3});
	mapping pargmap = (["postkey":"postvalue"]);

	int i;
	int c = 1;
	
	for (i = 0; i < c; ++i) {
		Get(host, port, uri + "?", params, func, gargstr);
		/*
		Get(host, port, uri + "?", params, func, gargarr);
		Get(host, port, uri + "?", params, func, gargmap);


		Post(host, port, uri, params, func, pargstr);
		Post(host, port, uri, params, func, pargarr);
		Post(host, port, uri, params, func, pargmap);

		Head(host, port, uri + "?", params, headfunc, gargstr);
		Head(host, port, uri + "?", params, headfunc, gargarr);
		Head(host, port, uri + "?", params, headfunc, gargmap);
		*/
	}
}

//////////////// http server

//rule: pattern,module,function,arg

MEMORY_VAR(rules, ({}))

#define HTTP_REMOTE_HOST(req) ((req)["request"]["remote_host"])
#define HTTP_METHOD(req) ((req)["request"]["method"])
#define HTTP_URI(req) ((req)["request"]["uri"])
#define HTTP_DATA(req) ((req)["data"])
#define HTTP_HEADER(req) ((req)["header"])

static string * allow_host = ({
	"^192.168.0",
	"^192.168.12",
	"^121.10.246", //公司电信外部机器
	"^61.143.222", //公司电信外部机器
	"^119.38.128", //公司网通外部机器
	"^120.31.48", //公司网通外部机器
	"^183.62.45.74", //公司内部机器出口
	"^183.62.45.75", //公司内部机器出口
	"^183.62.45.76", //公司内部机器出口
	"^183.62.45.77", //公司内部机器出口
	"^183.62.45.78", //公司内部机器出口
});

string HttpServerCB(mapping HttpRequest)
{
	string  host = HTTP_REMOTE_HOST(HttpRequest);
	string *remote_host = ({host});
	string *uri = ({HTTP_URI(HttpRequest),});
	int pass = 0;

	foreach (string pattern in allow_host) {
		string *match = regexp(remote_host, pattern); 
		if (sizeof(match) == 1) {
			pass = 1;
			break;
		}
	}

	if (pass == 0) {
		logger->Error("deny remote host=%s", host);
		return 0;
	}

	//debug_message(sprintf("req:%O,rule:%O\n", uri, rules));
	foreach (mapping rule in rules) {
		string *match = regexp(uri, rule["pattern"]); 
		if (sizeof(match) == 1) {
			//string response = evaluate(rule["function"], HttpRequest, rule["arg"]);
			string response = call_other(rule["module"], ({rule["function"], HttpRequest, rule["arg"],}), );
			return response;
		}
	}

	//return 非字符串，让引擎返回错误
	return "";
} 

void SetHttpServerRule(mapping *rule)
{
	rules = rule;
}



#define ERR_CALL -1
#define ERR_METHOD -2
#define ERR_PARAM -3
#define ERR_NOT_IMPLEMENTED -4
#define ERR_LOGIC -5
#define ERR_LONG_PACK -6
#define ERR_HOSTNUM -7

#define ERR_GATEWAY_CALL -10
#define ERR_GATEWAY_CONN -11
#define ERR_DISMATCH_HID -12
#define ERR_POST_METHOD_AUTH -13
#define ERR_POST_METHOD_GAME -14

//rpc 基本错误
mapping rpcerr = ([
	ERR_CALL:"bad call format",
	ERR_METHOD:"bad method",
	ERR_PARAM:"bad param",
	ERR_HOSTNUM:"bad hostnum",
	ERR_NOT_IMPLEMENTED:"not implemented",
	ERR_LOGIC:"app logic error",
	ERR_LONG_PACK:"package too long",
	ERR_DISMATCH_HID:"dismatch hid",
	ERR_POST_METHOD_AUTH:"authd only accept center_method",
	ERR_POST_METHOD_GAME:"gamed do not accept center_method",
]);


mapping rpc_ok(mixed *argc...)
{
	return (["result":argc]);
}

mapping rpc_error(int errno, string errstr, mixed *info...)
{
	return (["error":(["errnor":errno, "errstr":errstr, "info":info]),]);
}

///////////////////////////////////////////////////////////////////
//call: method=abc, params=[x,y,z,]
//return: {result:any} or {error:{errno = no ,errstr = str, info = any}}

#define RPC_ERROR(no) lpc2pystr(rpc_error((no), rpcerr[(no)]))

mixed GetDataKey(mapping req, mixed key)
{
	mapping data = HTTP_DATA(req);
	//debug_message(sprintf("dispatch http:%O\n", HttpRequest));
	if (!mapp(data)) {
		return 0;
	}

	return data[key];
}

string rpc_dispatch(mapping HttpRequest, mapping arg)
{
	mapping info;
	string method;
	string params;
	mixed *arr;
	mapping ret;
	string *match;
	string hostnum;
	int hid;
	int localhid = getHostNum();

	mapping data = HTTP_DATA(HttpRequest);
	//debug_message(sprintf("dispatch http:%O\n", HttpRequest));
	if (!mapp(data)) {
		return RPC_ERROR(ERR_CALL);
	}

	method = data["method"];
	params = data["params"];
	hostnum = data["hostnum"];
	if (!stringp(method)) {
		return RPC_ERROR(ERR_METHOD);
	}
	if (!stringp(params)) {
		return RPC_ERROR(ERR_PARAM);
	}
	if (!stringp(hostnum)) {
		return RPC_ERROR(ERR_HOSTNUM);
	}

	hid = to_int(hostnum);
	match = regexp(({method}), "^center_"); 

	//如果是center方法，应该只能访问验证服;否则只能访问游戏服
	if ("module/serverinfo"->IsAuthHost(localhid)) {//只处理center打头的method
		if (sizeof(match) <= 0) {
			logger->Error("center only accept center_method hid=%d,inhid=%d,method=%s", localhid, hid, method);
			return RPC_ERROR(ERR_POST_METHOD_AUTH);
		}
	} else {
		if (sizeof(match) > 0) {
			logger->Error("gamed do not accept center_method hid=%d,inhid=%d,method=%s", localhid, hid, method);
			return RPC_ERROR(ERR_POST_METHOD_GAME);
		}
		if(localhid != hid) {
			logger->Error("dismatch hostnum hid=%d,inhid=%d,method=%s", localhid, hid, method);
			return RPC_ERROR(ERR_DISMATCH_HID);
		}
	}

	//在这里统一限制包的长度，防止数据包过长或者恶性攻击
	if (sizeof(params) >= HTTP_MAX_LEN) {
		logger->Error("package too long");
		return RPC_ERROR(ERR_LONG_PACK);
	}

	arr = pystr2lpc(params);
	if (!arrayp(arr)) {
		return RPC_ERROR(ERR_PARAM);
	}

	ret = call_other(arg["module"], ({arg["prefix"] + method, HttpRequest}) + arr);
	if (undefinedp(ret)) {
		return RPC_ERROR(ERR_NOT_IMPLEMENTED);
	} else if(mapp(ret)) {
		return lpc2pystr(ret);
	} else {
		return RPC_ERROR(ERR_LOGIC);
	}
}


void create()
{

	logger = Import("LOG")->New("http");
}
