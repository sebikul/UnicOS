#include <types.h>
#include "task.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_BUFFER_SIZE 128

typedef struct {
	char scancode;
	char ascii;
	char caps;
} scancode;

typedef struct {
	bool caps;
	bool ctrl;
	bool alt;
} kstatus;

typedef struct {
	uint64_t scancode;
	dka_handler handler;
	unsigned int console;
	pid_t pid;
} dka_catch;


int keyboard_wait_for_buffer(int len);
char keyboard_get_char_from_buffer();
void keyboard_catch(uint64_t scancode, dka_handler handler, unsigned int console, pid_t pid);
void keyboard_replace_last_written(char* s);
void keyboard_set_distribution(keyboard_distrib d);

#endif
