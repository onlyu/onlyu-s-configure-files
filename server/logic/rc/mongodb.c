
#define  DB_HOST               "192.168.0.17"
#define  DB_PORT               80
#define  DB_URL                "/fs_cgi/db"


void addCommand(mapping content, string cmd, mixed param)
{	
	mapping m = ([]);
	m["cmd"] = cmd;
	m["param"] = param;
	content["cmds"] += ({ m });
}

mapping initCommand(string table)
{
	mapping m = ([]);
	m["table"] = table;
	m["cmds"] = ({});
	return m;
}


mapping packParam(string cmd, string table, mapping param)
{
	mapping m = ([]);
	m["table"] = table;
	m["cmd"] = cmd;
	m["param"] = param;
	return m;
}

void dbHttpCallBack(mixed arg, int code, string response)
{
	if (sizeof(arg) && functionp(arg[0])) {
		mixed res = ([]);
		if (code == 200) {
			res = pystr2lpc(response);
		} else {
			res = response;
		}
		evaluate(arg[0], code, res, arg[1]);
	}
}

void post(mapping param, function func, mixed args)
{
	Import("HTTP")->Post(DB_HOST, DB_PORT, DB_URL, param, (:dbHttpCallBack:), ({func, args}));
}

void insert(string table, mixed param, function func, mixed args)
{
	mapping m = initCommand(table);
	addCommand(m, "insert", param);
	post(m, func, args);
}

void update(string table, mixed param, function func, mixed args)
{
	mapping m = initCommand(table);
	addCommand(m, "update", param);
	post(m, func, args);
}

void find(string table, mixed param, function func, mixed args)
{
	mapping m = initCommand(table);
	addCommand(m, "find", param);
	post(m, func, args);
}

void findOne(string table, mixed param, function func, mixed args)
{
	mapping m = initCommand(table);
	addCommand(m, "findOne", param);
	post(m, func, args);
}

void drop(string table, function func, mixed args)
{
	mapping m = initCommand(table);
	addCommand(m, "drop", ([]));
	post(m, func, args);
}

void create()
{

}


// ====================================================
// TEST

// find
// find_one
// find_and_modify
// group
// insert
// map_reduce
// list(connection['test'].things.find())

void testCallBack(int code, mixed res, int index)
{
	debug_message("code:%d, call back:%O, index:%O", code, res, index);
}

void test()
{
	string table = "users";

	// ==============
	// !!!!!!attention 以下代码会无序执行

	// drop(table, (:testCallBack:), ({1}));
	// insert(table, (["uid":1005, "name": "xxx"]), (:testCallBack:), ({2}));
	// find(table, (["uid":1005, ]), (:testCallBack:), ({3}));
	// update(table, ({ (["uid":1005, ]), (["name": "bbb"]), }), (:testCallBack:), ({}));
	// update(table, ({ (["uid":1005, ]), ([ "$set": (["name": "bbb"]), ]) }), (:testCallBack:), ({4}));

	// right
	// ============>
	
	mapping m = initCommand(table);
	addCommand(m, "drop", ([]));
	addCommand(m, "insert", (["uid":1005, "name": "xxx"]));
	addCommand(m, "find", (["uid":1005, ]));
	addCommand(m, "update", ({ (["uid":1005, ]), ([ "$set": (["name": "bbb"]), ]) }));
	post(m,  (:testCallBack:), ({1}));
}
