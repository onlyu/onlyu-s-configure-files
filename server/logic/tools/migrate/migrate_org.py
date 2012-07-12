
#!/usr/local/bin/python
#-*- coding: utf-8 -*

import os, time, shutil

def ChangeOrgId(uidfile, pairs):
	#backup
	#shutil.copy2('e:/disk/vm.xls', 'new_name_of_copy')
	f1 = open(uidfile, "r")

	content = f1.read()

	if not '"mOrgInfo":{},' in content:
		for from_id, to_id in pairs.items():
			SrcStr = '"mOrgInfo":{"Id":%d,'%(from_id)
			
			if not SrcStr in content:
				continue

			DstStr = '"mOrgInfo":{"Id":%d,'%(to_id)

			f1.close()
			f1 = open(uidfile, "w")
			f1.write(content.replace(SrcStr, DstStr))
			print uidfile
			break

	f1.close()

def MigrateOrg():
	#migrate org,%d,%d,%d.
	#srcDir = "/home/canoe/test_mirgrate/"
	srcDir = "./migrate_src/"
	#logfile = "/home/canoe/src/server/trunk/logic/log/merge_server.log"
	logfile = "./log/migrate.log"

	Orgs = {}

	for line in file(logfile).readlines():
		if not "migrate org" in line:
			continue

		_, hostnum, from_id, to_id = line.split(",")

		hostnum = int(hostnum)
		from_id = int(from_id)
		to_id = int(to_id.split(".")[0])

		if not Orgs.has_key(hostnum):
			Orgs[hostnum] = {}

		Orgs[hostnum][from_id] = to_id	

	for hostnum, pairs in Orgs.items():
		#rename orgobj
		for from_id, to_id in pairs.items():
			# > 1000
			if from_id > 10000 :
				continue

			srcfile = "%s/%d/dat/org/%02d/%d.py"%(srcDir, hostnum, from_id%100, from_id)
			dstfile = "%s/%d/dat/org/%02d/%d.py"%(srcDir, hostnum, from_id%100, to_id)
			'''
			datdir = "%s/%d/dat/org/%02d/"%(srcDir, hostnum, from_id)
			dstfile = "%s/%d.py"%(datdir, to_id)
			try :
				os.mkdir(datdir)
			except :
				pass
			'''	

			shutil.move(srcfile, dstfile)

		userDir = "%s/%d/user"%(srcDir, hostnum)
		for root, dirs, files in os.walk(userDir):
			for name in files:
				ChangeOrgId(os.path.join(root, name), pairs)

if __name__ == '__main__':
	print "useless file."
	print time.time()
	#MigrateOrg()
	print time.time()
