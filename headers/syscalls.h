
#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define SYS_CALL_RTC	2
#define SYSCALL_READ	3
#define SYSCALL_WRITE	4

enum FD
{
	FD_STDERR,
	FD_STDOUT
};

typedef struct {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t day;
	uint8_t month;
	uint32_t year;//ultimos dos digitos
} time_t;

typedef enum FD FD;

void sys_write(FD fd, char* s, int len);

int sys_read(FD fd, char* s, int len);

void sys_time(time_t* t) ;


#endif