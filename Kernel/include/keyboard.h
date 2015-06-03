
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <syscalls.h>

#define NULL 0

#define TRUE 1
#define FALSE 0

#define KEYBOARD_BUFFER_SIZE 128

typedef char bool;

typedef struct {
	char scancode;
	char ascii;
	char caps;
} scancode;

//vamos a sacrificar mucho espacio para que el codigo sea mas legible. Se podrian usar mascaras
typedef struct {
	bool caps;
	bool ctrl;
	bool alt;
} kstatus;

typedef struct {
	uint64_t scancode;
	dka_handler handler;
} dka_catch;


int keyboard_wait_for_buffer(int len);
char keyboard_get_char_from_buffer();
void keyboard_catch(uint64_t scancode, dka_handler handler);
void keyboard_replace_last_written(char* s);
void keyboard_set_distribution(keyboard_distrib d);

#endif








