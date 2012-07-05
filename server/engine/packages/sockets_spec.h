#include "../grammar/spec.h"

// 创建socket
//int socket_create(string | function close_callback);
int socket_create(string | function );
// socket绑定端口
// 并监听fd设置accept回调函数
// int socket_bind(int fd, int port, string | function listen_callback);
int socket_bind_and_listen(int, int, string | function );
// accept函数,设置回调的read_callback,write_callback
// int socket_accept(int fd, string | function readcallback, string | function writecallback);
int socket_accept(int, string | function, string | function);
// 连接目标地址:端口，同时设置回调
// int socket_connect(int fd, string ip, string | function read_callback, string | function write_callback);
int socket_connect(int, string, string | function, string | function);
// int socket_write(int fd, mixed data);
int socket_write(int, mixed);
// int socket_close( int fd );
int socket_close(int);
// 返回fd对方的ip地址
// int socket_address(int fd);
string socket_address(int );
// dump lpc socket的状态
string dump_socket_status();
