import socket

STR = """
<cross-domain-policy>
<site-control permitted-cross-domain-policies="*"/>
<allow-access-from domain="*" to-ports="*"/>
</cross-domain-policy>
"""

if __name__=="__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('', 2843))
    sock.listen(1)

    print 'waiting client'
    while 1:
        conn, addr = sock.accept()
        print 'client connected', addr
        data = conn.recv(1024)
        print("recv:%s,len:%d"%(data, len(data)))
        conn.send(STR)
	conn.send("\0")
        print 'send xml'
	conn.close()


    sock.close()
