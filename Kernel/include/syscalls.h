#define FD_READ 		0
#define FD_WRITE 		1

void sys_write(int fd, char* s, int len);

void sys_read(int fd, char* s, int len);