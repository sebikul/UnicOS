#include <stdint.h>

#ifndef TYPES_H
#define TYPES_H

#define NULL (void*)0

typedef enum {FALSE, TRUE} bool;

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

#define video_get_fg(color) (0x0F & color)
#define video_get_bg(color) ((0xF0 & color) >> 4)

//Devuelve 8 bites, formato bgfg
#define BUILD_COLOR(fg,bg) (color_t)((bg << 4) | fg)

typedef uint8_t color_t;

#define KEYBOARD_IGNORE 		(1<<1) 		//Decide si se debe imprimir el scancode en pantalla o no. Si por lo menos un handler lo tiene desactivado entonces se muestra.
#define KEYBOARD_RANGE 			(1<<2)		//Divide al scancode en un rango de scancodes a analizar
#define KEYBOARD_WILDCARD 		(1<<3)		//No analiza el scancode recibido
#define KEYBOARD_ALLCONSOLES 	(1<<4)

typedef enum {
	KEYBOARD_USA,
	KEYBOARD_LATIN
} keyboard_distrib;

typedef void (*dka_handler)(uint64_t s);

typedef uint64_t (*task_entry_point)(int argc, char** argv);
typedef uint64_t pid_t;
typedef enum {TASK_PAUSED, TASK_RUNNING, TASK_SLEEPING, TASK_JOINING, TASK_STOPPED} task_state_t;

typedef struct task_t {
	struct task_t *next;
	struct task_t *join;

	void *stack;

	char *name;
	pid_t pid;
	task_state_t state;
	uint8_t console;
} task_t;

#endif
