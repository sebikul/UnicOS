
#include <stdint.h>
#include "task.h"
#include "string.h"

#define MAX_TASKS 1024

static task_t* tasks[MAX_TASKS] = {0};
static pid_t current_pid = 0;
static pid_t nextpid = 0;

static pid_t getnewpid() {
	pid_t pid = nextpid;
	nextpid++;

	return pid;
}

void task_init() {

	for (int i = 0; i < VIRTUAL_CONSOLES; i++) {
		task_t *task = task_create(NULL, "init", 0, NULL);

		task_setconsole(task, i);
	}
}

static void task_add(task_t *task) {
	tasks[task->pid] = task;
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

	task_add(task);

	return task;
}

void task_setconsole(task_t *task, console_t console) {
	task->console = console;
}

task_t* task_get_current() {
	return tasks[current_pid];
}