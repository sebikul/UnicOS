
#include <syscalls.h>
#include <types.h>
#include <video.h>
#include <keyboard.h>
#include <rtc.h>
#include <lib.h>

void sys_rtc_get(time_t* t) {
	rtc_get_time(t);
}

void sys_rtc_set(time_t* t) {
	rtc_set_time(t);
}

void sys_write(FD fd, char* s, int len) {

	color_t colorbk;

	switch (fd) {
	case FD_STDOUT:
		video_write_string(s);
		break;

	case FD_STDERR:

		colorbk = video_get_color();

		video_set_color(COLOR_RED, COLOR_BLACK);

		video_write_string(s);

		video_set_full_color(colorbk);
		break;


	}

}

int sys_read(FD fd, char* s, int len) {

	int read = 0;

	int i = 0;

	read = keyboard_wait_for_buffer(len);

	while (i < read) {
		s[i] = keyboard_get_char_from_buffer();

		i++;

	}

	s[i] = 0;

	return read;

}

void* sys_malloc(int len) {

	return malloc(len);

}

void* sys_calloc(int len) {

	return calloc(len);

}

void sys_free(void* m) {
	free(m);
}

void sys_keyboard_catch(uint64_t scancode, dka_handler handler) {
	keyboard_catch(scancode, handler);
}

void sys_clear_indexed_line(int index) {
	video_clear_indexed_line(index);
}

void sys_keyboard_replace_buffer(char* s) {
	keyboard_replace_last_written(s);
}

color_t sys_get_color() {
	return video_get_color();
}

void sys_set_color(color_t t) {
	video_set_full_color(t);
}

void sys_kbd_set_distribution(keyboard_distrib d) {
	keyboard_set_distribution(d);
}