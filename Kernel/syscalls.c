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
#include "signal.h"

extern uint64_t pit_timer;

uint64_t irq80_handler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {

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
		return sys_keyboard_catch(rsi, (dka_handler) rdx, rcx, (char*)r8);
		break;

	case SYSCALL_VIDEO_CLR_INDEXED_LINE:
		sys_clear_indexed_line(rsi);
		break;

	case SYSCALL_VIDEO_RESET_CURSOR:
		sys_reset_cursor();
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

	case SYSCALL_TASK_CREATE:
		return sys_task_create((task_entry_point) rsi, (task_mode_t) rdx, (char*) rcx, (int)r8, (char**) r9);
		break;

	case SYSCALL_TASK_READY:
		sys_task_ready((pid_t) rsi);
		break;

	case SYSCALL_TASK_JOIN:
		return sys_task_join((pid_t) rsi, (pid_t) rdx);
		break;

	case SYSCALL_TASK_GET_PID:
		return sys_task_get_pid();
		break;

	case SYSCALL_TASK_YIELD:
		sys_task_yield();
		break;

	case SYSCALL_TASK_GETALL:
		return (uint64_t)sys_task_getall();
		break;

	case SYSCALL_SLEEP:
		sys_sleep(rsi);
		break;

	case SYSCALL_UPTIME:
		return sys_uptime();
		break;

	case SYSCALL_ATOMIC:
		sys_atomic();
		break;

	case SYSCALL_UNATOMIC:
		sys_unatomic();
		break;

	case SYSCALL_SIGNAL_KILL:
		sys_signal_kill((pid_t) rsi, (signal_t) rdx);
		break;

	case SYSCALL_SIGNAL_SET:
		sys_signal_set((signal_t) rsi, (sighandler_t) rdx);
		break;

	default:
		kdebug("ERROR: INVALID SYSCALL: ");
		kdebug_base(rdi, 10);
		kdebug_nl();
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
	task_t *current = task_get_current();


	switch (fd) {
	case FD_STDOUT:
		kdebug("Writing to console ");
		kdebug_base(current->console, 10);
		kdebug_nl();

		video_write_string(current->console, s);
		break;

	case FD_STDERR:

		colorbk = video_get_color(current->console);

		video_set_color(current->console, COLOR_RED, COLOR_BLACK);

		video_write_string(current->console, s);

		video_set_full_color(current->console, colorbk);
		break;
	}
}

uint64_t sys_read(FD fd, char* s, uint64_t len) {

	uint64_t i = 0;

	kdebug("Esperando entrada\n");

	task_pause(task_get_current());

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

int32_t sys_keyboard_catch(uint64_t scancode, dka_handler handler, uint64_t flags, char* name) {
	//Un proceso de usersoace no deberia poder imprimir en todas las consolas
	flags = flags & ~KEYBOARD_ALLCONSOLES;
	task_t *current = task_get_current();

	return keyboard_catch(scancode, handler, current, flags, name);
}

void sys_clear_indexed_line(uint64_t index) {
	video_clear_indexed_line(task_get_current()->console, index);
}

void sys_reset_cursor() {
	video_reset_cursor(video_current_console());
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
	screensaver_set_wait(t);
}

void sys_clear_screen() {
	video_clear_screen(task_get_current()->console);
}

void sys_screensaver_trigger() {
	screensaver_trigger();
}

void sys_keyboard_clear_handler(uint32_t handler) {
	keyboard_clear_handler(handler);
}

void sys_kdebug(char *str) {
	_kdebug(str);
}

pid_t sys_task_create(task_entry_point func, task_mode_t mode, const char* name, int argc, char** argv) {
	task_t *task;
	console_t curr_console = task_get_current()->console;

	task = task_create(func, name, argc, argv);
	task_setconsole(task, curr_console);

	if (mode == TASK_FOREGROUND) {
		task_set_foreground(task, curr_console);
	}

	return task->pid;
}

void sys_task_ready(pid_t pid) {
	task_t *task = task_find_by_pid(pid);

	if (task == NULL) {
		return;
	}
	task_ready(task);
}

uint64_t sys_task_join(pid_t pid, pid_t otherpid) {
	task_t *task = task_find_by_pid(pid);
	task_t *other = task_find_by_pid(otherpid);

	if (task == NULL) {
		//TODO errno
		return 0;
	}
	return task_join(task, other);
}

pid_t sys_task_get_pid() {
	return task_get_current()->pid;
}

void sys_task_yield() {
	kdebug("Yielding CPU to next task\n");
	sys_sleep(0);
}

task_t* sys_task_getall() {
	return task_get_first();
}

void sys_sleep(uint64_t ms) {
	task_t *task = task_get_current();
	task_sleep(task, ms);
}

uint64_t sys_uptime() {
	return pit_timer;
}

void sys_atomic() {
	task_atomic(task_get_current());
}

void sys_unatomic() {
	task_unatomic(task_get_current());
}

void sys_signal_kill(pid_t pid, signal_t sig) {
	task_t *task = task_find_by_pid(pid);

	if (task == NULL) {
		//TODO errno
		return;
	}

	kdebug("Executing signal handler for task: ");
	_kdebug(task->name);
	_kdebug("' pid=");
	kdebug_base(task->pid, 10);
	_kdebug(" signal: ");
	kdebug_base(sig, 10);
	kdebug_nl();

	signal_send(task, sig);
}

void sys_signal_set(signal_t sig, sighandler_t handler) {
	task_t *task = task_get_current();

	kdebug("Setting signal handler for task: ");
	_kdebug(task->name);
	_kdebug("' pid=");
	kdebug_base(task->pid, 10);
	_kdebug(" signal: ");
	kdebug_base(sig, 10);
	kdebug_nl();

	signal_set(task, sig, handler);
}
