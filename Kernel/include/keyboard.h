#include <types.h>
#include "task.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_BUFFER_SIZE 128
#define NOCHAR (char)0

typedef struct {
	bool caps;
	bool ctrl;
	bool alt;
	bool shift;
} kstatus;

typedef struct {
	uint64_t scancode;
	dka_handler handler;
	pid_t pid;
	uint64_t flags;
	console_t console;
	char* name;
} dka_catch;

void keyboard_init();
int keyboard_wait_for_buffer(int len);
char keyboard_get_char_from_buffer();
int keyboard_catch(uint64_t scancode, dka_handler handler, console_t console, pid_t pid, uint64_t flags, char* name);
void keyboard_clear_handler(int index);
void keyboard_replace_buffer(char* s);
void keyboard_set_distribution(keyboard_distrib d);

#endif
