#include <stdint.h>
#include "video.h"
#include "mem.h"
#include "string.h"
#include "types.h"

#ifndef TASK_H
#define TASK_H

typedef enum {TASK_PAUSED, TASK_RUNNING, TASK_SLEEPING, TASK_STOPPED} task_state_t;

typedef struct {
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
	
	uint64_t base;
} __attribute__((packed)) context_t;

typedef struct task_t {
	struct task_t *next;

	void *stack;

	char *name;
	pid_t pid;
	task_state_t state;
	uint8_t console;
} task_t;

void task_init();
task_t *task_create(task_entry_point func, const char* name, int argc, char** argv);

void task_ready(task_t *task);
void task_pause(task_t *task);
void task_sleep(task_t *task);

void task_setconsole(task_t *task, console_t console);
task_t* task_get_current();

void task_next();

void task_set_foreground(task_t *task, console_t console);
task_t* task_get_foreground(console_t console);

#endif