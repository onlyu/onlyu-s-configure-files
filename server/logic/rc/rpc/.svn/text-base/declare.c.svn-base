
#include <static_rpc_id.h>

static mapping static_login_ptoid = ([
		"rpc_server_version": RPC_SERVER_VERSION,
		"rpc_client_update_pto": RPC_CLIENT_UPDATE_PTO,
		"rpc_client_version_return":RPC_CLIENT_VERSION_RETURN,
		"rpc_server_login" : RPC_SERVER_LOGIN,
		"rpc_server_new_uid" : RPC_SERVER_NEW_UID,
]);

static mapping *cfg = ({                        
	([
		"declare":"/rc/rpc/login_decl.c",
		"implement":"/module/login.c",
		"static_ptoid": static_login_ptoid,
	]),
	([
		/*
		 "declare":"/rc/rpc/scene_decl.c",
		 "c_implement":"true",
		 "static_ptoid":static_scene_ptoid,
		*/
		"declare":"/rc/rpc/lscene_decl.c",
         	"implement":"/module/scene/main.c",
	]),
        ([  
         	"declare":"/rc/rpc/wizcmd_decl.c",
         	"implement":"/cmd/wizcmd.c",
        ]), 
	([
		 "declare":"/rc/rpc/user_cmd_decl.c",
		 "implement":"/char/user_cmd.c",
	]),
	([
		 "declare":"/rc/rpc/menu_decl.c",
		 "implement":"/module/menu.c",
	]),
	([
		 "declare":"/rc/rpc/chat_decl.c",
		 "implement":"/module/chat.c",
	]),
	([
		 "declare":"/rc/rpc/mission_decl.c",
		 "implement":"/module/mission/main.c",
	]),
	([
		 "declare":"/rc/rpc/item_decl.c",
		 "implement":"cmd/item.c",
	]),
	([
		 "declare":"/rc/rpc/client_drama_decl.c",
		 "implement":"/module/client_drama.c",
	]),
	([
		 "declare":"/rc/rpc/fight_decl.c",
		 "implement":"/module/fight/main.c",
	]),
	([
		 "declare":"/rc/rpc/fuben_decl.c",
		 "implement":"/module/fuben/rpc.c",
	]),
	([
		 "declare":"/rc/rpc/summon_decl.c",
		 "implement":"cmd/summon.c",
	]),
	([
		 "declare":"/rc/rpc/equip_decl.c",
		 "implement":"module/equip/main.c",
	]),
	([
		 "declare":"/rc/rpc/shop_decl.c",
		 "implement":"module/shop/main.c",
	]),
	([
		 "declare":"/rc/rpc/storage_decl.c",
		 "implement":"cmd/item.c",
	]),
	([
		 "declare":"/rc/rpc/friend_decl.c",
		 "implement":"module/friend/friend.c",
	]),
	([
		 "declare":"/rc/rpc/mail_decl.c",
		 "implement":"module/mail/rpc.c",
	]),
	([
		 "declare":"/rc/rpc/worldmap_decl.c",
		 "implement":"module/worldmap/rpc.c",
	]),
	([
		 "declare":"/rc/rpc/zhaocaimao_decl.c",
		 "implement":"module/zhaocaimao/rpc.c",
	]),
	([
		 "declare":"/rc/rpc/dazuo_decl.c",
		 "implement":"module/dazuo/rpc.c",
	]),
	([
		 "declare":"/rc/rpc/zhaojiang_decl.c",
		 "implement":"module/zhaojiang/rpc.c",
	]),
	([
		 "declare":"/rc/rpc/confirm_dlg_decl.c",
		 "implement":"module/confirm_dialog.c",
	]),
});

mapping* get_cfg()
{
	return cfg;
}

