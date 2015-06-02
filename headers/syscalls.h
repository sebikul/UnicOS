
#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define SYSCALL_RTC							2
#define SYSCALL_READ						3
#define SYSCALL_WRITE						4
#define SYSCALL_MALLOC						5
#define SYSCALL_CALLOC						6
#define SYSCALL_FREE						7
#define SYSCALL_KEYBOARD_CATCH 				8
#define SYSCALL_VIDEO_CLR_INDEXED_LINE 		9
#define SYSCALL_KEYBOARD_REPLACE_BUFFER		10
#define SYSCALL_COLOR											11

enum FD{
	FD_STDERR,
	FD_STDOUT
};

typedef struct {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t day;
	uint8_t month;
	uint32_t year;//ultimos dos digitos
} time_t;


typedef struct {
	uint8_t fontColor;
	uint8_t	backgroundColor;
} color_t;

typedef enum FD FD;

typedef void (*dka_handler)(uint64_t s);

void sys_write(FD fd, char* s, int len);
int sys_read(FD fd, char* s, int len);
void sys_rtc_time(time_t* t) ;
void* sys_malloc(int len);
void* sys_calloc(int len);
void sys_free(void* m);
void sys_keyboard_catch(uint64_t scancode, dka_handler handler);
void sys_clear_indexed_line(int index);
void sys_keyboard_replace_buffer(char* s);
void sys_color(color_t * t);

#endif
