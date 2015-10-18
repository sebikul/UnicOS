#include <stdint.h>
#include "video.h"
#include "mem.h"
#include "string.h"

#ifndef TASK_H
#define TASK_H

typedef uint64_t pid_t;

typedef enum {TASK_PAUSED, TASK_RUNNING, TASK_SLEEPING} task_state_t;

//Codigo adaptado de https://github.com/a-darwish/cuteOS para definir las estructuras de control.

typedef struct {
	uint64_t rbp;
	uint64_t rbx;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;

	uint64_t rsp;

} task_pcb_t;

typedef struct {
	/* ABI scratch registers. Pushed by us on IRQ
	 * handlers entry to freely call C code from
	 * those handlers without corrupting state */
	uint64_t r11;			/* 0x0 (%rsp) */
	uint64_t r10;			/* 0x8 (%rsp) */
	uint64_t r9;			/* 0x10(%rsp) */
	uint64_t r8;			/* 0x18(%rsp) */
	uint64_t rsi;			/* 0x20(%rsp) */
	uint64_t rdi;			/* 0x28(%rsp) */
	uint64_t rdx;			/* 0x30(%rsp) */
	uint64_t rcx;			/* 0x38(%rsp) */
	uint64_t rax;			/* 0x40(%rsp) */

	/* Regs pushed by CPU directly before invoking
	 * IRQ handlers.
	 *
	 * Those are %RIP and stack of the _interrupted_
	 * code, where all handlers including the ticks
	 * context-switching code will 'resume' to */
	uint64_t rip;			/* 0x48(%rsp) */
	uint64_t cs;			/* 0x50(%rsp) */
	uint64_t rflags;		/* 0x58(%rsp) */
	uint64_t rsp;			/* 0x60(%rsp) */
	uint64_t ss;			/* 0x68(%rsp) */
} irq_ctx_t;

typedef struct task_t {
	struct task_t *next;

	task_pcb_t pcb;

	char *name;
	pid_t pid;
	task_state_t state;
	uint8_t console;
} task_t;

typedef uint64_t (*task_entry_point)(int argc, char** argv);

void task_init();
task_t *task_create(task_entry_point func, const char* name, int argc, char** argv);

void task_ready(task_t *task);
void task_pause(task_t *task);
void task_sleep(task_t *task);

void task_setconsole(task_t *task, console_t console);
task_t* task_get_current();

task_t* task_next();

void task_save_current_stack(void* rsp);
void* task_get_current_stack();

#endif