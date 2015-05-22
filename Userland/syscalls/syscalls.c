

#include "include/syscalls.h"

extern void syscall(int,int,char*,int);

void sys_write(int fd, char* s, int len){

	syscall(SYS_CALL_WRITE,fd,s,len);

}

void sys_read(int fd, char* s, int len){
	
	syscall(SYS_CALL_READ,fd,s,len);

}
/*
	 00  RTC seconds
	 01  RTC seconds alarm
	 02  RTC minutes
	 03  RTC minutes alarm
	 04  RTC hours
	 05  RTC hours alarm
	 06  RTC day of week
	 07  RTC day of month
	 08  RTC month
	 09  RTC year
*/
void sys_rtc_time(int* hours, int* minutes, int* seconds){

	//hay que ver como llamar al rtc y eso.... es medio raro http://stanislavs.org/helppc/cmos_ram.html
	//hours=syscall(SYS_CALL_RTC, RTC_HOURS); 
	//minutes=syscall(SYS_CALL_RTC, RTC_MINUTES);
	//seconds=syscall(SYS_CALL_RTC, RTC_SECONDS);
}