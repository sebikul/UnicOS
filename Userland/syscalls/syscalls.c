

#include "syscalls.h"

extern syscall()

void sys_write(int fd, const char* s, int len){

	syscall(4,fd,s,len);

}

void sys_read(int fd, char* s, int len){
	
	syscall(3,fd,s,len);

}