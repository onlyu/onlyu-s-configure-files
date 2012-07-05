# -*- coding: utf-8 -*-

import sys


#默认的模板
SRC_TEMPLATE = """
mapping data = ([ 
{% autoescape off %} 
{%for primary, data in table.items%}\
	"{{primary}}":([{%for key, value in data.items%}
		"{{key}}":{{value}},{%endfor%}
	]),
{%endfor%}\
{% endautoescape %} 
]);

mapping get_data()
{
	return data;
}
"""


if __name__ == "__main__":
	sys.path.append(sys.argv[1] + "tools/autocode/")
	PARSE = __import__("TemplateParse")
	UTIL = __import__("Util")
	ParseFile, ParseSheet, OutputFile = sys.argv[2:5]
	table = PARSE.DoParseVertical(ParseFile, ParseSheet)
	print table
	vars_map = {}
	vars_map["table"] = table
	#print vars_map
	#Content = UTIL.RenderTemplateString(SRC_TEMPLATE, vars_map)
	#PARSE.DoWrite(Content, OutputFile)
	#print Content
