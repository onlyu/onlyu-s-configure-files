# -*- coding: utf-8 -*- 

import sys
import Util
import xlrd
import os
import locale
import string
import re

macros = {}
gMappingParse = False

def encode(val):
	import locale
	encoding = locale.getdefaultlocale()[1]
	res = ''
	try : 
		res = val.encode(encoding)
	except :
		res = val
	return res

def parse_int(value):
	ret = 0

	if  isinstance(value, float):
		if value > 0:
			return int(value+0.5)
		return int(value)
	
	if  isinstance(value, int):
		return value

	value = value.strip()

	if  value == '':
		return 0
	
	if  isinstance(value, basestring):
		return int(float(value))
	return ret

def parse_float(value):
	if  isinstance(value, float):
		return value
	return parse_int(value)

'''
	print "___________________", value
	if  isinstance(value, float):
		strval = "%f"%(value)
	elif  isinstance(value, int):
		strval = "%d"%(value)
	elif  value == '':
		return 0
	print "______________", strval
	ret = string.atoi(strval)
	print "_________________end:", ret
'''
	
def parse_string(value):
	try :
		if isinstance(value, float):
			ret = '%d'%(int(float(value)))
		elif isinstance(value, int):
			ret = str(value)
		else:
			ret = value.strip();
	except :
		ret = value.strip()
	return ret.strip().replace('"', '\\"')

def parse_bool(val):
	if val == u"是":
		return 1
	return 0

def del_quot(val):
	if len(val) > 2 and val[0] == '"' and val[len(val) - 1] == '"':
		return val[1:(len(val) - 1)]
	return val

def parse_map(val):
	val = val.strip()
	val = val.replace(u"，", ",");
	val = val.replace(u"：", ":");

	try:
		ret = eval(u"{" + val + u"}")
	except:
		kv = val.split(",")
		ret = {}
		for el in kv:
			if el.find(":") > 0:
				kvs = el.split(":")
				key = del_quot(kvs[0])
				ret[key] = eval(kvs[1])
			else:
				key = del_quot(el)
				if len(key):
					ret[key] = 0
	return ret

def parse_mixed(val):
	if  isinstance(val, float):
		#if val > 0:
		#	return int(val+0.5)
		#return int(val)
		return val
	if  isinstance(val, int):
		return val
	val = val.strip()
	try :
		val = val.replace(u"，", ",");
		val = val.replace(u"：", ":");
		return eval(val)
	except:
		return val

def parse_array(val):
	if isinstance(val, str) or isinstance(val, unicode):
		val = val.strip()
		if not len(val):
			return []
	else:
		val = "%d,"%val
	encoding = sys.getdefaultencoding()
	encoding2 = locale.getdefaultlocale()[1]

	val = val.replace(u"，", ",");
	try:
		ret = eval(u'''[''' + val + u''']''')
	except:
		ret = []
		kv = val.split(",")
		for el in kv:
			if not len(el):
				continue
			key = del_quot(el)
			ret.append(key)
	return ret

def is_null(val):
	if val == None or val == 0 or val == '' or val == [] or val == {}:
		return 1
	return 0

def parse_c2t(val):
	#cmd = "echo " + encode(val) + " | c2t "
	#content = os.popen(cmd).read()
	cur_dir = os.getcwd()
	c2t_dir = cur_dir + "/tools/c2t/"
	os.chdir(c2t_dir)
	sys.path.append(c2t_dir)
	C2T_MOD = __import__("ch2py")
	content = C2T_MOD.ch2pr(encode(val))
	content = content.strip()
	content = content.replace(" ","_")
	content = re.sub("[0-9]","", content)
	os.chdir(cur_dir)
	return content.upper()

def parse_c2t_macros(val):
	cur_dir = os.getcwd()
	c2t_dir = cur_dir + "/tools/c2t/"
	os.chdir(c2t_dir)
	sys.path.append(c2t_dir)
	C2T_MOD = __import__("ch2py")
	content = C2T_MOD.ch2pr(encode(val))
	content = content.strip()
	content = content.replace(" ","_")
	content = re.sub("[0-9]","", content)
	os.chdir(cur_dir)

	if len(content):
		return "@@SYS_MACROS_"+content.upper()
	return ""

def parse_function(statement, val):
	if not len(val) :
		return 0
	# return "@@function (mapping env) {return (%s);}" % (val)
        return "@@%s { %s; }" % (statement, val)

def do_parse_value(type, val):
	if type == "int":
		ret = parse_int(val)
		return ret
	if type == "float":
		return parse_float(val)
	elif type == "string" or type == "str":
		return parse_string(val)
	elif type == "bool":
		return parse_bool(val)
	elif type == "map":
		return parse_map(val)
	elif type == "array":
		return parse_array(val)
	elif type == "macros":
		if gMappingParse:
			try:
				tmp = macros[val]
				return tmp
			except:
				print u"macros error!", type, val
				if len(val) > 0:raise

		tmp = parse_string(val)
		if len(tmp):
			return "@@"+ tmp
		else:
			return 0
	elif type == "c2t":
		return [parse_string(val), parse_c2t(val)]
	elif type == "c2t_macros":
		return parse_c2t_macros(val)
	elif type == "mixed":
		return parse_mixed(val)
        elif type.startswith('function'):
                return parse_function(type, val)
	return ''

def do_parse_value_from_str(valueStr):
	value = valueStr.split("(")[0]
	t = valueStr.split("(")[1].rstrip(")")
	return do_parse_value(t, value)


def do_parse(type, val):
	ret = do_parse_value(type, val)
	return Util.PythonData2Lpc(ret)

TO_NULL = 0
TO_LPC = 1
TO_LUR = 2
TO_JSON = 3

def toLangStr(to_lang, parse_str):
	if to_lang == TO_LPC:
		return Util.PythonData2Lpc(parse_str)
	elif to_lang == TO_LUR:
		return Util.PythonData2Lua(parse_str)
	elif to_lang == TO_JSON:
		return Util.PythonData2Json(parse_str)
	else:
		return parse_str

def DoParseSheet(dictConfig, SheetDict, to_lang):

	vars_map = {}
	#print "Parse", SheetDict["name"], "..."

	#导出udata
	vars_map["udata"] = {}
	if "udata" in dictConfig.keys():
		for dkey in dictConfig["udata"].keys():
			col = dictConfig["udata"][dkey]["col"]
			row = dictConfig["udata"][dkey]["row"]
			parse_str = do_parse_value(dictConfig["udata"][dkey]['type'], SheetDict[row][col]["value"])
			vars_map["udata"][dkey] = toLangStr(to_lang, parse_str)


	#导出table
	vars_map["table"] = {}
	keyrow = dictConfig.get("KeyRow", 0)
	metarow = dictConfig.get("MetaRow", -1)
	parserow = dictConfig.get("ParseRow", 0)
	parsecol = 0
	if "ParseCol" in dictConfig.keys():
		parsecol = dictConfig.get("ParseCol", 0)
	primarycol = -1
	if "PrimaryKeyCol" in dictConfig.keys():
		primarycol = dictConfig.get("PrimaryKeyCol", 0)

	dictCols = {}
        meta = {}
	#取出所有的key
	for col in range(parsecol, SheetDict["ncols"], 1):
		key = SheetDict[keyrow][col]["value"]
		if not len(key):
			continue
		Re = re.compile("([\w|@]*)\((.*)\)")
		m = Re.findall(key)
		if not len(m):
			continue
		#print("**************", m, key, col, parsecol, keyrow)
		key = (m[0][0])
		type = (m[0][1])
		dictCols[col] = {"key": key, "type":type}

                if metarow >= 0:
                        meta[key] = SheetDict[metarow][col]["value"]

	#开始读表
	for row in range(parserow, SheetDict["nrows"], 1): 
		vars_col = {}
		vars_col["Cols"] = {}
		for col in range(parsecol, SheetDict["ncols"], 1):
			if col in dictCols.keys():
				colkey = dictCols[col]
				key = colkey["key"]
				type = colkey["type"]
				ret = do_parse_value(type, SheetDict[row][col]["value"])
				if not is_null(ret):
					if col == primarycol:
						ret = toLangStr(to_lang, ret)
						if ret:
							vars_col["PrimaryKey"] = ret

					else:
						#如果已经有这个key,那么这个key的值变成array
						if key in vars_col["Cols"].keys():
							value = vars_col["Cols"][key]
							if not isinstance(value, list):
								tmpval = vars_col["Cols"][key]
								vars_col["Cols"][key] = [tmpval]
							vars_col["Cols"][key].append(ret)
						else:
							vars_col["Cols"][key] = ret
		for key in vars_col["Cols"].keys():
			value = vars_col["Cols"][key]
			vars_col["Cols"][key] = toLangStr(to_lang, value)
		if primarycol == -1 or "PrimaryKey" in vars_col.keys():
			vars_map["table"][row - parserow] = vars_col

	return vars_map, meta
	#print "Parse", SheetDict["name"], "OK!"

def BeginEndParse(SheetDict):
	
	dictConfig = {}
	table = {}
	primarykey = ""

	#取出所有的key
	for row in range(0, SheetDict["nrows"], 1):
		if not len(SheetDict[row][0]):
			continue;
		value = SheetDict[row][0]["value"]
		if value == u"begin":
			break;
		if value.find(u"表项信息") >= 0:
			for col in range(1, SheetDict["ncols"], 1):
				dictkey = SheetDict[row][col]["value"]
				keyinfo = SheetDict[row+1][col]["value"]
				if not len(dictkey) or not len(keyinfo):
					continue
				Re = re.compile("([\w]*)\(([^)]*)\)")
				print [keyinfo]
				m = Re.findall(keyinfo)
				key = (m[0][0])
				type = (m[0][1])
				if type.find(",") > 0:
					type = type.split(",")
				dictConfig[dictkey] = {"key" : key, "type": type}
		if value == u"主键:":
			primarykey = SheetDict[row][1]["value"]
	
	if not len(primarykey):
		return

	#开始读表
	beginflag = 0
	primary = ""
	tmptable = {}
	for row in range(0, SheetDict["nrows"], 1): 
		value = SheetDict[row][0]["value"]
		if value == u"begin":
			beginflag = 1
			primary = ""
			tmptable = {}
			continue
		elif beginflag and value == u"end":
			beginflag = 0
			'''
			for key in tmptable.keys():
				if isinstance(tmptable[key], list):
					tmp = "({"
					for str in tmptable[key]:
						tmp += "\n\t\t\t%s,"%(str)
					tmp += "\n\t\t})"
					tmptable[key] = tmp
			'''
			table[primary] = tmptable
			continue
		if beginflag == 1:
			# 将value根据配置格式化为最终的输出
			if value in dictConfig.keys():
				type = dictConfig[value]["type"]
				key = dictConfig[value]["key"]
				if not isinstance(type, list):
					if value == primarykey:
						primary = do_parse_value(type,SheetDict[row][1]["value"])
					else:
						ret = do_parse_value(type,SheetDict[row][1]["value"])
						if is_null(ret):
							continue
						tmptable[key] = ret
				elif isinstance(type, list):
					tmparr = []
					hassubprimary = 0
					needsort = 0
					subprimary = ""
					for i in range(0, len(type), 1):
						tmptype = type[i]
						if tmptype.find(":") > 0:
							tmptypes = tmptype.split(":")
							if tmptypes[1] == "primary":
								hassubprimary = 1
								ret = do_parse_value(tmptypes[0], SheetDict[row][1+i]["value"])
								subprimary = ret
							if len(tmptypes) > 2:
								if tmptypes[2] == "sort":
									needsort = 1
						else:
							ret = do_parse_value(tmptype, SheetDict[row][1+i]["value"])
							tmparr.append(ret)

					if hassubprimary:
						if key not in tmptable.keys():
							tmptable[key] = {}
						if subprimary in tmptable[key].keys():
							print "primary key error, row: %d, %s"%(row, encode(subprimary))
							raise
						if needsort:
							if "_keys" not in tmptable[key].keys():
								tmptable[key]["_keys"] = []
							tmptable[key]["_keys"].append(subprimary)
						tmptable[key][subprimary] = tmparr
					else:
						if key not in tmptable.keys():
							tmptable[key] = []
						tmptable[key].append(tmparr)
	return table

def BeginEndParse2(SheetDict):
	global gMappingParse
	global macros
	gMappingParse = True	
	dictConfig = {}
	table = {}
	#取出所有的key
	for row in range(0, SheetDict["nrows"], 1):
		if not len(SheetDict[row][0]):
			continue;
		value = SheetDict[row][0]["value"]
		if value.find(u"定义表项") >= 0:
			key = SheetDict[row][1]["value"];
			type = SheetDict[row][2]["value"];
			if type.find(",") > 0:
				type = type.split(",");
			dictConfig[key] = {"key":"null", "type":type}
		elif value.find(u"定义宏") >= 0:
			name = SheetDict[row][1]["value"];
			value = SheetDict[row][2]["value"];
			value = do_parse_value_from_str(value);
			macros[name] = value

	#开始读表
	primary = ""
	for row in range(0, SheetDict["nrows"], 1): 
		value = SheetDict[row][0]["value"]
		if value == u"mapping":
			primary = SheetDict[row][1]["value"]
			primary = do_parse_value_from_str(primary)
			table[primary] = {}
			continue

		if value in dictConfig.keys():
			# 将value根据配置格式化为最终的输出
			type = dictConfig[value]["type"]
			key = dictConfig[value]["key"]
			if not isinstance(type, list):
				ret = do_parse_value(type,SheetDict[row][1]["value"])
				if is_null(ret): continue;
				table[primary][key] = ret
			elif isinstance(type, list):
				tmpDict = {}
				subprimary = ""
				subkey = "";
				for i in range(0, len(type), 1):
					tmptype = type[i]
					if tmptype.find(":") > 0:
						tmptypes = tmptype.split(":")
						if tmptypes[1] == "primary":
							ret = do_parse_value(tmptypes[0], SheetDict[row][1+i]["value"])
							subkey = ret;
						else:
							ret = do_parse_value(tmptypes[0], SheetDict[row][1+i]["value"])
							tmpDict[tmptypes[1]] = ret;
				table[primary][subkey] = tmpDict;
	return table

def VerticalParse(SheetDict, Config):
	
	dictConfig = Config
	table = {}
	primarykey = ""
	currow = 0

	#取出所有的key

	for row in range(0, SheetDict["nrows"], 1):
		if not len(SheetDict[row][0]):
			continue;
		value = SheetDict[row][0]["value"]
		if value == u"begin":
			break;
		if value == u"表项信息":
			begin = 0
			for parserow in range(row + 1, SheetDict["nrows"], 1):
				dictkey = SheetDict[parserow][0]["value"]
				if dictkey == u"End" or dictkey == u"end":
					currow = parserow
					break
				if dictkey == u"Begin" or dictkey == u"begin":
					begin = 1
					continue
				if not begin:
					continue
				keyinfo = SheetDict[parserow][1]["value"]
				if not len(dictkey) or not len(keyinfo):
					continue
				Re = re.compile("([\w]*)\(([^)]*)\)")
				m = Re.findall(keyinfo)
				key = m[0][0]
				type = m[0][1]
				types = []
				dictConfig[dictkey] = {"key" : key}
				if type.find(",") > 0:
					types = type.split(",")
					dictConfig[dictkey]["type"] = types
				else:
					dictConfig[dictkey]["type"] = type

	#开始读表
	beginflag = 0
	tmptable = {}
	for row in range(currow + 1, SheetDict["nrows"], 1): 
		value = SheetDict[row][0]["value"]
		if value in dictConfig.keys():
			types = dictConfig[value]["type"]
			key = dictConfig[value]["key"]
			if not isinstance(types, list):
				ret = do_parse_value(types, SheetDict[row][1]["value"])
				tmptable[key] = ret
			elif isinstance(types, list):
				# 将value根据配置格式化为最终的输出
				primary = ""
				if key not in tmptable.keys():
					tmptable[key] = {}

				has_primary = 0
				needsort = 0
				for i in range(0, len(types), 1):
					if types[i].find(":") > 0:
						tmptypes = types[i].split(":")
						if tmptypes[1] == u"primary":
							type = tmptypes[0]
							primary = do_parse_value(type, SheetDict[row][1+i]["value"])
							has_primary = 1
						if len(tmptypes) > 2:
							if tmptypes[2] == "sort":
								needsort = 1
						
				if has_primary and is_null(primary):
					continue
				vals = []
				for i in range(0, len(types), 1):
					if types[i].find(":") >= 0 and types[i].split(":")[1] == u"primary":
						continue
					currow = 1+i
					if currow not in SheetDict[row].keys():
						val = ""
					else:
						val = SheetDict[row][1+i]["value"]
					ret = do_parse_value(types[i], val)
					vals.append(ret)
				if len(vals) == 1:
					vals = vals[0]

				if not is_null(primary):
					if primary in tmptable[key].keys():
						print "primary key error, key exist!, row: %d"%(row), encode(primary)
						raise
					if needsort:
						if "_keys" not in tmptable[key].keys():
							tmptable[key]["_keys"] = []
						tmptable[key]["_keys"].append(primary)
					tmptable[key][primary] = vals
				else:
					tmptable[key] = vals
	return tmptable 

def XlsSheet2Dict(ParseFile, ParseSheet):
	return Util.XlsSheet2Dict(ParseFile, ParseSheet)


def DoWrite(Content, OutputFile, InputFile="undefined"):
	begin = r"// -------------------  Auto Generate Begin --------------------"
	end   = r"// -------------------  Auto Generate End   --------------------"
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
	Content = p.sub(begin + "\n" + Content + "\n" + end, src_data)

	f = file(OutputFile, "w")
	f.write(Content)
	f.close()

def DoParseNormalWrite(dictConfig, ParseFile, ParseSheet, OutputFile, SheetDict = {}):
	if not len(SheetDict):
		SheetDict = XlsSheet2Dict(ParseFile, ParseSheet)
	vars_map, _ = DoParseSheet(dictConfig, SheetDict, TO_LPC)
	Content = ""
	if "Template" in dictConfig.keys():
		Content = Util.RenderTemplateFile(dictConfig["Template"], vars_map)
	elif "TemplateSrc" in dictConfig.keys():
		Content = Util.RenderTemplateString(dictConfig["TemplateSrc"], vars_map)
	DoWrite(Content, OutputFile, ParseFile)

def DoParseNormal(dictConfig, ParseFile, ParseSheet):
	SheetDict = XlsSheet2Dict(ParseFile, ParseSheet)
	return DoParseSheet(dictConfig, SheetDict, TO_LPC)

def DoParseNormal2(dictConfig, ParseFile, ParseSheet):
	SheetDict = XlsSheet2Dict(ParseFile, ParseSheet)
	parsedata, _ = DoParseSheet(dictConfig, SheetDict, TO_NULL)
	res = {}
	for row in parsedata["table"].keys():
		data = parsedata["table"][row]
		if data["PrimaryKey"]:
			res[data["PrimaryKey"]] = data["Cols"]
	return res

def DoParseBeginEnd(ParseFile, ParseSheet):
	SheetDict = XlsSheet2Dict(ParseFile, ParseSheet)
	return BeginEndParse(SheetDict)

def DoParseBeginEnd2(ParseFile, ParseSheet):
	SheetDict = XlsSheet2Dict(ParseFile, ParseSheet)
	return BeginEndParse2(SheetDict);

def DoParseVertical(ParseFile, ParseSheet, Config):
	SheetDict = XlsSheet2Dict(ParseFile, ParseSheet)
	return VerticalParse(SheetDict, Config)

if __name__ == '__main__':
	#test c2t
	sys.path.append("/home/chenyh/work/trunk/logic")
	print parse_c2t(raw_input("please input chinese:"))
