
import os
import re
import urllib2

serverinfo = {
    }

def get_server_info():
    f = urllib2.urlopen( "http://mcs.175game.com/%s/serverinfo"%("fs") )
    lines = map(lambda(x): x.lstrip("*").split(","), f.readlines())
    for line in lines:
        try:
            serverinfo[int(line[0])] = line
        except:
            pass

def get_server_name(id):
    return serverinfo[id][1]
    
if __name__ == '__main__':
    msg = os.popen("ls sshfs")
    lines = msg.readlines()
    lines = map(lambda(x): x.strip('\n'), lines)
    lines = filter(lambda(x): re.match('^[0-9]+$', x), lines)
    onlines = map(lambda(x): os.popen("cat sshfs/"+x+"/fs/logic/log/online").readline(), lines)
    onlines = map(lambda(x): x.strip('\n').split(']')[2], onlines)
    onlines = map(lambda(x): int(x), onlines)
    get_server_info()
    for i in xrange(len(lines)):
        print get_server_name(int(lines[i])), onlines[i]
    print sum(onlines)


