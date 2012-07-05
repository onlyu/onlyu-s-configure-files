# -*- coding: utf-8 -*-

import sys

#默认的模板
SRC_TEMPLATE = """
{% autoescape off %} 

mapping data =
{{table}}
;

mapping get_data()
{
	return data;
}

mixed GetConfig(mixed table, mixed item, mixed prop)
{
	if (undefinedp(data[table])) return 0;
	if (undefinedp(data[table][item])) return 0;
	if (undefinedp(data[table][item][prop])) return 0;
	return data[table][item][prop];
}

mixed GetMapping(mixed name)
{
	if (undefinedp(data[name])) return ([]);
	return data[name];
}
{% endautoescape %} 
"""


if __name__ == "__main__":
	WorkDir, ParseFile, ParseSheet, OutputFile = sys.argv[1:5]
	sys.path.append(WorkDir + "tools/autocode/")
	PARSE = __import__("TemplateParse")
	UTIL = __import__("Util")
	table = PARSE.DoParseBeginEnd2(ParseFile, ParseSheet)
	vars_map = {}
	vars_map["table"] = UTIL.PythonData2Lpc(table, True)
	#print vars_map
	Content = UTIL.RenderTemplateString(SRC_TEMPLATE, vars_map)
	PARSE.DoWrite(Content, WorkDir + OutputFile, ParseFile)
