
#include "syscalls.h"
#include "types.h"
#include "video.h"
#include "keyboard.h"
#include "rtc.h"
#include "string.h"
#include "mem.h"
#include "task.h"
#include "kernel.h"

extern uint64_t screensaver_wait_time;
extern bool screensaver_is_active;

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
		video_write_string(task_get_current()->console, s);
		break;

	case FD_STDERR:

		colorbk = video_get_color(task_get_current()->console);

		video_set_color(task_get_current()->console, COLOR_RED, COLOR_BLACK);

		video_write_string(task_get_current()->console, s);

		video_set_full_color(task_get_current()->console, colorbk);
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

uint64_t sys_keyboard_catch(uint64_t scancode, dka_handler handler) {
	return keyboard_catch(scancode, handler, task_get_current()->console, task_get_current()->pid);
}

void sys_clear_indexed_line(int index) {
	video_clear_indexed_line(task_get_current()->console, index);
}

void sys_keyboard_replace_buffer(char* s) {
	keyboard_replace_buffer(s);
}

color_t sys_get_color() {
	return video_get_color(task_get_current()->console);
}

void sys_set_color(color_t t) {
	video_set_full_color(task_get_current()->console, t);
	video_update_screen_color(task_get_current()->console);
}

void sys_kbd_set_distribution(keyboard_distrib d) {
	keyboard_set_distribution(d);
}

void sys_set_screensaver_timer(uint64_t t) {

	screensaver_wait_time = t;
	screensaver_reset_timer();
}

void sys_clear_screen() {
	video_clear_screen(task_get_current()->console);
}

void sys_screensaver_trigger() {

	active_screensaver();

	while (screensaver_is_active);

}

void sys_keyboard_clear_handler(uint64_t handler) {
	keyboard_clear_handler(handler);
}