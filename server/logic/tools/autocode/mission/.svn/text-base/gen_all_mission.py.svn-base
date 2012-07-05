# -*- coding: utf-8 -*-

import sys
import os

# 默认的配置
DefaultTableMeta = {
        "MetaRow": 0,
	"KeyRow": 1,
	"ParseRow": 2,
	"PrimaryKeyCol": 0,
}

DEFAULT_ITEM_DIR = "module/mission/missions/"
PRELOAD_FILE_DIR = "module/mission/preload.c"

SRC_ITEM_TEMPLATE = u"""

#include "/module/mission/mission.h"
inherit {{inherit}};
#include <var_prop.h>

{% autoescape off %} 
{%for key, value in vars.items%}\
{%for mkey, mvalue in meta.items%}\
{%if key == mkey %}\
// {{mvalue}}
{%endif%}\
{%endfor%}\
RESET_ONUPDATE_VAR({{key}}, {{value}})
{%endfor%}\

{% endautoescape %} 

"""

SRC_PRELOAD_TEMPLATE = u"""

{% autoescape off %} 

static string *allMission = ({
{%for missId in missions%}\
        "{{missId}}",
{%endfor%}\
});

string *getData()
{
        return allMission;
}

{% endautoescape %} 
"""


def parse_normal_item(WorkDir, ParseFile, table, meta):
	ItemDir = "%s/%s"%(WorkDir, DEFAULT_ITEM_DIR)
	vars_map = {}
        missions = []

	for _row in table["table"].keys():
		colTable = table["table"][_row]["Cols"]
		primaryKey = table["table"][_row]["PrimaryKey"]
		vars_map = {}
                vars_map["vars"] = {"Id": primaryKey}

		itemType = primaryKey[1:-1]
                missions.append(itemType)

		for _key in colTable.keys():
			_val = colTable[_key]
                        vars_map["vars"][_key] = _val

                vars_map["meta"] = meta
                vars_map["inherit"] = vars_map["vars"]["Template"]

		Content = UTIL.RenderTemplateString(SRC_ITEM_TEMPLATE, vars_map) 
		filename = ItemDir + itemType + ".c"
		PARSE.DoWrite(Content, filename, ParseFile)

        return missions


ParseSheets = {
		u"例子" : { "parse": parse_normal_item, },
}

if __name__ == "__main__":
	WorkDir, ParseFile = sys.argv[1:3]
	sys.path.append(WorkDir + "tools/autocode/")
	PARSE = __import__("TemplateParse")
	UTIL = __import__("Util")
        preload = {"missions": []}

	import xlrd
	book = xlrd.open_workbook(ParseFile, formatting_info=True)
	for sheet_id in xrange(book.nsheets):
		sh = book.sheet_by_index(sheet_id)
		inittable, meta = PARSE.DoParseNormal(DefaultTableMeta, ParseFile, sh.name)
		preload["missions"] += parse_normal_item(WorkDir, ParseFile, inittable, meta)

	"""
	for _sheet in ParseSheets.keys():
		if UTIL.HasSheet(ParseFile, _sheet):
			inittable, meta = PARSE.DoParseNormal(DefaultTableMeta, ParseFile, _sheet)
			preload["missions"] += ParseSheets[_sheet]["parse"](WorkDir, ParseFile, inittable, meta)
	"""

        Content = UTIL.RenderTemplateString(SRC_PRELOAD_TEMPLATE, preload) 
        PARSE.DoWrite(Content, PRELOAD_FILE_DIR, ParseFile)

