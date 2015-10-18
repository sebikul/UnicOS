
#include <stdint.h>
#include "task.h"
#include "string.h"
#include "kernel.h"

#define MAX_TASKS 1024

static task_t *first = NULL;
static task_t *current = NULL;

static pid_t current_pid = 0;
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

static void task_set_state(task_t *task, task_state_t newstate) {
	task->state = newstate;
}

void task_init() {

	for (int i = 0; i < VIRTUAL_CONSOLES; i++) {
		task_t *task = task_create(task_shell, "init_shell", 0, NULL);

		task_setconsole(task, i);
		task_ready(task);
	}

	current = first;
}

task_t *task_create(task_entry_point func, const char* name, int argc, char** argv) {

	task_t *task = malloc(sizeof(task_t));

	task->state = TASK_PAUSED;
	task->pid = getnewpid();

	if (func != NULL) {
		task->console = task_get_current()->console;
	} else {
		task->console = KERNEL_CONSOLE;
	}

	task->name = malloc(strlen(name) + 1);

	memcpy(task->name, name, strlen(name) + 1);

	intsoff();
	task_add(task);
	intson();

	return task;
}

void task_ready(task_t *task) {
	task->state = TASK_RUNNING;
}

void task_setconsole(task_t *task, console_t console) {
	task->console = console;
}

task_t* task_get_current() {
	return current;
}
