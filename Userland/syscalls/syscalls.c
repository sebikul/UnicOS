#include <syscalls.h>

extern int syscall(int, FD, char*, int);
//extern void syscall(int* hours, int* minutes, int* seconds);

void sys_write(FD fd, char* s, int len) {

	syscall(SYSCALL_WRITE, fd, s, len);

}

int sys_read(FD fd, char* s, int len) {

	return syscall(SYSCALL_READ, fd, s, len);

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
void sys_rtc_time(int* hours, int* minutes, int* seconds) {

	//syscall
	//syscall(SYS_CALL_RTC,hours,minutes,seconds);
	//hay que ver como llamar al rtc y eso.... es medio raro http://stanislavs.org/helppc/cmos_ram.html
	//hours=syscall(SYS_CALL_RTC, RTC_HOURS);
	//minutes=syscall(SYS_CALL_RTC, RTC_MINUTES);
	//seconds=syscall(SYS_CALL_RTC, RTC_SECONDS);
}