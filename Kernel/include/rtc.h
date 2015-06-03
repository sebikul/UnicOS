

#ifndef RTC_H
#define RTC_H

#include <types.h>

#define RTC_CURRENT_SECOND		0x0
#define RTC_CURRENT_MINUTE 		0x2
#define RTC_CURRENT_HOUR 		0x4
#define RTC_CURRENT_DAY			0x7
#define RTC_CURRENT_MONTH 		0x8
#define RTC_CURRENT_YEAR 		0x9

void rtc_get_time(time_t* t);
//void rtc_init();

#endif