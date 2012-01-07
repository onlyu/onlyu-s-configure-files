import socket
import time
import sys

server_ip = '121.10.246.15'
server_port = 8001

if __name__ == '__main__':
    print sys.argv
    if len(sys.argv) == 3:
        ip = sys.argv[1]
        port = int(sys.argv[2])
        
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("", 0))
    sock.sendto("hello server", (server_ip, server_port))
    while True:
        sock.sendto("hello server", (ip, port))
        data, addr = sock.recvfrom(1024)
        print addr, data
        time.sleep(1)
