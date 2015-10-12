#include <stdint.h>
#include "video.h"
#include "mem.h"
#include "string.h"

#ifndef TASK_H
#define TASK_H

typedef uint32_t pid_t;

typedef enum {TASK_PAUSED, TASK_RUNNING, TASK_SLEEPING} task_state_t;

typedef struct {
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rip;

	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;

	uint64_t rbp;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;

	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} context_t;

typedef struct {
	pid_t pid;
	task_state_t state;
	context_t context;
	char *name;
	uint8_t console;
} task_t;

typedef uint64_t (*task_entry_point)(int argc, char** argv);

void task_init();
task_t *task_create(task_entry_point func, const char* name, int argc, char** argv);
task_t* task_get_current();

#endif