

#include <rtc.h>
#include <io.h>
#include <syscalls.h>
#include <video.h>

#define BCD_TO_INT(bcd) (((bcd / 16) * 10) + (bcd & 0xF))

static uint8_t rtc_read(uint8_t register);
static uint8_t rtc_update_in_progress();

void rtc_get_time(time_t* t) {

	uint8_t regb;

	while (rtc_update_in_progress());

	t->second = rtc_read(RTC_CURRENT_SECOND);
	t->minute = rtc_read(RTC_CURRENT_MINUTE);
	t->hour = rtc_read(RTC_CURRENT_HOUR);
	t->day = rtc_read(RTC_CURRENT_DAY);
	t->month = rtc_read(RTC_CURRENT_MONTH);
	t->year = rtc_read(RTC_CURRENT_YEAR);

	regb = inb(0xB);

	if ((regb & 0x4) == 0) {
		//los valores estan en formato BCD

		t->second = BCD_TO_INT(t->second);
		t->minute = BCD_TO_INT(t->minute);
		t->hour = BCD_TO_INT(t->hour);
		t->day = BCD_TO_INT(t->day);
		t->month = BCD_TO_INT(t->month);
		t->year = BCD_TO_INT(t->year);

	}

	t->year = 2000 + t->year;

	if ((regb & 0x02) == 0 && (t->hour & 0x80)) {
		//el reloj esta en formato 12hs y el primer bit que indica pm esta encendido. 
		//Hay que apagar el primer bit, sumarle 12, y compensar que medianoche son las 12pm
		t->hour = ((t->hour & 0x7F) + 12) % 24;
	}
}

static uint8_t rtc_update_in_progress() {

	return (rtc_read(0xA) & 0x8);

}

static uint8_t rtc_read(uint8_t reg) {

	outb(0x70, reg);

	return inb(0x71);

}



