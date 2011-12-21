import socket
import time
import sys
    
if __name__ == '__main__':
    print sys.argv
    if len(sys.argv) == 3:
        ip = sys.argv[1]
        port = int(sys.argv[2])
        
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("", 0))
    sock.sendto("hello server", (ip, port))
    while True:
        sock.sendto("hello server", (ip, port))
        time.sleep(1)
