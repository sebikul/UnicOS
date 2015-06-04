

#include <rtc.h>
#include <io.h>
#include <types.h>
#include <video.h>

#define BCD_TO_INT(bcd) ((((bcd) / 16) * 10) + ((bcd) & 0xF))

#define INT_TO_BCD(in) 	((((in) / 10) * 16) | (in % 10))

#define USES_BCD(regb)	((regb & 0x4) == 0)
#define CLOCK_IN_12HOURS(regb) ((regb & 0x02) == 0)
#define TO_PM(h)	(0x80 | (h))
#define IS_PM(h)	(0x80 & (h))

static uint8_t rtc_read(uint8_t reg);
static void rtc_write(uint8_t reg, uint8_t val);

static uint8_t rtc_update_in_progress();

void rtc_get_time(time_t* t) {

	uint8_t regb;

	regb = inb(0xB);

	while (rtc_update_in_progress());

	t->second = rtc_read(RTC_CURRENT_SECOND);
	t->minute = rtc_read(RTC_CURRENT_MINUTE);
	t->hour = rtc_read(RTC_CURRENT_HOUR);
	t->day = rtc_read(RTC_CURRENT_DAY);
	t->month = rtc_read(RTC_CURRENT_MONTH);
	t->year = rtc_read(RTC_CURRENT_YEAR);

	if (USES_BCD(regb)) {
		//los valores estan en formato BCD

		t->second = BCD_TO_INT(t->second);
		t->minute = BCD_TO_INT(t->minute);
		t->hour = BCD_TO_INT(t->hour);
		t->day = BCD_TO_INT(t->day);
		t->month = BCD_TO_INT(t->month);
		t->year = BCD_TO_INT(t->year);

	}

	t->year = 2000 + t->year;

	if (CLOCK_IN_12HOURS(regb) && IS_PM(t->hour)) {
		//el reloj esta en formato 12hs y el primer bit que indica pm esta encendido.
		//Hay que apagar el primer bit, sumarle 12, y compensar que medianoche son las 12pm
		t->hour = ((t->hour & 0x7F) + 12) % 24;
	}
}

void rtc_set_time(time_t* t) {

	uint8_t regb;

	regb = inb(0xB);

	while (rtc_update_in_progress());

	t->year = t->year % 100;

	//TODO: check: Agregue el '!' porque me mostraba mal la hora nose si estaria bien
	if (!CLOCK_IN_12HOURS(regb) && t->hour > 12) {

		t->hour = TO_PM(t->hour - 12);

	}

	if (USES_BCD(regb)) {

		t->second = INT_TO_BCD(t->second);
		t->minute = INT_TO_BCD(t->minute);
		t->hour = INT_TO_BCD(t->hour);
		t->day = INT_TO_BCD(t->day);
		t->month = INT_TO_BCD(t->month);
		t->year = INT_TO_BCD(t->year);

	}



	rtc_write(RTC_CURRENT_SECOND, t->second);
	rtc_write(RTC_CURRENT_MINUTE, t->minute);
	rtc_write(RTC_CURRENT_HOUR, t->hour);
	rtc_write(RTC_CURRENT_DAY, t->day);
	rtc_write(RTC_CURRENT_MONTH, t->month);
	rtc_write(RTC_CURRENT_YEAR, t->year);

}

static uint8_t rtc_update_in_progress() {

	return (rtc_read(0xA) & 0x8);

}

static uint8_t rtc_read(uint8_t reg) {

	outb(RTC_PORT_POS, reg);

	return inb(RTC_PORT_VALUE);

}

static void rtc_write(uint8_t reg, uint8_t val) {

	outb(RTC_PORT_POS, reg);
	outb(RTC_PORT_VALUE, val);

}
