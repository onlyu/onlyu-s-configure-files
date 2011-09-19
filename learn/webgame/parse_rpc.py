import sys
import getopt
import re

def usage():
	print '''
	parse_rpc.py -f input_file -o output_file
	parse_rpc.py -h 
'''
def parse_class(f):
	line = f.readline()
	m = re.search("field_count:(\d+),c_imp:(\d+),class_name=(.+)", line)
	if not m:
		assert False, "parse class def failed" 
	try:
		field_count = int(m.group(1))
		c_imp = int(m.group(2))
		class_name = m.group(3)
	except ValueError:
		assert False, "parse class def failed"

	class_def = {
		'c_imp':c_imp,
		'name':class_name,
	}
	fields = []
	for i in xrange(field_count):
		# field_type:0,class_index:-1,array:-2,field_name:id	
		line = f.readline()
		m = re.search("field_type:(\d+),class_index:([-\d]+),array:([-\d]+),field_name:(.+)", line)	
		if not m:
			assert False, "parse class field failed " + class_name + " " + str(i)
		try:
			ftype = int(m.group(1))
			cindex = int(m.group(2))
			arr = int(m.group(3))
			field_name = m.group(4)
		except ValueError:
			assert False, "parse class field failed"
		
		type_conf = {
			0:'int',
			1:'str',
			2:'class',
			5:'buffer',
		}	
		assert ftype in type_conf.keys(), "unknow type: " + class_name + " field_name:" + field_name
		ftype = type_conf[ftype]
		field = {
			'name':field_name,
			'type':ftype,
			'is_array':arr == -1,
			'class_index':cindex,	
		}
		fields.append(field)
	class_def['fields'] = fields
	return class_def

def parse_func(f):
	func_def = {}
	line = f.readline()
	m = re.search("function_id:(\d+),c_imp:(\d+),arg_count:(\d+),module:(.+),function_name:(.+)", line)
	assert m, "parse function def failed"
	try:
		index = int(m.group(1))
		c_imp = int(m.group(2))
		arg_count = int(m.group(3))
		module = m.group(4)
		name = m.group(5)
	except ValueError:
		assert False, "parse function def failed"

	func_def['name'] = name
	func_def['module'] = module
	func_def['c_imp'] = c_imp
	func_def['args'] = []
	for i in xrange(arg_count):
		line = f.readline()
		m = re.search("arg_type:(\d+),class_index:([-\d]+),array:([-\d]+)", line)
		assert m, "parse function argument failed " + name + " " + str(i)
		try:
			atype = int(m.group(1))
			class_index = int(m.group(2))
			array = int(m.group(3))
		except ValueError:
			assert False, "parse function argument failed " + name + " " + str(i)

		type_conf = {
			0:'int',
			1:'str',
			2:'class',
			3:'unkown',
			4:'unkonw',
			5:'buffer',
		}	
		assert atype in type_conf.keys(), "unknow type: " + name + " argument:" + str(i) + " type:" + str(atype)
		atype = type_conf[atype]
	
		func_def['args'].append({
			'type':atype,
			'class_index':class_index,
			'is_array':array == -1,
		})
	
	return func_def

def parse_rpc(input, output):
	f = open(input, "rb")
	o = open(output, "wb")

	classes = [ ] 
	# parse classes
	line = f.readline()
	m = re.search("class_table_num:(\d+)", line)
	assert m, "class table number parse failed"
	try:
		class_num = int(m.group(1))
	except ValueError:
		assert False, "error reading class table number"	

	print "number of classes", class_num 
	for i in xrange(class_num):
		class_def = parse_class(f)	
		classes.append(classes)
	print "parse class success"

	print "parsing function"
	line = f.readline()
	m = re.search("function_table_num:(\d+)", line)
	assert m, "function table number parse failed"
	try:
		function_num = int(m.group(1))
	except ValueError:
		assert False, "error reading function table number"	
	
	functions = []
	print "number of functions", function_num
	for i in xrange(function_num): 
		func_def = parse_func(f)
		functions.append(func_def)
	print "parse function success"

	o.close()
	f.close()	

if __name__ == '__main__':
	try:
		opts,args = getopt.getopt(sys.argv[1:], "f:o:h")
	except getopt.GetoptError:
		usage()	
		sys.exit(2)

	for opt,arg in opts:
		if opt in ("-h", "--help"):
			usage()
			sys.exit(2)
		elif opt in ("-f"):
			input_file = arg	
		elif opt in ("-o"):
			output_file = arg
		else:
			assert False, "unhandled option"

	parse_rpc(input_file, output_file)	
