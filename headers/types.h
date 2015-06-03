#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define NULL (void*)0

#define TRUE 1
#define FALSE 0

typedef char bool;

#define EOF 	-1

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

//todo moverlos a un .h separado. podemos hacer un types.h, etc.
#define video_get_fg(color) (0x0F & color)
#define video_get_bg(color) ((0xF0 & color) >> 4)

//Devuelve 8 bites, formato bgfg
#define BUILD_COLOR(fg,bg) (color_t)((bg << 4) | fg)

typedef uint8_t color_t;

typedef enum {
	KEYBOARD_USA,
	KEYBOARD_LATIN
} keyboard_distrib;

typedef void (*dka_handler)(uint64_t s);

#endif