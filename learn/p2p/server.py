import socket

if __name__ == '__main__':
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('121.10.246.15', 8001))
    while True:
        data, addr = sock.recvfrom(1024)
        sock.sendto("your address is "+str(addr), addr)
        sock.sendto(addr[0], addr)
        sock.sendto(str(addr[1]), addr)
        print addr, ":", data
        
