# -*- coding: utf-8 -*-

import sys
import cPickle

#默认的模板
SRC_TEMPLATE = """
mapping data = ([ 
{% autoescape off %} 
{%for row, data in table.items%}\
	{{data.PrimaryKey}}:([{%for key, value in data.Cols.items%} 
	{% if key|slice:":2" == "@@" %}{{key|slice:"2:"}}:{{value}}, \
{% else %} {% if key|slice:":2" == "K_" or key|slice:":2" == "G_" or key|slice:":3" == "KT_" %} {{key}}:{{value}},\
{% else %} "{{key}}":{{value}},{% endif %}{% endif %} \
{%endfor%}]),
{%endfor%}\
{% endautoescape %} 
]);

mapping get_data()
{
	return data;
}
"""

"""
data[row] = {
		"PrimaryKey": value,
		"Cols": {
			"key": value0,
			"key1": value1,
			}
		}
"""

#默认的模板
SRC_CLIENT_TEMPLATE = """
{% autoescape off %} 
package data.client_data {

	public class {{class_name}}
	{
// -------------------  Manual Generate Begin --------------------
		// todo
// -------------------  Manual Generate End   --------------------
		public function {{class_name}}()
		{

		}

		public static const data:* = {{content}};

	}
}
{% endautoescape %} 
"""

"""
data[row] = {
		"PrimaryKey": value,
		"Cols": {
			"key": value0,
			"key1": value1,
			}
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
	"DictName" : "xxx",   #CLIENT
	"Output" : True
	#"udata": udata,
	#"Template": xxx.template        #模板
	}


def ReadTblMeta(SheetDict):
	table_meta = {}
	start_row = 0
	PARSE = __import__("TemplateParse")

	for row in range(0, SheetDict["nrows"], 1):
		key = SheetDict[row][0]["value"]
		value = SheetDict[row][1]["value"]
		if key == u'output':
			table_meta["Output"] = True
			if SheetDict[row].has_key(2):
				value = SheetDict[row][2]["value"]
				if value == 0:
					table_meta["Output"] = False
			continue
		if key == u'key_row':
			if SheetDict[row].has_key(2):
				str_value = SheetDict[row][2]["value"]
				tmp_value = PARSE.do_parse_value("int", str_value)
				if tmp_value:
					value = tmp_value
			table_meta["KeyRow"] = int(float(value)) - 1
			continue
		if key == u'start_row':
			table_meta["ParseRow"] = int(float(value)) - 1
			start_row = int(float(value))
			continue

		if start_row and row > start_row:
			break
	if len(table_meta):
		table_meta["PrimaryKeyCol"] = 0
		table_meta["TemplateSrc"] = SRC_TEMPLATE
		return table_meta
	else:
		return DefaultTableMeta


DefaultClientTableMeta = {
		"output" : ["TRUE", "string"],
		"file_name" : ["test", "string"],
		"key_row" : [10, "int"],
		"start_row"	: [11, "int"],
		"class_name"	: ["test_cls", "string"],
		"description" : ["心法客户端配置表", "string"],
		}

def ReadTblClientMeta(SheetDict):
	table_meta = {}
	start_row = 0
	PARSE = __import__("TemplateParse")

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
		if key == u'output':
			table_meta["Output"] = True
			if value == "FALSE" or not value:
				table_meta["Output"] = False
			continue
		if key in DefaultClientTableMeta.keys():
			table_meta[key] = PARSE.do_parse_value(DefaultClientTableMeta[key][1], value);
			continue
		if start_row and row > start_row:
			break
	if len(table_meta):
		table_meta["PrimaryKeyCol"] = 0
		table_meta["TemplateSrc"] = SRC_CLIENT_TEMPLATE
	return table_meta

if __name__ == "__main__":
	WorkDir, ParseFile, ParseSheet, OutputFile = sys.argv[1:5]
	sys.path.append(WorkDir + "tools/autocode/")
	PARSE = __import__("TemplateParse")
	UTIL = __import__("Util")
	resDict = PARSE.XlsSheet2Dict(ParseFile, ParseSheet)
	table_meta = ReadTblMeta(resDict)
	if len(table_meta) and table_meta["Output"]:
		PARSE.DoParseNormalWrite(table_meta, ParseFile, ParseSheet, WorkDir + OutputFile, resDict)

	table_client_meta = ReadTblClientMeta(resDict)
	if len(table_client_meta) and table_client_meta["Output"]:
		table = PARSE.DoParseNormal2(table_client_meta, ParseFile, ParseSheet)
		vars_map = {}
		vars_map["content"] = UTIL.PythonData2Json(table, True, 1)
		vars_map["class_name"] = table_client_meta["file_name"]
		Content = UTIL.RenderTemplateString(table_client_meta["TemplateSrc"], vars_map)
		UTIL.WriteClientFileContent(WorkDir, table_client_meta["file_name"], Content)
