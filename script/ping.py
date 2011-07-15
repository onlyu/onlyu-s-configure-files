import sys
import getopt
import os
import re

def statics(records, index):
	result = {}
	for record in records:
		try:
			result[record[index]] = result[record[index]] + 1 	
		except:
			result[record[index]] = 1
	return result

def process(file_name):
	#os.system("cat " + file_name)	
	regx = re.compile(r'.*uid=(.*),time.*,fps=(.*),last_delay=(.*),average_delay=(.*)')
	#regx = re.compile(r'.* ([0-9]*):([0-9]*)/([0-9]*)/([0-9]*)')
	with open(file_name) as f:
		records = []
		for line in f:
			m = re.match(regx,line)
			uid,fps,last_delay,average_delay = m.group(1,2,3,4)
			#print uid, fps, last_delay, average_delay
			uid = int(uid)
			fps = int(fps)
			last_delay = int(last_delay)
			average_delay = int(average_delay)
			records.append((uid, fps, last_delay, average_delay))
	#print records
	return records


def usage():
	print '''usage:
(-h, --help)
	show the help message
(-f file, --file file) 
	set the file to process
'''


def main(argv):
	try:
		opts,args = getopt.getopt(argv, "hf:", ["help", "file="])
	except getopt.GetoptError:
		usage()
		sys.exit(-1)

	for opt,arg in opts:
		if opt in ("-h", "--help"):
			usage()
			sys.exit()
		elif opt in ("-f", "--file"):
			file_name = arg
		else:
			print 'not support argument option:',opt
			usage()
			sys.exit()

	if not file_name 
		usage()
		sys.exit()
	
	records = process(file_name)
	
	statics_fps = statics(records, 1)
	statics_last_delay = statics(records, 2)
	statics_average_delay = statics(records, 3)
	
	fps_data = re.compile(r'.*%fps_data%.*')
	last_delay_data = re.compile(r'.*%last_delay_data%.*')
	average_delay_data = re.compile(r'.*%average_delay_data%.*')

	out_file = open("ping.html", "w")
	with open("ping.template") as f:
		for line in f:
			if re.match(fps_data, line):
				for key in statics_fps.iterkeys():
					out_file.write( '["'+str(key)+'"'+','+ str(statics_fps[key]) + '],')
			elif re.match(last_delay_data, line):
				for key in statics_last_delay.iterkeys():
					out_file.write( '["'+str(key)+'"'+','+ str(statics_last_delay[key]) + '],')
			elif re.match(average_delay_data, line):
				for key in statics_average_delay.iterkeys():
					out_file.write( '["'+str(key)+'"'+','+ str(statics_average_delay[key]) + '],')
			else:
				out_file.write( line )
	out_file.close()
	print "ping.html generate success"

if __name__ == '__main__':
	main(sys.argv[1:])
