
#include <system_config.h>

int main()
{
	/*
	string lpc_rpc_dir = "/rc/config"->get_config("LPC_RPC_DIR");
	string sys_rpc_cfg = "/rc/config"->get_config("SYS_RPC_CFG");
	string sys_rpc_id = "/rc/config"->get_config("SYS_RPC_ID");
	string rpc_declare = "/rc/config"->get_config("LPC_RPC_DECLARE");
	*/

	// rpc描述结果文件,注意，是系统的路径，所以不能是 "/" 打头
	string sys_rpc_cfg = "/rc/config"->GetConfig(RPC_CFG);

	string sys_rpc_id = "rc/rpc/rpc_id.h";
	string rpc_declare = "/rc/rpc/declare.c";
	string rpc_tr_script = "/tools/rpc_tr.sh";

	mapping * decls;

	decls = rpc_declare->get_cfg();


	/*
	string *list;
	mapping function_id;
	int i;

	mixed files = get_dir(lpc_rpc_dir);
	list = filter_array(files, (:pre_filter:));

	for(i = 0; i < sizeof(list); ++i) {
		list[i] = lpc_rpc_dir + list[i];
	}

	list = filter_array(list, (:post_filter:), lpc_rpc_id);

	function_id = lpc_rpc_id->rpc_function_id();
	*/

	//fs_rpc_make_cfg(decl, function_id, sys_rpc_cfg);
	debug_message(sprintf("%d",  arrayp(decls)));
	fs_rpc_make_cfg(decls, sys_rpc_id, sys_rpc_cfg);
	os_command(rpc_tr_script, "", "sh");

	return 0;
}
