#include <types.h>
#include "task.h"

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_BUFFER_SIZE 128
#define NOCHAR (char)0
#define MAX_KBD_HANDLERS 256

typedef struct {
	bool caps;
	bool ctrl;
	bool alt;
	bool shift;
} kstatus;

typedef struct {
	uint64_t scancode;
	dka_handler handler;
	task_t  *task;
	uint64_t flags;
	char *name;
} dka_catch;

void keyboard_init();
int keyboard_wait_for_buffer(int len);
char keyboard_get_char_from_buffer();
int32_t keyboard_catch(uint64_t scancode, dka_handler handler, task_t *task, uint64_t flags, char* name);
void keyboard_clear_handler(uint32_t index);
void keyboard_replace_buffer(char* s);
void keyboard_set_distribution(keyboard_distrib d);
void keyboard_clear_from_task(task_t *task);

void keyboard_change_console(console_t console);

#endif
