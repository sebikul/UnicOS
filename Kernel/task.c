
#include <stdint.h>
#include "task.h"
#include "string.h"
#include "kernel.h"

#define MAX_TASKS 1024

static task_t *first = NULL;
static task_t *current = NULL;

static pid_t nextpid = 0;

static void* const shellCodeModuleAddress = (void*)0x400000;
static void* const shellDataModuleAddress = (void*)0x500000;

static pid_t getnewpid() {
	pid_t pid = nextpid;
	nextpid++;

	return pid;
}

static uint64_t task_shell(int argc, char** argv) {

	while (TRUE) {

		kdebug("Running shell. argc: ");
		kdebug_base(argc, 10);
		kdebug_nl();

		uint8_t console = task_get_current()->console;

		video_clear_screen(console);

		video_write_string(console, "Console #: ");
		video_write_dec(console, console);
		video_write_nl(console);
		//TODO Ver si se deberia copiar el codigo por cada task
		((task_entry_point)shellCodeModuleAddress)(argc, argv);
	}

	return 0;
}

static inline void task_add(task_t *task) {

	if (first == NULL) {
		task->next = task;
		first = task;
	} else {
		task->next = first->next;
		first->next = task;
	}
}

void task_init() {

	for (int i = 1; i < VIRTUAL_CONSOLES; i++) {
		task_t *task = task_create(task_shell, "init_shell", 0, NULL);

		task_setconsole(task, i);
		task_ready(task);
	}

	current = first;
}

task_t *task_create(task_entry_point func, const char* name, int argc, char** argv) {

	task_t *task = malloc(sizeof(task_t));
	irq_ctx_t *irq_ctx;
	void *stack;

	task->state = TASK_PAUSED;
	task->pid = getnewpid();

	if (func != NULL) {
		task->console = task_get_current()->console;
	} else {
		task->console = KERNEL_CONSOLE;
	}

	task->name = malloc(strlen(name) + 1);
	memcpy(task->name, name, strlen(name) + 1);

	stack = malloc(STACK_SIZE);
	stack = stack + STACK_SIZE;

	irq_ctx = (irq_ctx_t *) (stack - sizeof(irq_ctx_t));

	irq_ctx->rip = (uintptr_t) func;
	irq_ctx->rdi = argc;
	irq_ctx->rsi = (uintptr_t) argv;

	irq_ctx->cs = 0x08;
	irq_ctx->rflags = 0x200;
	irq_ctx->rsp = stack;
	irq_ctx->ss = 0x0;

	task->pcb.rsp=(uintptr_t) irq_ctx;

	//intsoff();
	task_add(task);
	//intson();

	return task;
}

void task_ready(task_t *task) {
	task->state = TASK_RUNNING;
}

void task_pause(task_t *task) {
	task->state = TASK_PAUSED;
}

void task_sleep(task_t *task) {
	task->state = TASK_SLEEPING;
}

void task_setconsole(task_t *task, console_t console) {
	task->console = console;
}

task_t* task_next() {
	task_t *task = current->next;

	while (task->state != TASK_RUNNING) {
		task = task->next;
	}

	return task;
}

task_t* task_get_current() {
	return current;
}
