
#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include <types.h>

#define SYSCALL_RTC							1
#define SYSCALL_RTC_SET						2
#define SYSCALL_READ						3
#define SYSCALL_WRITE						4
#define SYSCALL_MALLOC						5
#define SYSCALL_CALLOC						6
#define SYSCALL_FREE						7
#define SYSCALL_KEYBOARD_CATCH 				8
#define SYSCALL_VIDEO_CLR_INDEXED_LINE 		9
#define SYSCALL_KEYBOARD_REPLACE_BUFFER		10
#define SYSCALL_GET_COLOR					11
#define SYSCALL_SET_COLOR					12
#define SYSCALL_SET_TIME					13
#define SYSCALL_SET_KBD_DISTRIBUTION		14
#define SYSCALL_SCREENSAVER_TIMER			15

void sys_write(FD fd, char* s, int len);
int sys_read(FD fd, char* s, int len);
void sys_rtc_get(time_t* t);
void sys_rtc_set(time_t* t);
void* sys_malloc(int len);
void* sys_calloc(int len);
void sys_free(void* m);
void sys_keyboard_catch(uint64_t scancode, dka_handler handler);
void sys_clear_indexed_line(int index);
void sys_keyboard_replace_buffer(char* s);
void sys_set_color(color_t t);
color_t sys_get_color();
void sys_kbd_set_distribution(keyboard_distrib d);
void sys_set_screensaver_timer(uint64_t t);

#endif
