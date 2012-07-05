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
{% endautoescape %} 
"""


if __name__ == "__main__":
	WorkDir, ParseFile, ParseSheet, OutputFile = sys.argv[1:5]
	sys.path.append(WorkDir + "tools/autocode/")
	PARSE = __import__("TemplateParse")
	UTIL = __import__("Util")
	table = PARSE.DoParseBeginEnd(ParseFile, ParseSheet)
	vars_map = {}
	vars_map["table"] = UTIL.PythonData2Lpc(table, True)
	#print vars_map
	Content = UTIL.RenderTemplateString(SRC_TEMPLATE, vars_map)
	PARSE.DoWrite(Content, WorkDir + OutputFile)
