
#include <stdint.h>
#include "task.h"
#include "string.h"
#include "kernel.h"

static task_t *last = NULL;
static task_t *current = NULL;
static task_t *null_task = NULL;
static task_t *foreground[VIRTUAL_CONSOLES] = {NULL};

static pid_t nextpid = 1;

static void* const shellCodeModuleAddress = (void*)0x400000;
static void* const shellDataModuleAddress = (void*)0x500000;

extern uintptr_t kernel_stack;

#define DUMP_LIST_FROM_CURRENT() {\
	task_t *task = (current == NULL) ? last : current->next;\
	task_t *task2 = task;\
	_kdebug(" Lista: ");\
	do {\
		_kdebug(" --> 0x");\
		kdebug_base(task2, 16);\
		task2 = task2->next;\
	} while (task != task2);\
	_kdebug(" --> 0x");\
	kdebug_base(task2, 16);\
	kdebug_nl();\
}\

static pid_t getnewpid() {
	pid_t pid = nextpid;
	nextpid++;

	return pid;
}

static uint64_t task_shell(int argc, char** argv) {

	while (TRUE) {

		uint8_t console = task_get_current()->console;

		//video_clear_screen(console);

		// video_write_string(console, "Console #: ");
		// video_write_dec(console, console);
		// video_write_nl(console);

		kdebug("Running task with pid: ");
		kdebug_base(task_get_current()->pid, 10);
		kdebug_nl();

		// if (task_get_current()->pid == 4) {
		// 	kdebug("Rescheduling task with pid: ");
		// 	kdebug_base(task_get_current()->pid, 10);
		// 	kdebug_nl();
		// 	task_sleep(task_get_current());
		// 	kdebug("Returned again\n");
		// }

		// while (TRUE);

		((task_entry_point)shellCodeModuleAddress)(argc, argv);

	}

	return 0;
}

static inline void task_add(task_t *task) {

	if (last == NULL) {
		task->next = task;
		last = task;
	} else {
		task->next = last->next;
		last->next = task;
	}

	//first = task;
}

static uint64_t null_task_func(int argc, char** argv) {
	while (TRUE) {
		// kdebug("Looping NULL. pid: ");

		// kdebug_base(current->pid, 10);
		// kdebug_nl();
	}

	return 0;
}

void task_init() {

	for (int i = 0; i < VIRTUAL_CONSOLES; i++) {
		task_t *task = task_create(task_shell, "init_shell", 0, NULL);

		task_setconsole(task, i);
		task_set_foreground(task, i);
		task_ready(task);
	}

	null_task = task_create(null_task_func, "null_task", 0, NULL);

	task_setconsole(null_task, 0);
	task_ready(null_task);

	//Seteamos la tarea nula como la actual
	//current = task;
}

static void wrapper(task_entry_point func, int argc, char **argv) {

	uint64_t retval;
	task_t *task;

	retval = func(argc, argv);

	intsoff();

		kdebug("La tarea finalizo\n");

	task = task_get_current();
	video_write_nl(task->console);

	free(task->name);
	free(task->stack);

	task->state = TASK_STOPPED;

	intson();

	reschedule();

}

task_t *task_create(task_entry_point func, const char* name, int argc, char** argv) {

	task_t *task = malloc(sizeof(task_t));
	context_t *context;

	task->state = TASK_PAUSED;
	task->pid = getnewpid();

	if (func != NULL) {
		task->console = task_get_current()->console;
	} else {
		task->console = KERNEL_CONSOLE;
	}

	task->name = malloc(strlen(name) + 1);
	memcpy(task->name, name, strlen(name) + 1);

	task->stack = malloc(STACK_SIZE);
	task->stack = task->stack + STACK_SIZE - sizeof(context_t);

	context = (context_t *) task->stack;

	context->gs = 0x001;
	context->fs = 0x002;
	context->r15 =	0x003;
	context->r14 =	0x004;
	context->r13 =	0x005;
	context->r12 =	0x006;
	context->r11 =	0x007;
	context->r10 =	0x008;
	context->r9 = 0x009;
	context->r8 = 0x00A;
	context->rsi =	(uint64_t)argc;
	context->rdi =	(uint64_t)func;
	context->rbp =	0x00D;
	context->rdx =	(uint64_t)argv;
	context->rcx =	0x00F;
	context->rbx =	0x010;
	context->rax =	0x011;
	context->rip =	(uint64_t)wrapper;
	context->cs = 0x008;
	context->rflags = 0x202;
	context->rsp =	(uint64_t) & (context->base);
	context->ss = 0x000;
	context->base =	0x000;

	task_add(task);

	kdebug("New task: '");
	_kdebug(name);
	_kdebug("' pid=");
	kdebug_base(task->pid, 10);
	_kdebug(" at 0x");
	kdebug_base((uint64_t) task, 16);
	kdebug_nl();

	//DUMP_LIST_FROM_CURRENT()

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
	reschedule();
}

void task_setconsole(task_t *task, console_t console) {
	task->console = console;
}

void task_next() {
	task_t *task = (current == NULL) ? last : current->next;

	//DUMP_LIST_FROM_CURRENT();

	while (task->state != TASK_RUNNING && task != current) {
		task = task->next;
	}

	if (task == current) {
		//kdebug("No hay tareas para ejecutar...\n");
		current = null_task;
		return;
	}

	current = task;

	kdebug("Next task is pid#: ");

	kdebug_base(current->pid, 10);
	kdebug_nl();

}

void task_set_foreground(task_t *task, console_t console) {
	foreground[console] = task;
}

task_t* task_get_foreground(console_t console) {
	return foreground[console];
}

task_t* task_get_current() {
	return current;
}

task_t* task_find_by_pid(pid_t pid) {

	task_t *task = last->next;

	while (task->pid != pid && task != last) {
		task = task->next;
	}

	if (task == last && task->pid != pid) {
		return NULL;
	}

	return task;
}
