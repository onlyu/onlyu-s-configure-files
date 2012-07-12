# -*- coding: utf-8 -*-

#ver 0.01 by canoe 2010-06-02, 以后功能多了，就分class实现，这样功能清晰

import os, sys
import locale
from django.template import Context, Template, loader
from django.core.management import setup_environ
import settings as dj_settings
import re

def RenderTemplateFile(template_name, vars_map):
	sys.path.append( os.getcwd())
	os.environ['DJANGO_SETTINGS_MODULE'] = 'settings'

	#os.environ['TZ'] = 'GMT-8'    # timezone
	outstr = loader.render_to_string(template_name, vars_map)
	return outstr

def RenderTemplateString(templatestr, vars_map):
	sys.path.append( os.getcwd())
	os.environ['DJANGO_SETTINGS_MODULE'] = 'settings'

	t = Template(templatestr)
	c = Context(vars_map)
	outstr = t.render(c)
	return outstr


#通过模板生成文件
def RenderOut( template_name, vars_map, output_file ):

	outstr = RenderTemplateFile(template_name, vars_map)

	f = file(output_file, "w")
	f.write(outstr.encode(locale.getdefaultlocale()[1]))
	f.close()

def RenderStrOut(templatestr, vars_map, output_file):

	outstr = RenderTemplateString(templatestr, vars_map)

	f = file(output_file, "w")
	f.write(outstr.encode(locale.getdefaultlocale()[1]))
	f.close()

def encode(val):
	import locale
	encoding = locale.getdefaultlocale()[1]
	res = ''
	try : 
		res = val.encode(encoding)
	except :
		res = val
	return res

#返回表的dict
#请注意格式是按cell的概念先行后列: dictRes[sheet_index][row][col] 和平时表述习惯不同
#sheet_id: 0 sh.name: 表格1 row: 7 col: 4 cellname: E8 value: this_cell = res[0][7][4]
def Xls2Dict(xls_name):
	import xlrd

	dictRes = {}
	book = xlrd.open_workbook(xls_name, formatting_info=True)

	for sheet_id in xrange(book.nsheets):
		sh = book.sheet_by_index(sheet_id)

		tmp_book_dict = {"name": sh.name, "nrows":sh.nrows, "ncols": sh.ncols, }

		for r in xrange(sh.nrows):
			tmp_row_dict = {}
			for c in xrange(sh.ncols):
				cell_value = sh.cell_value(r,c)
				cell_type  = sh.cell_type(r,c)
				tmp_row_dict[c] = { "type": cell_type, "value": cell_value }
				#print "sheet_id:", sheet_id,"sh.name:", sh.name, "row:", r, "col:", c, "cellname:", xlrd.cellname(r,c), "value:", cell_value
			tmp_book_dict[r] = tmp_row_dict

		dictRes[sheet_id] = tmp_book_dict

	return dictRes

def XlsSheet2Dict(xls_name, sheet_name = ""):
	import xlrd

	dictRes = {}
	book = xlrd.open_workbook(xls_name, formatting_info=True)

	for sheet_id in xrange(book.nsheets):
		sh = book.sheet_by_index(sheet_id)

		if len(sheet_name) and encode(sheet_name) != encode(sh.name):
			continue

		tmp_book_dict = {"name": sh.name, "nrows":sh.nrows, "ncols": sh.ncols, }
		for r in xrange(sh.nrows):
			tmp_row_dict = {}
			for c in xrange(sh.ncols):
				cell_value = sh.cell_value(r,c)
				cell_type  = sh.cell_type(r,c)
				tmp_row_dict[c] = { "type": cell_type, "value": cell_value }
				#print "sheet_id:", sheet_id,"sh.name:", sh.name, "row:", r, "col:", c, "cellname:", xlrd.cellname(r,c), "value:", cell_value
			tmp_book_dict[r] = tmp_row_dict
		return tmp_book_dict

	return dictRes


def ChineseEncode(name):
	if not isinstance(name, unicode):
		return name
	encoding = locale.getdefaultlocale()[1]
	try :
		res = name.encode(encoding)
	except :
		res = name
	return res	

indent_space = '        '
max_indent_cnt = 3


# 计算缩进
def getIndent( indentFlg, indentCnt):
	result = ''
	
	if not indentFlg:
		return result
	
	for i in range( 0, indentCnt):
		result = result + indent_space
		
	return result

# 将python dict 转换为lpc dict
def PythonDict2Lpc( data, indentFlg=False, indentCnt=0):
	
	if indentCnt > max_indent_cnt:
		indentFlg = False
		
	result = '(['
	if indentFlg :
		result = result + "\n"
	
	# 遍历 数据
	keys = data.keys()
	
	keys.sort()
	
	for key in keys:
		value = data[key]

		result = result + getIndent(indentFlg, indentCnt+1)

		strKey = (PythonData2Lpc(key,indentFlg, indentCnt+1))
		strValue = (PythonData2Lpc(value,indentFlg, indentCnt+1))
		
		result = result  + ("%s:%s, "%(strKey, strValue))
		if indentFlg:
			result = result + "\n"
	
	result = result + getIndent(indentFlg, indentCnt) + '])'
	return result

# 将python list 转换为lpc list
def PythonList2Lpc( data, indentFlg=False, indentCnt=0):
        if indentCnt > max_indent_cnt:
                indentFlg = False
        
        result = '({'
        
        if indentFlg:
                result = result + "\n"
        # 遍历 数据
	reslen = 0
        for value in data:
			tmpres = getIndent(indentFlg, indentCnt+1) + ("%s, "%(PythonData2Lpc(value,indentFlg, indentCnt+1)))
			result = result + tmpres
			if indentFlg:
				result = result + "\n"
			reslen += len(tmpres)
			if reslen > 200:
				reslen = 0
				result = result + "\n"
                
        result = result + getIndent(indentFlg, indentCnt) + '})'
        return result


# 将python tuple 转换为lpc tuple
def PythonTuple2Lpc( data, indentFlg=False, indentCnt=0):
        return PythonList2Lpc(data, indentFlg, indentCnt)
        
# 将python数据转换为lpc数据
def PythonData2Lpc( data, indentFlg=False, indentCnt=0):
	if isinstance(data, str):
		data = data.decode("utf-8")
		if data.startswith(u"@@"):
			return '%s'%(data[2:len(data)])
		return '"%s"'%(data)
        elif isinstance(data, unicode):
		if data.startswith(u"@@"):
			return '%s'%(data[2:len(data)])
                return '"%s"'%(data)
        elif isinstance(data, int):
                return "%d"%data
        elif isinstance(data, float):
                return "%f"%data
        elif isinstance(data, list):
                return PythonList2Lpc( data, indentFlg, indentCnt)
        elif isinstance(data, tuple):
                return PythonList2Lpc( data, indentFlg, indentCnt)
        elif isinstance(data, dict):
                return PythonDict2Lpc( data, indentFlg, indentCnt)

# 将python dict 转换为lua dict
def PythonDict2Lua( data, indentFlg=False, indentCnt=0):
	if indentCnt > max_indent_cnt:
		indentFlg = False

	result = '{'
	if indentFlg :
		result = result + "\n"
	# 遍历 数据
	keys = data.keys()
	keys.sort()

	for key in keys:
		value = data[key]
		result = result + getIndent(indentFlg, indentCnt+1)

		strKey = (PythonData2Lua(key,indentFlg, indentCnt+1))
		strValue = (PythonData2Lua(value,indentFlg, indentCnt+1))
		result = result  + ("[%s] = %s, "%(strKey, strValue))
		if indentFlg:
			result = result + "\n"
	result = result + getIndent(indentFlg, indentCnt) + '}'
	return result

# 将python list 转换为lua list
def PythonList2Lua(data, indentFlg=False, indentCnt=0):
	if indentCnt > max_indent_cnt:
		indentFlg = False

	result = '{' 
	if indentFlg:
		result = result + "\n"
        # 遍历 数据
	index = 0
	while index < len(data):
		value = data[index]
		strValue = PythonData2Lua(value,indentFlg, indentCnt+1)
		result = result  + ("%s, "%(strValue))
		if indentFlg:
			result = result + "\n"
		index += 1
	result = result + getIndent(indentFlg, indentCnt) + '}'
	return result


# 将python tuple 转换为lua tuple
def PythonTuple2Lua( data, indentFlg=False, indentCnt=0):
        return PythonList2Lua(data, indentFlg, indentCnt)

# 将python数据转换为lua数据
def PythonData2Lua( data, indentFlg=False, indentCnt=0):
	if isinstance(data, str):
		data = data.decode("utf-8")
		if data.startswith(u"@@"):
			return "%s"%(data[2:len(data)])
		return "'%s'"%(data)
	elif isinstance(data, unicode):
		if data.startswith(u"@@"):
			return "%s"%(data[2:len(data)])
		return "'%s'"%(data)
	elif isinstance(data, int):
		return "%d"%data
	elif isinstance(data, float):
		return "%f"%data
	elif isinstance(data, list):
		return PythonList2Lua( data, indentFlg, indentCnt)
	elif isinstance(data, tuple):
		return PythonList2Lua( data, indentFlg, indentCnt)
	elif isinstance(data, dict):
		return PythonDict2Lua( data, indentFlg, indentCnt)


# 将python dict 转换为json dict
def PythonDict2Json( data, indentFlg=False, indentCnt=0):
	if indentCnt > max_indent_cnt:
		indentFlg = False

	result = '{'
	if indentFlg :
		result = result + "\n"
	# 遍历 数据
	keys = data.keys()
	keys.sort()

	content = ""
	for key in keys:
		value = data[key]
		if len(content):
			content += ","
			if indentFlg:
				content = content + "\n"
		content = content + getIndent(indentFlg, indentCnt+1)

		strKey = (PythonData2Json(key,indentFlg, indentCnt+1))
		strValue = (PythonData2Json(value,indentFlg, indentCnt+1))
		content = content  + ("%s : %s"%(strKey, strValue))
	result = result + content + "\n" + getIndent(indentFlg, indentCnt) + '}'
	return result

# 将python list 转换为Json list
def PythonList2Json(data, indentFlg=False, indentCnt=0):
	if indentCnt > max_indent_cnt:
		indentFlg = False

	result = '[' 
	if indentFlg:
		result = result + "\n"
        # 遍历 数据
	index = 0
	content = ""
	while index < len(data):
		value = data[index]
		strValue = PythonData2Json(value,indentFlg, indentCnt+1)
		if len(content):
			content += ","
		content = content  + ("%s"%(strValue))
		if indentFlg:
			content = content + "\n"
		index += 1
	result = result + content + getIndent(indentFlg, indentCnt) + ']'
	return result


# 将python tuple 转换为lua tuple
def PythonTuple2Json( data, indentFlg=False, indentCnt=0):
        return PythonList2Json(data, indentFlg, indentCnt)

# 将python数据转换为lua数据
def PythonData2Json( data, indentFlg=False, indentCnt=0):
	if isinstance(data, str):
		data = data.decode("utf-8")
		if data.startswith(u"@@"):
			return "%s"%(data[2:len(data)])
		return "\"%s\""%(data)
	elif isinstance(data, unicode):
		if data.startswith(u"@@"):
			return "%s"%(data[2:len(data)])
		return "\"%s\""%(data)
	elif isinstance(data, int):
		return "%d"%data
	elif isinstance(data, float):
		return "%f"%data
	elif isinstance(data, list):
		return PythonList2Json( data, indentFlg, indentCnt)
	elif isinstance(data, tuple):
		return PythonList2Json( data, indentFlg, indentCnt)
	elif isinstance(data, dict):
		return PythonDict2Json( data, indentFlg, indentCnt)

def WriteUpdateFile(WorkDir, UpdateFile):
	filename = WorkDir + "/tmp/update_file.txt"
	try :
		f = file(filename, "a+b")
	except :
		sys.exit(-1)
	f.write(UpdateFile)
	f.close()

def write_file(filename, content ):
	msg = "writting to file " + filename
	try :
		f = file(filename, "w+b")
	except :
		msg = "can not write to " + filename + "\n"
		sys.exit(-1)
	f.write(content)
	f.close()

def DoWriteKeepManual(OutputFile, Content, InputFile="undefined"):
	begin = r"// -------------------  Manual Generate Begin --------------------"
	end   = r"// -------------------  Manual Generate End   --------------------"
	p = re.compile(begin + r".*?" + end, re.S | re.M)

	Index = InputFile.find('tmp')
	if Index > 0:
		_InputFile = InputFile[Index:len(InputFile)]
	else :
		_InputFile = InputFile

	if isinstance(Content, unicode):
		Content = Content.encode(locale.getdefaultlocale()[1])
		Content += "// from "
		Content += _InputFile
	try:
		src_data = open(OutputFile, "rb").read()
	except IOError:
		src_data = begin + "\n\n" + end + "\n"

	src_str = p.search(src_data).group()

	Content = p.sub(src_str, Content)

	f = file(OutputFile, "w")
	f.write(Content)
	f.close()



def WriteClientFileContent(work_dir, client_file, content):
	svn_path = "https://192.168.0.3/qtz/wg01/design/data/client_data"
	svn_user_auth = " --username fs_autoparse_design --password \!QA2ws3ed --no-auth-cache"
	svn_tmp_path = "tmp/"

	content = encode(content);

	#切换到tmp目录
	os.chdir(work_dir + svn_tmp_path)

	if not client_file.endswith(".as"):
		client_file = client_file + ".as"

	export_svn_dir = svn_path
	export_dir = os.path.basename(svn_path)
	work_export_dir = work_dir + svn_tmp_path + export_dir
	work_export_file = work_export_dir + "/" + client_file

	re_init = 0
	if os.path.isdir(export_dir) and os.path.isfile(work_export_file):
		os.chdir(work_export_dir)
		os.system("svn up" + svn_user_auth)
		DoWriteKeepManual(work_export_file, content)
		#write_file(work_export_file, content)
	else:
		os.system("rm -rf " + export_dir)
		os.system("svn co " + export_svn_dir + " " + export_dir + svn_user_auth)
		os.chdir(work_export_dir)
		DoWriteKeepManual(work_export_file, content)
		#write_file(work_export_file, content)

		#svn add
		os.system("svn add " + client_file + svn_user_auth)
		re_init = 1

	os.system("svn ci -m " + "\"" + "导表更新 from " + work_dir + "\"" + svn_user_auth)

#返回目标路径
def WriteClientFile(work_dir, client_file, tblkey, pytbl):
	content = "%s = %s"%(tblkey, PythonData2Lua(pytbl, True))
	WriteClientFileContent(work_dir, client_file, content)


if __name__ == "__main__":
	#test
	WriteClientFile("/home/chenyh/work/trunk/logic/", "xxxx", "test", {"a" : 1, "b" : 2})

def HasSheet(ParseFile, SheetName):
	resDict = XlsSheet2Dict(ParseFile, SheetName)
	if len(resDict):
		return True
	return False


