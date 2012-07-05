# -*- coding: utf-8 -*-

import sys
import os

# 默认的配置
DefaultTableMeta = {
	"KeyRow": 1,
	"ParseRow": 2,
	"PrimaryKeyCol": 0,
}

global vars_all_items

DEFAULT_ITEM_DIR = "char/item/"
DEFAULT_INHERIT_ITEM = "ITEM_BASE"

SRC_ITEM_TEMPLATE = u"""

#include <item.h>
#include <item_key.h>
#include <macros.h>
#include <equip.h>
#include <common_key.h>
#include <baoshi.h>

{% if inherit %}\
inherit {{inherit}};
{% endif %}

mapping data = ([
{% autoescape off %} 
{%for key, value in data_var.items%}\
	{% if key|slice:":2" == "@@" %}{{key|slice:"2:"}}:{{value}}, \
{% else %} {% if key|slice:":2" == "K_" or key|slice:":2" == "G_" or key|slice:":3" == "KT_" %} {{key}}:{{value}},\
{% else %} "{{key}}":{{value}},{% endif %}{% endif %}
{%endfor%}
{% endautoescape %} 
]);

mapping getData()
{
	return data;
}

{% autoescape off %} 
void InitItem(int itemid) 
{
{%for key, value in save_var.items%}\
	{% if "K_" in key %}SetItemSave(itemid, {{key}}, {{value}});\
{% else %}SetItemSave(itemid, "{{key}}", {{value}});{% endif %}
{%endfor%}\

{%for key, value in temp_var.items%}\
	{% if "KT_" in key %}SetItemTemp(itemid, {{key}}, {{value}});\
{% else %}SetItemTemp(itemid, "{{key}}", {{value}});{% endif %}
{%endfor%}\
	::InitItem(itemid);
}
{% endautoescape %} 

"""

SRC_ALL_ITEM_TEMPLATE = u"""
mapping data = ([ 
{% autoescape off %} 
{%for primarykey, data in table.items%}\
	{{primarykey}}:([{%for key, value in data.items%}
			{% if key|slice:":2" == "@@" %}{{key|slice:"2:"}}:{{value}}, \
{% else %} {% if key|slice:":2" == "K_" or key|slice:":2" == "G_" or key|slice:":3" == "KT_" %} {{key}}:{{value}},\
{% else %} "{{key}}":{{value}},{% endif %}{% endif %}{%endfor%} 
		]),
{%endfor%}\
{% endautoescape %} 
]);

mapping getData()
{
	return data;
}
"""

SaveKeyTable = {
}

def parse_normal_item(WorkDir, ParseFile, inherit, table):
	ItemDir = "%s/%s"%(WorkDir, DEFAULT_ITEM_DIR)
	global vars_all_items

	vars_map = {}

	if len(inherit):
		vars_map["inherit"] = inherit;
	else:
		vars_map["inherit"] = DEFAULT_INHERIT_ITEM

	for _row in table["table"].keys():
		colTable = table["table"][_row]["Cols"]
		primaryKey = table["table"][_row]["PrimaryKey"]
		vars_map = {}
		vars_map["inherit"] = inherit
		vars_map["temp_var"] = {}
		vars_map["save_var"] = {}
		vars_map["data_var"] = {}

		itemType = primaryKey[1:-1]

		vars_map["save_var"]["K_ITEM_TYPE"] = primaryKey

		vars_all_items["table"][primaryKey] = {}

		for _key in colTable.keys():
			_val = colTable[_key]

			if _key == "KT_ITEM_NAME":
				vars_all_items["table"][primaryKey]["G_ITEM_NAME"] = _val

			if _key.startswith("G_"):
				vars_all_items["table"][primaryKey][_key] = _val
				continue

			if _key.startswith("KT_ITEM_") or _key.startswith("K_ITEM_"):
				if _key.startswith("K_"):
					vars_map["save_var"][_key] = _val
				else:
					vars_map["temp_var"][_key] = _val
			else:
				vars_map["data_var"][_key] = _val


		Content = UTIL.RenderTemplateString(SRC_ITEM_TEMPLATE, vars_map) 
		filename = ItemDir + itemType + ".c"
		PARSE.DoWrite(Content, filename, ParseFile)
		UTIL.WriteUpdateFile(WorkDir, itemType + ".c" + "\n")

ParseSheets = {
		u"装备表" : { "parse": parse_normal_item, "inherit": "EQUIP_BASE", },
		u"杂货表" : { "parse": parse_normal_item, "inherit": "ITEM_BASE", },
		u"装备图谱表" : { "parse": parse_normal_item, "inherit": "EQUIP_TUPU_BASE", },
		u"装备材料表" : { "parse": parse_normal_item, "inherit": "EQUIP_CAILIAO_BASE", },
		u"宝石表" : { "parse": parse_normal_item, "inherit": "EQUIP_BAOSHI_BASE", },
}

if __name__ == "__main__":
	global vars_all_items

	WorkDir, ParseFile = sys.argv[1:3]
	sys.path.append(WorkDir + "tools/autocode/")
	PARSE = __import__("TemplateParse")
	UTIL = __import__("Util")
	COMMON_PARSE = __import__("common_parse")

	vars_all_items = {}
	vars_all_items["table"] = {}

	client_all_items_table = {}

	for _sheet in ParseSheets.keys():
		if UTIL.HasSheet(ParseFile, _sheet):
			resDict = PARSE.XlsSheet2Dict(ParseFile, _sheet)
			metaTable = COMMON_PARSE.ReadTblMeta(resDict)
			inittable, _ = PARSE.DoParseNormal(metaTable, ParseFile, _sheet)
			ParseSheets[_sheet]["parse"](WorkDir, ParseFile, ParseSheets[_sheet]["inherit"], inittable)

			#client
			table_client_meta = COMMON_PARSE.ReadTblClientMeta(resDict)
			table = PARSE.DoParseNormal2(table_client_meta, ParseFile, _sheet)
			client_all_items_table.update(table);
				
	vars_map = {}
	vars_map["content"] = UTIL.PythonData2Json(client_all_items_table, True, 1)
	vars_map["class_name"] = "AllItemsCFG"
	ClientContent = UTIL.RenderTemplateString(table_client_meta["TemplateSrc"], vars_map)
	UTIL.WriteClientFileContent(WorkDir, "AllItemsCFG", ClientContent)


	Content = UTIL.RenderTemplateString(SRC_ALL_ITEM_TEMPLATE, vars_all_items) 
	filename = WorkDir + "data/all_items_data.c"
	PARSE.DoWrite(Content, filename, ParseFile)
