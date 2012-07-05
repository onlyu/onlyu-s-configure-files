# -*- coding: utf-8 -*-

import sys

#默认的模板
SRC_TEMPLATE = """
mapping data = ([ 
{% autoescape off %} 
{%for row, data in table.items%}\
	{{data.PrimaryKey}}:([{%for key, value in data.Cols.items%}"{{key}}":{{value}},{%endfor%}]),
{%endfor%}\
{% endautoescape %} 
]);

mapping get_data()
{
	return data;
}
"""

# 本模块为通用的导表程序，如果需要自己定制，则配置一个table_meta
# 并调用PARSE.DoParse()

# 默认的配置
DefaultTableMeta = {
	"KeyRow": 1,
	"ParseRow": 2,
	"PrimaryKeyCol": 0,
	"TemplateSrc": SRC_TEMPLATE,
	#"udata": udata,
	#"Template": xxx.template        #模板
	}

"""
output	TRUE
file_name	title
key_row	10
start_row	11
dict_name	data.title_config
description	场景
"""

def ReadTblMeta(SheetDict):
	table_meta = {}
	start_row = 0
	for row in range(0, SheetDict["nrows"], 1):
		key = SheetDict[row][0]["value"]
		value = SheetDict[row][1]["value"]
		if key == u'key_row':
			table_meta["KeyRow"] = int(float(value)) - 1
			continue
		if key == u'start_row':
			table_meta["ParseRow"] = int(float(value)) - 1
			start_row = int(float(value))
			continue
		if start_row and row > start_row:
			break
	return table_meta

if __name__ == "__main__":
	WorkDir, ParseFile, ParseSheet, OutputFile = sys.argv[1:5]
	sys.path.append(WorkDir + "tools/autocode/")
	PARSE = __import__("TemplateParse")
	resDict = PARSE.XlsSheet2Dict(ParseFile, ParseSheet)
	table_meta = ReadTblMeta(resDict)
	table_meta["PrimaryKeyCol"] = 0
	table_meta["TemplateSrc"] = SRC_TEMPLATE
	print table_meta
	PARSE.DoParseNormalWrite(table_meta, ParseFile, ParseSheet, WorkDir + OutputFile)
