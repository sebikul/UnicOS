

#include "include/syscalls.h"

extern void syscall(int,int,char*,int);

void sys_write(int fd, char* s, int len){

	syscall(4,fd,s,len);

}

void sys_read(int fd, char* s, int len){
	
	syscall(3,fd,s,len);

}