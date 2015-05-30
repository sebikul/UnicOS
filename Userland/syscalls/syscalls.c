#include <syscalls.h>
#include <stdint.h>

//extern int syscall(int, FD, char*, int);

extern int syscall(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4);
//extern void syscall(int* hours, int* minutes, int* seconds);

void sys_write(FD fd, char* s, int len) {

	syscall((uint64_t)SYSCALL_WRITE, (uint64_t)fd, (uint64_t)s, (uint64_t)len);

}

int sys_read(FD fd, char* s, int len) {

	return syscall((uint64_t)SYSCALL_READ, (uint64_t)fd, (uint64_t)s, (uint64_t)len);

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

	syscall((uint64_t)SYS_CALL_RTC, (uint64_t)hours, (uint64_t)minutes, (uint64_t)seconds);

	//syscall
	//syscall(SYS_CALL_RTC,hours,minutes,seconds);
	//hay que ver como llamar al rtc y eso.... es medio raro http://stanislavs.org/helppc/cmos_ram.html
	//hours=syscall(SYS_CALL_RTC, RTC_HOURS);
	//minutes=syscall(SYS_CALL_RTC, RTC_MINUTES);
	//seconds=syscall(SYS_CALL_RTC, RTC_SECONDS);
}