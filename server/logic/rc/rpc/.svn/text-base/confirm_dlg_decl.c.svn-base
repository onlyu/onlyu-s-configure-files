
#define T_DIALOG_DEFAULT	(0)
#define T_DIALOG_POPUP		(1)

class msg_confirm_dialog_s {
	int sid;
	string message;            
	string title;
	string ok;
	string cancel;
	int modeless; //1:非模态 0:模态
	int check_type;
	int check_state;
	string check_str;
}

class msg_confirm_dialog_return_s {
	int sid;
	int result;    /*1(ok)/0(cancel)*/
	int check_type;
}

void rpc_client_show_confirm_dialog(int uid, class msg_confirm_dialog_s info);

void rpc_server_confirm_dialog_result(object oUser, class msg_confirm_dialog_return_s info);

