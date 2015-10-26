#include <stdint.h>
#include "syscalls.h"
#include "types.h"
#include "video.h"
#include "keyboard.h"
#include "rtc.h"
#include "string.h"
#include "mem.h"
#include "task.h"
#include "kernel.h"
#include "input.h"

extern uint64_t screensaver_wait_time;
extern bool screensaver_is_active;

uint64_t irq80_handler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8) {

	//kdebug("Despachando syscall int 80h\n");

	switch (rdi) {

	case SYSCALL_RTC:
		sys_rtc_get((time_t*)rsi);
		break;

	case SYSCALL_RTC_SET:
		sys_rtc_set((time_t*)rsi);
		break;

	case SYSCALL_READ:
		return sys_read((FD)rsi, (char*)rdx, rcx);
		break;

	case SYSCALL_WRITE:
		sys_write((FD)rsi, (char*)rdx, rcx);
		break;

	case SYSCALL_MALLOC:
		return (uint64_t) sys_malloc(rsi);
		break;

	case SYSCALL_CALLOC:
		return (uint64_t) sys_calloc(rsi);
		break;

	case SYSCALL_FREE:
		sys_free((void*)rsi);
		break;

	case SYSCALL_KEYBOARD_CATCH:
		return sys_keyboard_catch(rsi, (dka_handler) rdx, rcx);
		break;

	case SYSCALL_VIDEO_CLR_INDEXED_LINE:
		sys_clear_indexed_line(rsi);
		break;

	case SYSCALL_KEYBOARD_REPLACE_BUFFER:
		sys_keyboard_replace_buffer((char*)rsi);
		break;

	case SYSCALL_GET_COLOR:
		return (uint64_t) sys_get_color();
		break;

	case SYSCALL_SET_COLOR:
		sys_set_color((color_t) rsi);
		break;

	case SYSCALL_SET_KBD_DISTRIBUTION:
		sys_kbd_set_distribution(rsi);
		break;

	case SYSCALL_SCREENSAVER_TIMER:
		sys_set_screensaver_timer(rsi);
		break;

	case SYSCALL_SCREENSAVER_TRIGGER:
		sys_screensaver_trigger();
		break;

	case SYSCALL_CLEAR_SCREEN:
		sys_clear_screen();
		break;

	case SYSCALL_EXIT:
		//hang();
		break;

	case SYSCALL_KEYBOARD_CLEAR_HANDLER:
		sys_keyboard_clear_handler(rsi);
		break;

	case SYSCALL_KDEBUG:
		sys_kdebug((char*)rsi);
		break;
	}

	return 0;
}

void sys_rtc_get(time_t* t) {
	rtc_get_time(t);
}

void sys_rtc_set(time_t* t) {
	rtc_set_time(t);
}

void sys_write(FD fd, char* s, uint64_t len) {

	color_t colorbk;

	switch (fd) {
	case FD_STDOUT:
		kdebug("Writing to console ");
		kdebug_base(task_get_current()->console, 10);
		kdebug_nl();

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

uint64_t sys_read(FD fd, char* s, uint64_t len) {

	uint64_t i = 0;

	kdebug("Esperando entrada\n");

	task_sleep(task_get_current());
	//reschedule();

	kdebug("Entrada recibida!\n");

	while (i < len) {
		s[i] = input_getc();

		if (s[i] == '\n') {
			break;
		}
		i++;
	}

	s[i] = 0;

	kdebugs(s);
	kdebug("Caracteres ingresados: ");
	kdebug_base(i, 10);
	kdebug_nl();

	return i;
}

void* sys_malloc(uint64_t len) {
	return malloc(len);
}

void* sys_calloc(uint64_t len) {
	return calloc(len);
}

void sys_free(void* m) {
	free(m);
}

uint64_t sys_keyboard_catch(uint64_t scancode, dka_handler handler, uint64_t flags) {
	//Un proceso de usersoace no deberia poder imprimir en todas las consolas
	flags = flags & ~KEYBOARD_ALLCONSOLES;
	
	return keyboard_catch(scancode, handler, task_get_current()->console, task_get_current()->pid, flags);
}

void sys_clear_indexed_line(uint64_t index) {
	video_clear_indexed_line(task_get_current()->console, index);
}

void sys_keyboard_replace_buffer(char* s) {
	input_replace(s);
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

void sys_kdebug(char *str) {
	_kdebug(str);
}