#include "../grammar/spec.h"

// ����socket
//int socket_create(string | function close_callback);
int socket_create(string | function );
// socket�󶨶˿�
// ������fd����accept�ص�����
// int socket_bind(int fd, int port, string | function listen_callback);
int socket_bind_and_listen(int, int, string | function );
// accept����,���ûص���read_callback,write_callback
// int socket_accept(int fd, string | function readcallback, string | function writecallback);
int socket_accept(int, string | function, string | function);
// ����Ŀ���ַ:�˿ڣ�ͬʱ���ûص�
// int socket_connect(int fd, string ip, string | function read_callback, string | function write_callback);
int socket_connect(int, string, string | function, string | function);
// int socket_write(int fd, mixed data);
int socket_write(int, mixed);
// int socket_close( int fd );
int socket_close(int);
// ����fd�Է���ip��ַ
// int socket_address(int fd);
string socket_address(int );
// dump lpc socket��״̬
string dump_socket_status();
