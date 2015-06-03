
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
#define SYSCALL_GET_COLOR					11
#define SYSCALL_SET_COLOR					12
#define SYSCALL_SET_TIME					13


//todo moverlos a un .h separado. podemos hacer un types.h, etc.
#define video_get_fg(color) (0x0F & color)
#define video_get_bg(color) ((0xF0 & color) >> 4)
typedef enum{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
} vga_color;


typedef enum FD{
	FD_STDERR,
	FD_STDOUT
} FD;

typedef struct {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t day;
	uint8_t month;
	uint32_t year;
} time_t;

typedef uint8_t color_t;

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
void sys_set_color(color_t t);
color_t sys_get_color();

#endif
