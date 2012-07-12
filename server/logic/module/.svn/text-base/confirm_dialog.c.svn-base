
#include <item_key.h>
#include <save.h>
#include "/rc/rpc/confirm_dlg.h"

#define DLG_MAX_NUM      10
#define DLG_TIME         100
#define DLG_CALL_BACK    "_confirm_dlg_call_back"
#define DLG_SID          "_confirm_dlg_id"


#define PROMPT_NULL             0
#define PROMPT_DAY_CANCEL       1
#define PROMPT_FOREVER_CANCEL   2
#define PROMPT_LOGIN_CANCEL		3

#define PROMPT_FOREVER_CANCEL_VAL	-1
#define PROMPT_LOGIN_CANCEL_VAL		-2

DB_VAR(data, ([]))

string save_name = "prompt";

string temp_prop = "_t_m_prompt_temp_var";

static mapping mpStaticPrompt = ([]);

static mapping mpDateStr2Type = ([
		"今天" : PROMPT_DAY_CANCEL, 
		"永久" : PROMPT_FOREVER_CANCEL, 
		"本次登录" : PROMPT_LOGIN_CANCEL,
		]);

void create()
{
	setSaveName(save_name);
	doRestore();
	mpStaticPrompt = "data/prompt_data"->get_data();
}

mapping getSaveData(int uid)
{
	int hash = uid % 100;
	if (undefinedp(data[hash])) {
		data[hash] = ([]);
	}
	if (undefinedp(data[hash][uid])) {
		data[hash][uid] = ([]);
	}
	return data[hash][uid];
}

int getPromptCancelType(string promptKey)
{
	mapping mpPrompt = mpStaticPrompt[promptKey];
	if (undefinedp(mpPrompt)) return 0;

	if (sizeof(mpPrompt["date"])) {
		return mpDateStr2Type[mpPrompt["date"]];
	}
	return 0;
}

int needDayPrompt(object user, string promptKey)
{
	int cancel_type = getPromptCancelType(promptKey);
	if (cancel_type == PROMPT_LOGIN_CANCEL) {
		mapping mpTemp = user->getTemp(temp_prop, ([]));
		if (undefinedp(mpTemp[promptKey]) || undefinedp(mpTemp[promptKey]["time"])) {
			return 1;
		}
		return 0;
	}
	else {
		mapping mpData = getSaveData(user->getId());
		debug_message("%O", mpData);
		if (undefinedp(mpData[promptKey])) {
			return 1;
		}
		int time = mpData[promptKey]["time"];
		if (time > 0 && get_rela_day_no(time) == get_rela_day_no()) {
			return 0;
		} else if (time < 0) {
			return 0;
		}
		return 1;
	}
}

int getDayPromptResult(object user, string promptKey)
{
	// 若为PROMPT_LOGIN_CANCEL则取result，否则暂时返回1
	int cancel_type = getPromptCancelType(promptKey);
	if (cancel_type == PROMPT_LOGIN_CANCEL) {
		mapping mpTemp = user->getTemp(temp_prop, ([]));
		if (undefinedp(mpTemp[promptKey]) ||
				undefinedp(mpTemp[promptKey]["result"])) {
			return 1;
		}
		int result = mpTemp[promptKey]["result"];
		if (undefinedp(result)) return -1;
		return result;
	}
	return 1;
}

void setPromptCancel(object user, string promptKey, int result)
{
	mapping mpData = getSaveData(user->getId());
	if (undefinedp(mpData[promptKey])) mpData[promptKey] = ([]);
	
	mapping mpPrompt = mpStaticPrompt[promptKey];

	int check_type = mpPrompt["date"];

	if (check_type == PROMPT_DAY_CANCEL) {
		mpData[promptKey]["time"] = time();
		mpData[promptKey]["result"] = result;
	} 
	else if (check_type == PROMPT_FOREVER_CANCEL) {
		mpData[promptKey]["time"] = PROMPT_FOREVER_CANCEL_VAL;
		mpData[promptKey]["result"] = result;
	} 
	else if (check_type == PROMPT_LOGIN_CANCEL) {
		//mpData[promptKey]["time"] = PROMPT_LOGIN_CANCEL_VAL;
		mapping mpTemp = user->getTemp(temp_prop, ([]));
		mpTemp[promptKey] = ([]);
		mpTemp[promptKey]["time"] = PROMPT_LOGIN_CANCEL_VAL;
		mpTemp[promptKey]["result"] = result;
	}
}

void clearPrompt(object user)
{
	mapping mpData = getSaveData(user->getId());
	clearDBVar(mpData);
}

int getSessionID(object user)
{
	int sid = user->getTemp(DLG_SID);

	sid++;
	user->setTemp(DLG_SID, sid);
	return sid;
}

void rpc_server_confirm_dialog_result(object user, class msg_confirm_dialog_return_s info)
{
	mapping mpInfo = user->getTemp(DLG_CALL_BACK);
	int sid = user->getTemp(DLG_SID);
	mapping mpCallBack;

	if (!mapp(mpInfo)) {
		return;
	}

	if (undefinedp(mpInfo[info->sid])) {
		return;
	}

	mpCallBack = mpInfo[info->sid];

	if (sizeof(mpCallBack["prompt"]) && info->check_type) {
		setPromptCancel(user, mpCallBack["prompt"], info->result);
	}

	if ( (!mpCallBack["check"])
		|| 	call_other(this_object(), mpCallBack["check"]["func"], user, mpCallBack["check"]["args"]) > 0 )
	{	
		if (sizeof(mpCallBack["func"])) {
			call_other(mpCallBack["file"], ({ mpCallBack["func"], user, info->result, }) + mpCallBack["args"]);
		}
	}	
	map_delete(mpInfo, info->sid);
}

int initDialog(object user, string prompt, string file, string func, mixed *args)
{
	int sid = getSessionID(user);
	mapping mpInfo = user->getTemp(DLG_CALL_BACK);
	mixed *delSids;

	if (!mapp(mpInfo)) {
		user->setTemp(DLG_CALL_BACK, ([]));
		mpInfo = user->getTemp(DLG_CALL_BACK);
	}

	if (sizeof(mpInfo) > DLG_MAX_NUM) {
		debug_message(sprintf("确认框大于%d", DLG_MAX_NUM));
		return 0;
	}

	// 清除超时的确认框
	delSids = ({});
	foreach (int tmpsid, mapping mpTmpCB in mpInfo) {
		if (mpTmpCB["time"] + DLG_TIME < time()) {
			delSids += ({ tmpsid });
		}
	}
	if (sizeof(delSids)) {
		foreach (int delid in delSids) {
			map_delete(mpInfo, delid);
		}
	}

	mpInfo[sid] = ([
			"file": file,
			"func": func, 
			"time": time(),
			]);

	if (sizeof(prompt)) {
		mpInfo[sid]["prompt"] = prompt;
	}
	mpInfo[sid]["args"] = sizeof(args) ? args : ({});
	return sid;
}

varargs int doSendConfirmDialog(int uid, int sid, string title, string message, string ok, string cancel, int modeless, int check_type, int check_state, string check_str)
{
	class msg_confirm_dialog_s dlg_info = new(class msg_confirm_dialog_s);
	dlg_info->sid = sid;
	dlg_info->message = message;
	dlg_info->title = title;
	dlg_info->ok = ok;
	dlg_info->cancel = cancel;
	dlg_info->modeless = modeless ? 1 : 0;
	dlg_info->check_type = check_type ? check_type : 0;
	dlg_info->check_state = check_state;
	dlg_info->check_str = sizeof(check_str) ? check_str : "";
	rpc_client_show_confirm_dialog(uid, dlg_info);
	return sid;
}

int sendPopupConfirmDialog(int uid, string title, string message, string ok, string cancel, string func, mixed *args, int timeout, int modeless)
{
	string file = caller_name();
	int sid = initDialog(get_user(uid), "", file, func, args);
	if (sid) {
		doSendConfirmDialog(uid, sid, title, message, ok, cancel, modeless, 0, 0, "");
	}
	return sid;
}

int sendConfirmDialog(int uid, string title, string message, string ok, string cancel, string func, mixed *args, int modeless)
{
	string file = caller_name();
	int sid = initDialog(get_user(uid), "", file, func, args);
	if (sid) {
		doSendConfirmDialog(uid, sid, title, message, ok, cancel, modeless);
	}
	return sid;
}

void tellPrompt(object user, string promptKey, string file, string func, mixed *args, mapping replaceDict)
{
	int uid = user->getId();

	if (undefinedp(mpStaticPrompt[promptKey])) {
		debug_message("prompt : %s not exist", promptKey);
		return;
	}

	if (!arrayp(args)) args = ({});

	if (!needDayPrompt(user, promptKey)) {
		if (sizeof(func)) {
			int result = getDayPromptResult(user, promptKey);
			if (result < 0) {
				result = 1;
			}
			call_other(file, ({ func, user, result, }) + args);
		}
		return;
	}

	int sid = initDialog(user, promptKey, file, func, args);

	if (sid) {
		mapping mpPrompt = mpStaticPrompt[promptKey];

		string title = !undefinedp(mpPrompt["title"]) ? mpPrompt["title"] : "";
		string message = mpPrompt["content"]; 
		string confirm = mpPrompt["confirm"]; 
		string cancel = mpPrompt["cancel"]; 
		int check_state = mpPrompt["check_select"];
		int cancel_type = PROMPT_NULL;
		string check_str = "";

		if (sizeof(replaceDict)) {
			foreach (string _key, string _val in replaceDict) {
				message = replace_string(message, "$"+_key, _val);
			}
		}
		if (sizeof(mpPrompt["date"])) {
			cancel_type = mpDateStr2Type[mpPrompt["date"]];
		}

		if (sizeof(mpPrompt["hint"])) {
			check_str = mpPrompt["hint"];
		}
		int modeless = undefinedp(mpPrompt["modeless"]) ? 1 : 0;
		doSendConfirmDialog(uid, sid, title, message, confirm, cancel, modeless, cancel_type, check_state, check_str);
	}
}

/*
int CheckItemFunc(object user, int itemid)
{
	if ( !is_item_online(itemid) ) return 0;
	if (GetItemTemp(itemid, I_ITEM_OWNER) != user->getId()) return 0; 

	return 1;
}

//  这个dialog会强制对物品是否在身上做判断
void itemConfirmDialog(int uid, int itemid, string message, string func, mixed *args)
{
	object user = get_user(uid);

	if ( CheckItemFunc(user, itemid) < 1 ) return ;

	string file = caller_name();
	int sid = initDialog(get_user(uid), "", file, func, args);
	if (sid) {
		doSendConfirmDialog(uid, sid, "物品确认", message, "确定", "取消");
		mapping mpInfo = user->getTemp(DLG_CALL_BACK);
		mpInfo[sid]["check"] = ([ "func" :"CheckItemFunc", "args" : itemid ]);
	}
}
*/


// ==============================================

void test_prompt_cb(object user, int iResult, string cTest)
{
	user->tellMe("result : %d, %s", iResult, cTest);
}

void test_prompt(object user, string promptKey)
{
	tellPrompt(user, promptKey, __FILE__, "test_prompt_cb", ({ "test" }), ([]));
}
