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
#include "filesystem.h"

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

	case SYSCALL_FS_OPEN:
		return sys_open((const char*) rsi, rdx);
		break;

	case SYSCALL_FS_READ:
		return sys_read((int32_t) rsi, (char*)rdx, (uint32_t)rcx);
		break;

	case SYSCALL_FS_WRITE:
		return  sys_write((int32_t) rsi, (const char*)rdx, (uint32_t)rcx);
		break;

	case SYSCALL_FS_CLOSE:
		sys_close((int32_t) rsi);
		break;

	case SYSCALL_FS_SIZE:
		return sys_size((int32_t) rsi);
		break;

	case SYSCALL_FS_FSTREE:
		sys_fstree();
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

int32_t sys_open(const char* path, uint64_t flags) {

	task_t *task = task_get_current();

	kdebug("Syscall open\n");

	for (uint32_t i = 3; i < MAX_FS_CHILDS; i++) {
		if (task->files[i].file == NULL) {

			file_t *file = fs_open(path, 0);

			if (file == NULL) {
				return -1;
			}

			task->files[i].file = file;

			if (flags & O_APPEND) {
				task->files[i].cursor = file->size;
			} else if (flags & O_TRUNC) {
				free(file->start);
				file->start = NULL;
				file->size = 0;
			} else {
				task->files[i].cursor = 0;
			}

			task->files[i].flags = flags;

			return i;
		}
	}

	return -2;
}

int32_t sys_read(int32_t fd, char* buf, uint32_t size) {

	task_t *task = task_get_current();
	int32_t len = 0;
	fd_t *fds;

	kdebug("Syscall read. fd: ");
	kdebug_base(fd, 10);
	kdebug_nl();

	switch (fd) {
	case stdin:

		kdebug("Esperando entrada\n");

		task_pause(task_get_current());

		kdebug("Entrada recibida!\n");

		while (len < size) {
			buf[len] = input_getc();

			if (buf[len] == '\n') {
				break;
			}
			len++;
		}

		buf[len] = 0;

		kdebugs(buf);
		kdebug("Caracteres ingresados: ");
		kdebug_base(len, 10);
		kdebug_nl();

		return len;
		break;

	default:

		fds = &(task->files[fd]);

		if (!((fds->flags & O_RDWR) || (fds->flags & O_RDONLY))) {
			return -2;
		}

		if (fds->file == NULL) {
			return -1;
		}

		kdebug("Reading content of file: ");
		_kdebug(fds->file->name);
		_kdebug(" size: ");
		kdebug_base(size, 10);
		kdebug_nl();

		len = fs_read(fds->file, buf, size, fds->cursor);

		kdebug("Read: '");
		_kdebug(buf);
		_kdebug("'");
		kdebug_nl();

		fds->cursor += len;

		return len;
	}
}

int32_t sys_write(int32_t fd, const char* data, uint32_t size) {

	color_t colorbk;
	task_t *task = task_get_current();
	fd_t *fds;
	int32_t len = 0;

	kdebug("Syscall write. fd: ");
	kdebug_base(fd, 10);
	kdebug_nl();

	switch (fd) {
	case stdout:
		kdebug("Writing to console ");
		kdebug_base(task->console, 10);
		kdebug_nl();

		video_write_string(task->console, data);
		return size;
		break;

	case stderr:

		colorbk = video_get_color(task->console);
		video_set_color(task->console, COLOR_RED, COLOR_BLACK);
		video_write_string(task->console, data);
		video_set_full_color(task->console, colorbk);
		return size;
		break;

	default:
		fds = &(task->files[fd]);

		if (!((fds->flags & O_RDWR) || (fds->flags & O_WRONLY))) {
			return -2;
		}

		if (fds->file == NULL) {
			return -1;
		}

		kdebug("Writing to fs: ");
		_kdebug(data);
		_kdebug(" size: ");
		kdebug_base(size, 10);
		kdebug_nl();

		len = fs_write(fds->file, data, size, fds->cursor);

		fds->cursor += len;

		return len;
	}
}

void sys_close(int32_t fd) {

	task_t *task = task_get_current();

	kdebug("Syscall clsoe\n");

	task->files[fd].file = NULL;
	task->files[fd].cursor = 0;
}

uint32_t sys_size(int32_t fd) {

	task_t *task = task_get_current();
	fd_t *fds;

	fds = &(task->files[fd]);

	if (fds->file == NULL) {
		return -1;
	}

	return fds->file->size;
}

void sys_fstree() {
	fs_dump();
}

