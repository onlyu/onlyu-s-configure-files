#ifndef  __MENU_H__
#define  __MENU_H__


mapping initNpcOptions(int npcId)
{
	return Import("MENU")->initNpcOptions(npcId);
}

void addOption(mapping options, mapping option)
{
        Import("MENU")->addOption(options, option);
}

// 文本选项
mapping createOption(string optMsg, string func, mixed *args)
{
	return Import("MENU")->createOption(optMsg, base_name(this_object()), func, args);
}

// 按钮选项
mapping createButtonOption(string optMsg, string func, mixed *args)
{
	return Import("MENU")->createButtonOption(optMsg, base_name(this_object()), func, args);
}

// 默认选项(客户端右下角的默认操作按钮)
mapping createDefaultOption(string optMsg, string func, mixed *args)
{
	return Import("MENU")->createDefaultOption(optMsg, base_name(this_object()), func, args);
}

// 文本输入
mapping createTextInputOption(string optMsg, string key)
{
	return Import("MENU")->createTextInputOption(optMsg, key);
}

// 数字输入
varargs mapping createNumInputOption(string optMsg, string key, int lowLimit, int upLimit, int defaultValue)
{
	return Import("MENU")->createNumInputOption(optMsg, key, lowLimit, upLimit, defaultValue);
}

// 奖励信息
mapping createRewardOption(int money, int exp, mapping items)
{
        return Import("MENU")->createRewardOption(money, exp, items);
}

// 获取用户输入
mixed optionsGetInput(mixed user, string key)
{
	return Import("MENU")->optionsGetInput(user, key);
}

void tellOptions(object user, mapping options, string title)
{
	Import("MENU")->tellOptions(user, options/*, base_name(this_object())*/, title);
}

#endif   // __MENU_H__
