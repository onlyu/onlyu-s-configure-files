// gamed不通过dbd直接操作用户数据用模块
// -------------------------------------------------------
// 本文件不能加入对user obj的写操作，如果需要请用dbd异步作
// 因为，如果本地作写操作可能引起数据不同步
// 本文件提供的数据也不能完全保证是最新的，
// 所以在玩家在线的情况下，不应该跑到本文件
// 注:这也是一个潘多拉盒子，如果要使用这个里面的内容，
// 请在充分理解架构之后，合理利用。
// -------------------------------------------------------


#define USER_DIR "user/"		
#define INACTIVE_USER_DIR "inactive_user/"		
#define USER_EXT ".py"		
#define USER_FILE_FORMAT USER_DIR"%02d/u%d"USER_EXT
#define INACTIVE_USER_FILE_FORMAT INACTIVE_USER_DIR"%02d/u%d"USER_EXT

//uidFile = sprintf("user/%02d/u%d.py", ukey, uid);
int getKey( int uid )
{
	return uid%100;
}

// data = fs_restore_from_file(uidFile);
// prop = data["prop"];

// 本函数返回离线玩家属性，同时执行op，将prop传递过去
varargs mixed getProp( int uid, mixed key, function op, mixed* args)
{
	mapping data, prop;
	int ukey;
	string uidFile;

	// 暂时本地，只作警告
	if ( is_online( uid ))
	{
		debug_message( "Error:在玩家在线的情况下，调用到了rc/user_data_op的GetProp" );	
	}

	ukey = getKey( uid );
	uidFile = sprintf( USER_FILE_FORMAT, ukey, uid );

	data = fs_restore_from_file(uidFile);
		
	if ( undefinedp( data ) )
	{
		uidFile = sprintf( INACTIVE_USER_FILE_FORMAT, ukey, uid ); 
		data = fs_restore_from_file(uidFile);

		if ( undefinedp( data ) )
		{
			debug_message( "[" + uid + "] not Exist!!!" );
			return 0;
		}
	}

	prop = data["prop"];

	if ( undefinedp( prop ) )
	{
		debug_message( "[" + uid + "] formate ERROR!!!" );
		return 0;
	}

	if (functionp(op))
	{
		evaluate(op, uid, prop, args);
	}
	
	return prop[key];
}
