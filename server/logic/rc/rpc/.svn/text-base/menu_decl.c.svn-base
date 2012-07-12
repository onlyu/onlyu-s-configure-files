
class msg_option_t {
	string type;
	string text;
        string control; //输入项的控制信息 input_num的lowLimit,upLimit...
}

class msg_menu_t {
	int menuId;
	int icon;
	string name;
	string msg;
	class msg_option_t *options;
}

class msg_menu_return_t {
	int menuId;
	int index;
	string *inputs;
}


void rpc_client_menu(int uid, class msg_menu_t menu);
void rpc_server_menu(object user, class msg_menu_return_t menu_ret);

