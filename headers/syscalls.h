#include <stdint.h>
#include <types.h>

#ifndef SYSCALLS_H
#define SYSCALLS_H

typedef enum { SYSCALL_RTC,
               SYSCALL_RTC_SET,
               SYSCALL_READ,
               SYSCALL_WRITE,
               SYSCALL_MALLOC,
               SYSCALL_CALLOC,
               SYSCALL_FREE,
               SYSCALL_KEYBOARD_CATCH,
               SYSCALL_VIDEO_CLR_INDEXED_LINE,
               SYSCALL_KEYBOARD_REPLACE_BUFFER,
               SYSCALL_GET_COLOR,
               SYSCALL_SET_COLOR,
               SYSCALL_SET_TIME,
               SYSCALL_SET_KBD_DISTRIBUTION,
               SYSCALL_SCREENSAVER_TIMER,
               SYSCALL_SCREENSAVER_TRIGGER,
               SYSCALL_CLEAR_SCREEN,
               SYSCALL_EXIT,
               SYSCALL_KEYBOARD_CLEAR_HANDLER,
               SYSCALL_KDEBUG,
               SYSCALL_TASK_CREATE,
               SYSCALL_TASK_READY
} syscall_t;

void sys_write(FD fd, char* s, uint64_t len);
uint64_t sys_read(FD fd, char* s, uint64_t len);
void sys_rtc_get(time_t* t);
void sys_rtc_set(time_t* t);
void* sys_malloc(uint64_t len);
void* sys_calloc(uint64_t len);
void sys_free(void* m);
uint64_t sys_keyboard_catch(uint64_t scancode, dka_handler handler, uint64_t flags);
void sys_clear_indexed_line(uint64_t index);
void sys_keyboard_replace_buffer(char* s);
void sys_set_color(color_t t);
color_t sys_get_color();
void sys_kbd_set_distribution(keyboard_distrib d);
void sys_set_screensaver_timer(uint64_t t);
void sys_clear_screen();
void sys_screensaver_trigger();
void sys_exit();
void sys_keyboard_clear_handler(uint64_t index);
void hang();
void sys_kdebug(char* str);

pid_t sys_task_create(task_entry_point func, const char* name, int argc, char** argv);
void sys_task_ready(pid_t pid);

#define sSTR_HELPER(x) #x
#define sSTR(x) sSTR_HELPER(x)
#define ksysdebug(str) sys_kdebug(__FILE__ ":" sSTR(__LINE__) ": " str)
#define ksysdebugs(str) (ksysdebug(""), sys_kdebug(str), sys_kdebug("\n"))

#endif
