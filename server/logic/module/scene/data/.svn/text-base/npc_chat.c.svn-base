// -------------------  Auto Generate Begin --------------------

mapping data = ([ 
 
	1:([ 
	  "chat":"守护这个村子几十年，人都老咯。", ]),
	2:([ 
	  "opts":({"2号NPC选项1", "2号NPC选项2", }),  
	  "chat":"我打的刀剑，毫无疑问，是最锋利的。", ]),
	3:([ 
	  "opts":({"3号NPC选项1", "3号NPC选项2", }),  
	  "chat":"冬天快到了，得让孩子们多打一些毛皮回来做冬衣。", ]),
	5:([ 
	  "chat":"这龙宫一向祥和，最近却紧闭森严。据说天地龙门都封印起来了！", ]),
	6:([ 
	  "chat":"上了年纪，腿脚不如以前了，岁月不饶人啊……", ]),
	7:([ 
	  "chat":"别看镇长现在这样，年轻时可是百里闻名的有为青年，我的箭法都是他教的。", ]),
	8:([ 
	  "chat":"黑土这孩子，一天到晚就知道玩，愁死我了……", ]),
	10:([ 
	  "chat":"我只希望尽读天下书，尽知天下事。", ]),
	11:([ 
	  "chat":"吾夜观天象，看罡星乱变，前所未有，不知道人世会遭何等大难。", ]),
	13:([ 
	  "chat":"最近总有些生人出没，扰了这渔村的宁静。", ]),
	14:([ 
	  "chat":"在东海渔村呆久了，倒像是自己家乡一样，有感情了。", ]),
	15:([ 
	  "chat":"我最爱爷爷，爷爷也最爱我。", ]),
 
]);

mapping get_data()
{
	return data;
}
// from tmp/data/世界NPC.xls
// -------------------  Auto Generate End   --------------------

// NPC闲话
string getNpcChat(int uniqId)
{
        mapping mpInfo = data[uniqId];
        if( !mpInfo ) return "";

        return mpInfo["chat"];
}

string *getNpcOpts(int uniqId)
{
        mapping mpInfo = data[uniqId];
        if( !mpInfo ) return ({});

        return mpInfo["opts"];
}
