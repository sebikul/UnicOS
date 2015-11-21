#include <stdint.h>

#ifndef TYPES_H
#define TYPES_H

#define NULL (void*)0

typedef enum {FALSE, TRUE} bool;

#define EOF 	-1

typedef enum {
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


// typedef enum FD {
// 	stderr,
// 	FD_STDOUT
// } FD;

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

typedef enum {
	SIGINT,
	SIGKILL,
	SIGUSR1,
	SIGUSR2,

	//Debe ser el ultimo para mantener la cuenta!
	SIGCOUNT
} signal_t;

typedef void (*sighandler_t)(signal_t s);

typedef enum {TASK_PAUSED, TASK_RUNNING, TASK_SLEEPING, TASK_JOINING, TASK_ZOMBIE} task_state_t;

typedef enum {TASK_FOREGROUND, TASK_BACKGROUND} task_mode_t;

#define MAX_TASK_KBD_HANDLERS 16
#define MAX_FS_CHILDS 16

typedef struct file file_t;

typedef struct {
	file_t* file;
	uint64_t cursor;
	uint64_t flags;
} fd_t;

#define O_RDONLY 	(1<<1) 
#define O_WRONLY 	(1<<2)
#define O_RDWR 		(1<<3)
#define O_APPEND 	(1<<4)
#define O_CREAT 	(1<<5)
#define O_TRUNC 	(1<<6)

#define stdin  0
#define stdout 1
#define stderr 2

typedef struct task_t {
	struct task_t* prev;
	struct task_t* next;
	struct task_t* join;
	void* stack;
	char* name;
	pid_t pid;
	uint64_t sleep_limit;
	uint64_t retval;
	uint64_t atomic_level;
	uint64_t quantum;
	sighandler_t sighandlers[SIGCOUNT];
	int64_t kbdhandlers[MAX_TASK_KBD_HANDLERS];
	fd_t files[MAX_FS_CHILDS];
	task_state_t state;
	uint8_t console;
	uint8_t errno;
} task_t;

#define EINVALID_TASK 1
#define ENOT_FOUND 2
#define ENO_ROOT 3
#define EFD_TABLE_FULL 4
#define ENO_PERM 5
#define EINV_OP 6

#endif
