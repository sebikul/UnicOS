#define SYS_CALL_RTC	2
#define SYS_CALL_READ	3
#define SYS_CALL_WRITE	4

#define FD_READ 		0
#define FD_WRITE 		1

#define RTC_SEC_ALARM 		101
#define RTC_MINUTES 		102
#define RTC_MINUTES_ALARM 	103
#define RTC_HOURS 			104
#define RTC_HOURS_ALARM 	105
#define RTC_DAY_WEEK 		106
#define RTC_DAY_MONTH 		107
#define RTC_MONTH 			108
#define RTC_YEAR 			109

void sys_write(int fd, char* s, int len);

void sys_read(int fd, char* s, int len);

void sys_rtc_time(int* hours, int* minutes, int* seconds);

