# -*- coding: utf-8 -*-
import os
import sys
import xlrd

CONTENT_TEMPLATE_SRC = """
mapping data = %s;

mapping get_data()
{
	return data;
}
"""

if __name__ == "__main__":
	WorkDir, ParseFile, OutputFile = sys.argv[1:5]
	sys.path.append(WorkDir + "tools/autocode/")
	PARSE = __import__("TemplateParse")
	UTIL = __import__("Util")
	COMMON = __import__("common_parse")
	table_meta = COMMON.DefaultTableMeta

	book = xlrd.open_workbook(ParseFile)
	total = {}

	for x in xrange(book.nsheets):
		sh = book.sheet_by_index(x)
		# 不处理说明表格
		if sh.name == u"说明":
			continue


		if len(table_meta):
			table = PARSE.DoParseNormal2(table_meta, ParseFile, sh.name)
			drama = []
			for i,row in table.iteritems():
				stepIndex = row["step"]
				if len(drama) < stepIndex:
					drama.append([])
				del row["step"]
				drama[stepIndex-1].append(row)

			total[sh.name] = drama


	content = UTIL.PythonData2Lpc(total, True, 1)
	content = CONTENT_TEMPLATE_SRC % (content)
	PARSE.DoWrite(content, os.path.join(WorkDir, OutputFile), ParseFile)

