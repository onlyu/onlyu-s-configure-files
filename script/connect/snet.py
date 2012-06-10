#!/usr/bin/env python 
#-*- coding:gbk -*-
import urllib2
import subprocess
import ConfigParser
import sys
import ch2py
import os
import inspect

#���ڵõ����ݵ��б�
server_list = []

#�������Ժ��server list�ֵ�
server_set = {}

#��http���ӣ��õ�����
def open_http(product):
	global server_list
	print '��ʼȡ��serverinfo����ƷΪ', product
	f = urllib2.urlopen( "http://mcs.175game.com/%s/serverinfo"%(product) )
	print '�õ��б���ϣ���ʼ������ʽ...'
	server_list = f.readlines()
	f.close()
	if len(server_list) == 0:
		print "û��ȡ�����ݣ�������"
		sys.exit(1)

def head_py(cname):
	py = ch2py.ch2pr(cname)
	py_set = py.split(" ")
	if ( len(py_set) < 2):
		print "�õ���������д���ִ��� %s" %(cname)
		return ""
	
	short_name = py_set[0][0] + py_set[1][0]
	return short_name
	
def parse_server_list(product):
	global server_list
	global server_set
	
	if product == "fs":
		for line in server_list:
			if not line.startswith("*"):
				continue
			
			#*1001,�������(ԤԼ),121.10.246.12:119.38.128.204:121.10.246.12,10013,10015,2,��Ϸ,10011,10012,10010	
			hostid,cname,ips,port1,port2,load_avg,tag,port3,port4,port5 = line.split(",")
			hostid = hostid.split("*")[1]
			
			ip = ips.split(":")[0]
			
			py_name = head_py(cname)
			server_set[hostid] = [ip, py_name, cname, tag]
	else:
		print "������б�!"
		sys.exit(1)

	print "=============================="
	print "����server %d ��" % len(server_set)
	print "=============================="
	#print server_set

def is_number(number):
	try:
		int(number)
		return True
	except:
		return False

def current_path():
	current_file = inspect.getfile(inspect.currentframe())
	return os.path.dirname(current_file)

def ssh_openssh(config, ip):
	username = config.get('global', 'username')
	cmd = "ssh -F ~/.ssh/config " + ip + " -l " + username
	os.system(cmd)

def ssh_securecrt(config, ip):
	securecrt_path = config.get('global', 'securecrt_path')
	username = config.get('global', 'username')
	port = config.get('global', 'port')
	auth = config.get('global', 'auth')
	newtab = config.get('global', 'newtab')
	crt_ver = config.get('global', 'crt_ver')

	server_name = ip 
	if crt_ver =="6":
		CMD_LINE = '"%s" /N %s /L "%s" /P %s  /AUTH "%s" ' % (securecrt_path, server_name, username, port, auth)
	else:
		CMD_LINE = '"%s" /L "%s" /P %s  /AUTH "%s" ' % (securecrt_path, username, port, auth)

	if newtab == "1":
		CMD_LINE += " /T "
	subprocess.Popen(CMD_LINE + ip)

def ssh(config, ip):
	client = config.get('global', 'client')
	if client == "openssh":
		return ssh_openssh(config, ip)
	elif client == "securecrt":
		return ssh_securecrt(config, ip)
	else:
		print "client not configure: must be 'openssh' or 'securecrt'"
		return -1

def process():
	global server_set
	#��ȡ�����ļ�
	config = ConfigParser.ConfigParser()
	config.read(current_path() + '/snet.ini')
	product = config.get('global', 'product')
	
	#�õ��б�
	open_http("fs")
	
	#�����б�
	parse_server_list(product)

	#����ѭ��
	while 1:
		user_input = raw_input("Ϊ�л�֮���������\r\n������Ŀ���������ǰ����ƴ����ĸ��hostid,all,q�˳���").strip()
		if user_input == "q":
			return
		elif user_input == "all" or len(user_input) == 2 :
			for _hostid, _tuples in server_set.items():
				if user_input == "all" or user_input in _tuples[1] :
					print _hostid, _tuples[0], _tuples[1], _tuples[2], _tuples[3]
			
			continue

		elif is_number(user_input):
			if not server_set.has_key(user_input):
				print "û�����hostid"
				continue
			ip = server_set[user_input][0]
			ssh(config, ip)
		elif user_input in ("game", "sync", "game_wan"):
			ip_table = {
						"game":"192.168.0.8",
						"sync":"192.168.0.9",
						"game_wan":"183.62.45.76",
						}
			ip = ip_table[user_input]
			ssh(config, ip)
		else :
			print "�������"

if __name__ == "__main__":
	process()
