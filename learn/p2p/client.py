import socket
import time
import sys
    
if __name__ == '__main__':
    print sys.argv
    if len(sys.argv) == 3:
        ip = sys.argv[1]
        port = int(sys.argv[2])
    else:
        ip = '121.10.246.15'
        port = 8001
        
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("", 0))
    sock.sendto("hello server", (ip, port))
    while True:
        data, addr = sock.recvfrom(1024)
        print addr, ":", data
        if addr[1] == 8001:
            ip,ad = sock.recvfrom(1024)
            port,ad = sock.recvfrom(1024)
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.bind(("", 0))
            s.sendto("hello hero", (ip, int(port)))
        else:
            sock.sendto("hello " + str(addr), addr)
            time.sleep(1)
        

    
        
