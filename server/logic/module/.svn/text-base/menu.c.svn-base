#include "/rc/rpc/menu.h"
#include <npc_key.h>
//#include <user_key.h>
//#include <macros.h>

// 菜单类型
#define MENU_T_NPC        1

#define MENU_ID           "_memu_id_"
#define MENU_OPTIONS	  "_menu_options_"

/*

options = ([
        "type": MENU_T_NPC,
        "id": npcId,
        "opts": ([
                "menu": optMsg,
                "type": "text/button/input_text/input_num/reward",
                "control":"", //一些额外信息，一般是打成json格式
                "key":"input key", //只有input类型才用到
                "cbFile":回调文件
                "func": callbackFunc,
                "args": ({arg1, arg2}),
        ]),
])

*/

static int _addOption(mapping options, mapping option)
{
	int size;
	if (undefinedp(options)){
		throw("menu参数没有初始化");
		return 0;
	}
	if (undefinedp(options["opts"])){
		options["opts"] = ([]);
	}
	size = sizeof(options["opts"]);
	size ++;
	options["opts"][size] = option;
        return size;
}

int addOption(mapping options, mapping option)
{
        return _addOption(options, option);
}

// 初始化选项
mapping initNpcOptions(int npcid)
{
        mapping options = ([]);
	options["type"] = MENU_T_NPC;
	options["id"] = npcid;
	return options;
}

// 文本选项
mapping createOption(string optMsg, string cbFile, string func, mixed *args)
{
        mapping option = ([
                "menu": optMsg,
                "type": "text",
                "cbFile": cbFile,
                "func": func,
                "args": args,
        ]);
        return option;
}

// 按钮选项
mapping createButtonOption(string optMsg, string cbFile, string func, mixed *args)
{
        mapping option = ([
                "menu":optMsg,
                "type":"button",
                "cbFile":cbFile,
                "func":func,
                "args":args,
        ]);
        return option;
}

// 默认选项(客户端右下角的默认操作按钮)
mapping createDefaultOption(string optMsg, string cbFile, string func, mixed *args)
{
        mapping option = ([
                "menu":optMsg,
                "type":"default",
                "cbFile":cbFile,
                "func":func,
                "args":args,
        ]);
        return option;
}

// 文本输入
mapping createTextInputOption(string optMsg, string key)
{
        mapping option = ([
                "menu":optMsg,
                "type":"input_text",
                "func":0,
                "key":key, // 查询输入用的key
                "args":([]),
        ]);
        return option;
}

// 数字输入
varargs mapping createNumInputOption(string optMsg, string key, int lowLimit, int upLimit, int defaultValue)
{
        mapping control = ([]);
        if (!undefinedp(lowLimit)) control["L"] = lowLimit;
        if (!undefinedp(upLimit)) control["U"] = upLimit;
        if (!undefinedp(defaultValue)) control["D"] = defaultValue;

        mapping option = ([
                "menu":optMsg,
                "type":"input_num",
                "control":lpc2pystr(control),
                "func":0,
                "key":key, // 查询输入用的key
                "args":([]),
        ]);


        return option;
}

// 奖励信息
mapping createRewardOption(int money, int exp, mapping items)
{
        mapping control = ([
                "money": money,
                "exp": exp,
                "items": items,
        ]);

        mapping option = ([
                "menu":"",
                "type":"reward",
                "control":lpc2pystr(control),
        ]);

        return option;
}

// 用于获取输入
static mapping _inputs = ([]);
static int _menu_id = 0;
static void optionsSetInputs(mapping inputs)
{
	_inputs = inputs;
}

mixed optionsGetInput(object user, string key)
{
        int menuId = user->getTemp(MENU_ID);
        // 如果menuId不对的话 返回空串
        if( _menu_id != menuId ) return "";

        return _inputs[key];
}

static void optionsClearInputs()
{
	_inputs = ([]);
}

void tellOptions(object user, mapping options, string msg)
{
	int menuId, i;
	mapping option;

	//user->TellMe(sprintf("%O", options));
	// 检查对象是否存在
	if(!objectp(user)){
		debug_message("user is null! can't get object!");
		return;
	}

        // 获取npc uniqId
	int npcId = options["id"];
	int uniqId = Import("UNIQ_NPC")->npcId2SystemId(npcId);

	// 获取menuId
	menuId = user->getTemp(MENU_ID);
	if( undefinedp(menuId) ) menuId=0;
	menuId ++;

	user->setTemp(MENU_ID, menuId);
	
	class msg_menu_t menu = new(class msg_menu_t);
	menu->menuId = menuId;
	menu->icon = GetNpc(npcId, K_NPC_ICON);
	menu->name = GetNpc(npcId, K_NPC_NAME);
	menu->msg = msg;
	menu->options = allocate(sizeof(options["opts"]));

	for(i = 1; i<=sizeof(options["opts"]); ++i){
		class msg_option_t opt = new(class msg_option_t);

		option = options["opts"][i];
		opt->text = option["menu"];
		opt->type = option["type"];
		opt->control = undefinedp(option["control"]) ? "" : option["control"];
		menu->options[i - 1] = opt;
	}
	user->setTemp(MENU_OPTIONS, options);
	rpc_client_menu(user->getId(), menu);
	return;
}

void rpc_server_menu(object user, class msg_menu_return_t menu_return)
{
	int menuId = user->getTemp(MENU_ID);
	int uid = user->getId();
	mixed *args = ({});
	mapping options = user->getTemp(MENU_OPTIONS);

	// user->TellMe(sprintf("%O", menu_return));
	// ID不同说明不是最后一次弹出框的回复
	if (menu_return->menuId != menuId) {
		debug_message(sprintf("user[%d], 菜单ID[%d], 出错%d", uid, menu_return->menuId, menuId));
		return;
	}

	if (!mapp(options) || undefinedp(options["opts"])) {
		user->setTemp(MENU_ID, menuId + 1);	
		return;
	}

	mapping option = options["opts"][menu_return->index];

	if (!mapp(option)) {
		user->setTemp(MENU_ID, menuId + 1);	
		return;
	}

	if (undefinedp(option["func"])) {
		user->setTemp(MENU_ID, menuId + 1);	
		return;
	}

	// 生成玩家输入结构
	int j = 0;
	mapping inputs = ([]);
	
	for( int i=1; i<=sizeof(options["opts"]); i++){
		mapping opt = options["opts"][i];
		if( opt["type"] == "input_text"){
			inputs[ opt["key"] ] = menu_return->inputs[j++];
		} else if( opt["type"] == "input_num" ) {
			inputs[ opt["key"] ] = to_int(menu_return->inputs[j++]);
                }
	}

	optionsSetInputs(inputs);
	_menu_id = menu_return->menuId;

	if (sizeof(option["func"])) {
		if (options["type"] == MENU_T_NPC) {
			if(0 /*options["id"] && Import("UTIL")->IsNpcAway(user, options["id"])*/) {
				user->TellMe("距离太远了，你们无法对话!");
				optionsClearInputs();
				user->setTemp(MENU_ID, menuId + 1);
				return;
			}   
			call_other(option["cbFile"], 
					({ option["func"], user, options["id"], }) + option["args"]);
		} else {
			call_other(option["cbFile"], 
					({ option["func"], user, }) + option["args"]);
		}
	}

	optionsClearInputs();
        user->setTemp(MENU_ID, menuId + 1);	
}

