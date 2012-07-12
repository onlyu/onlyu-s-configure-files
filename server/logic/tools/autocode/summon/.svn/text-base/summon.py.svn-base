
# -*- coding: utf-8 -*-
"""
召唤兽信息生成程序
"""
import xlrd
import sys
import os
import glob
import getopt
import re
import string
import codecs
from Python2Lpc import PythonData2Lpc

QinmiLvlDict = {u"漠视":1,u"冷淡":2,u"普通":3,u"友好":4}


def write_file(filename, content ):
	msg = "writting to file",filename

	try :
		f = file(filename, "w+b")
	except :
		msg = "can not write to", filename
		sys.exit(-1)
	
	f.write(content.encode('gbk'))	
	f.close()


begin = "// -------------------  Auto Generate Begin --------------------"
end   = "// -------------------  Auto Generate End   --------------------\n"
replace_pattern  = re.compile(begin + r".*?" + end, re.S | re.M)

def get_float_from_sheet( sheet, row, col ):
	value = sheet.cell_value(rowx=row, colx=col)
	return float(value)

def get_int_from_sheet( sheet, row, col ):
	value = sheet.cell_value(rowx=row, colx=col)
	return int(value)
	
def get_str_from_sheet( sheet, row, col ):
	value = sheet.cell_value(rowx=row, colx=col)
	
	if  isinstance(value, float):
		return str(int(value))
	
	if  isinstance(value, int):
		return str(value)

	return value

		
def usage():
	print "USAGE:summon.py root_path excel_file 输出文件名 "

dicColInfo = {
u"名称":"string",
u"造型编号":"int",
u"招将台等级":"int",
u"系别":"string",
u"初始亲密等级":"string",
u"初始耐力":"int",
u"初始武力":"int",
u"初始智力":"int",
u"初始敏捷":"int",
u"初始耐力成长":"float",
u"初始武力成长":"float",
u"初始智力成长":"float",
u"初始敏捷成长":"float",
}

def parse_data_sheet(sh, summonInfo):
	startRow = 0
	for i in range(0, sh.nrows):
		# 名称
		value = get_str_from_sheet( sh, i, 0 )

		if ( value == u'名称'):
			startRow = i
			break;

	titleRow = startRow
	titleKeyRow = startRow + 1

	for i in range(startRow+3, sh.nrows):
		id = get_int_from_sheet( sh, i, 1 )	
		tmpTbl = {}
		summonInfo[id] = tmpTbl

		for j in range( 0, sh.ncols):
			title = get_str_from_sheet( sh, titleRow, j ).strip()

			if title == u"初始亲密等级":
				data = get_str_from_sheet( sh, i, j )	
				tmpTbl[get_str_from_sheet( sh, titleKeyRow, j )] = QinmiLvlDict[data]; 
				continue;

			if title in dicColInfo:
				dataInfo = dicColInfo[title]

				if dataInfo == "int":
					tmpTbl[get_str_from_sheet( sh, titleKeyRow, j )] = get_int_from_sheet( sh, i, j )
				elif dataInfo == "float":
					tmpTbl[get_str_from_sheet( sh, titleKeyRow, j )] = get_float_from_sheet( sh, i, j )
				elif dataInfo == "string":
					tmpTbl[get_str_from_sheet( sh, titleKeyRow, j )] = get_str_from_sheet( sh, i, j )


def parse_xls(filename, sheet,outputfile):
	try :
		book = xlrd.open_workbook(filename)
	except :
		msg = "can't open file?", filename
		print msg
		usage()
		sys.exit(-1)

	summonInfo = {}
	
	for x in xrange(book.nsheets):
		sh = book.sheet_by_index(x)
		print sh.name.encode('gbk')
		# 不处理说明表格
		if sh.name != sheet: 
			continue

		# 其他表格解析
		parse_data_sheet(sh, summonInfo)
	
	# 尝试读取outputfile
	try:
		file = codecs.open(outputfile, "rb", "gbk")
		lpc_src = file.read()
		if ( len( lpc_src ) == 0):
			lpc_src = begin + "\n\n" + end
		file.close()
	except IOError:
		lpc_src = begin + "\n\n" + end
	
	#print summonInfo
	new_src = PythonData2Lpc(summonInfo, True, 1)  
	tmp = replace_pattern.sub(begin + "\n" + new_src + "\n" + end, lpc_src)
	
	write_file( outputfile, tmp )



def main():
	try:
		opts, args = getopt.getopt(sys.argv[1:], "ho:v", ["help", "output="])
		for o, a in opts:
			if o == "-v":
				print version	
				sys.exit()
			elif o in ("-h", "--help"):
				usage()
				sys.exit()
			elif o in ("-o", "--output"):
				output = a
			else:
				assert False, "unhandled option"
		if (len(args) < 4):
			usage()
			sys.exit(-1)
		rootPath = args[0]
		excelFile = args[1]	
		excelSheet = args[2].decode("gb18030")
		outputFile = rootPath + args[3]	
	
	except getopt.GetoptError, err:
		# print help information and exit:
		print str(err) # will print something like "option -a not recognized"
		usage()
		raise
		#sys.exit(2)

	print excelFile,outputFile

	parse_xls( excelFile, excelSheet, outputFile)


if __name__ == '__main__':
	#gen_allsummon_info()
	#gen_skill_upgrade_tbl()
	print "summon main"
	main()
