#include <stdint.h>
#include "video.h"
#include "mem.h"
#include "string.h"

#ifndef TASK_H
#define TASK_H

typedef uint64_t pid_t;

typedef enum {TASK_PAUSED, TASK_RUNNING, TASK_SLEEPING} task_state_t;


typedef struct task_t{
	struct task_t *next;

	char *name;
	void *stack;
	pid_t pid;
	task_state_t state;
	uint8_t console;
} task_t;

typedef uint64_t (*task_entry_point)(int argc, char** argv);

void task_init();
task_t *task_create(task_entry_point func, const char* name, int argc, char** argv);
task_t* task_get_current();
void task_setconsole(task_t *task, console_t console);

#endif