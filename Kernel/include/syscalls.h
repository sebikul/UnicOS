#define FD_STDERR 		0
#define FD_STDOUT		1

void sys_write(int fd, char* s, int len);

void sys_read(int fd, char* s, int len);