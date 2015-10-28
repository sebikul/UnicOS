#include <syscalls.h>
#include <types.h>
#include <stdint.h>

extern uint64_t syscall(uint64_t callid, ...);

void sys_write(FD fd, char* s, uint64_t len) {

	syscall((uint64_t)SYSCALL_WRITE, (uint64_t)fd, (uint64_t)s, (uint64_t)len);

}

uint64_t sys_read(FD fd, char* s, uint64_t len) {

	return syscall((uint64_t)SYSCALL_READ, (uint64_t)fd, (uint64_t)s, (uint64_t)len);

}

void sys_rtc_get(time_t* t) {
	syscall((uint64_t)SYSCALL_RTC, (uint64_t)t);
}

void sys_rtc_set(time_t* t) {
	syscall((uint64_t)SYSCALL_RTC_SET, (uint64_t)t);
}

void* sys_malloc(uint64_t len) {
	return (void*)syscall((uint64_t)SYSCALL_MALLOC, len);
}

void* sys_calloc(uint64_t len) {
	return (void*)syscall((uint64_t)SYSCALL_CALLOC, len);
}

void sys_free(void* m) {
	syscall((uint64_t)SYSCALL_FREE, (uint64_t)m);
}

uint64_t sys_keyboard_catch(uint64_t scancode, dka_handler handler, uint64_t flags) {
	return syscall((uint64_t)SYSCALL_KEYBOARD_CATCH, scancode, (uint64_t)handler, flags);
}

void sys_clear_indexed_line(uint64_t index) {
	syscall((uint64_t)SYSCALL_VIDEO_CLR_INDEXED_LINE, index);
}

void sys_keyboard_replace_buffer(char* s) {
	syscall((uint64_t)SYSCALL_KEYBOARD_REPLACE_BUFFER, (uint64_t)s);
}

color_t sys_get_color() {
	return (color_t)syscall((uint64_t)SYSCALL_GET_COLOR);
}

void sys_set_color(color_t t) {
	syscall((uint64_t)SYSCALL_SET_COLOR, (uint64_t)t);
}

void sys_kbd_set_distribution(keyboard_distrib d) {
	syscall((uint64_t)SYSCALL_SET_KBD_DISTRIBUTION, (uint64_t)d);
}

void sys_set_screensaver_timer(uint64_t t) {

	syscall((uint64_t)SYSCALL_SCREENSAVER_TIMER, t);
}

void sys_clear_screen() {
	syscall((uint64_t)SYSCALL_CLEAR_SCREEN);
}

void sys_screensaver_trigger() {
	syscall((uint64_t) SYSCALL_SCREENSAVER_TRIGGER);
}

void sys_exit() {
	syscall((uint64_t)SYSCALL_EXIT);
}

void sys_keyboard_clear_handler(uint64_t index) {
	syscall((uint64_t)SYSCALL_KEYBOARD_CLEAR_HANDLER, index);
}

void sys_kdebug(char* str) {
	syscall((uint64_t)SYSCALL_KDEBUG, str);
}

pid_t sys_task_create(task_entry_point func, const char* name, int argc, char** argv) {
	return (pid_t)syscall((uint64_t)SYSCALL_TASK_CREATE, (uint64_t)func, (uint64_t)name, (uint64_t)argc, (uint64_t)argv);
}

void sys_task_ready(pid_t pid) {
	syscall((uint64_t)SYSCALL_TASK_READY, pid);
}

void sys_task_join(pid_t pid, pid_t otherpid) {
	syscall((uint64_t) SYSCALL_TASK_JOIN, pid, otherpid);
}

pid_t sys_task_get_pid() {
	return (pid_t)syscall((uint64_t)SYSCALL_TASK_GET_PID);
}

void sys_task_yield() {
	syscall((uint64_t)SYSCALL_TASK_YIELD);
}

task_t* sys_task_getall(){
	return syscall((uint64_t)SYSCALL_TASK_GETALL);
}
