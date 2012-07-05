

1, migrate_server.c里面的 static int * src_hostnum_list = ({}) 请填入合服的hostnum。
	例如:  把1018合到1001, src_hostnum_list = ({1001, 1018});

2, 在logic 目录下面做一个符号连接 migrate_src 指到合服的数据目录, 目录下放合服的源数据hostnum/[dat|user]。
   在logic 目录下生成 migrate_dst/[dat|user]，存放合服的结果。
	例如，在自己的logic目录下执行脚本:
		mkdir -p migrate_dst/dat
		mkdir -p migrate_dst/user
		ln -s /home/fs/data migrate_src

3,
先执行./engine -r tools/migrate/migrate_server.c 开始合服，里面会自动生成帮派合并数据。

#(此条已经废弃！)再执行 python migrate_org.py 批量转换用户的帮派。

4, 把合服后migrate_dst的目录cp到目的hostnum目录覆盖相应的存盘文件。
	例如：cp migrate_dst的内容到1001的对应目录,因为我们把1018合并到1001服务器。
