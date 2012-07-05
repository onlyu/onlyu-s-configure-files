import re

import simplejson

# INT = 0
# STRING = 1
# CLASS = 2

ARRAY_VAR = 0x00010000

RPC_FILE = "rc/rpc/rpc.cfg"
RPC_ID_FILE = "rc/rpc/rpc_id.h"
JSON_FILE = "rc/rpc/rpcJson.cfg"

# return class_table_num
def read_class_table_num(f):
	line = f.readline()
	match = re.match('class_table_num:(\d+)', line)
	return int(match.group(1))

# return (field_count, c_imp, class_name)
def read_class_info(f):
	line = f.readline()
	match = re.match('field_count:(\d+),c_imp:(\d+),class_name=(\w+)', line)
	return (int(match.group(1)), int(match.group(2)), match.group(3))

# return (field_type, class_index, array, field_name)
def read_class_field_type(f):
	line = f.readline()
	match = re.match('field_type:(\d+),class_index:(-?\d+),array:(-?\d+),field_name:(\w+)', line)
	return (int(match.group(1)), int(match.group(2)), int(match.group(3)), match.group(4))

# return function_table_num
def read_function_table_num(f):
	line = f.readline()
	match = re.match('function_table_num:(\d+)', line)
	return int(match.group(1))

# return (function_id, c_imp, arg_count, function_name)
def read_function_info(f):
	line = f.readline()
	match = re.match('function_id:(\d+),c_imp:(\d+),arg_count:(\d+),module:([\w|/]+),function_name:(\w+)', line)
	return (match.group(1), int(match.group(2)), int(match.group(3)), match.group(5))

# return (arg_type, class_index, array)
def read_function_arg_type(f):
	line = f.readline()
	match = re.match('arg_type:(\d+),class_index:(-?\d+),array:(-?\d+)', line)
	return (int(match.group(1)), int(match.group(2)), int(match.group(3)))

if __name__ == '__main__':
	f = open(RPC_FILE, "r")

	classJson = []
	classTableNum = read_class_table_num(f)
	while classTableNum > 0:
		field_count, c_imp, class_name = read_class_info(f)
		_tmpObj = {"class_name": class_name, "field": []}

		while field_count > 0:
			field_type, class_index, array, field_name = read_class_field_type(f)
			if array != -2: field_type = field_type | ARRAY_VAR

			_tmpObj["field"].append({"type": field_type, "class_index": class_index, "field_name": field_name})
			field_count = field_count - 1
		
		classJson.append(_tmpObj)
		classTableNum = classTableNum - 1


	functionJson = {}
	functionTableNum = read_function_table_num(f)
	while functionTableNum > 0:
		function_id, c_imp, arg_count, function_name = read_function_info(f)
		_tmpObj = {"function_name": function_name, "args": []}

		# remove the first arg
		read_function_arg_type(f)
		arg_count = arg_count - 1

		while arg_count > 0:
			arg_type, class_index, array = read_function_arg_type(f)
			if array != -2: arg_type = arg_type | ARRAY_VAR

			_tmpObj["args"].append({"type": arg_type, "class_index": class_index})
			arg_count = arg_count - 1

		functionJson[function_id] = _tmpObj
		functionTableNum = functionTableNum - 1

	f.close()


	f = open(RPC_ID_FILE, "r")
	lines = f.readlines()
	f.close()

	"""
	rpcMacroJson = {}
	for line in lines:
		if line.startswith("#define RPC"):
			match = re.match('#define (\w+) (\d+)', line)
			rpcMacroJson[match.group(1)] = match.group(2)
	jsonStr = simplejson.dumps({"class_cfg": classJson, "function_cfg": functionJson, "rpc_macro": rpcMacroJson})
	"""

	jsonStr = simplejson.dumps({"class_cfg": classJson, "function_cfg": functionJson})

	f = open(JSON_FILE, "w")
	f.write(jsonStr)
	f.close()

	print 'rpc2json finish:', JSON_FILE

	
