
#include <stdint.h>
#include "task.h"
#include "string.h"
#include "kernel.h"

#define MAX_TASKS 1024

static task_t *first = NULL;
static task_t *current = NULL;

static pid_t nextpid = 1;

static void* const shellCodeModuleAddress = (void*)0x400000;
static void* const shellDataModuleAddress = (void*)0x500000;

extern uintptr_t kernel_stack;

static pid_t getnewpid() {
	pid_t pid = nextpid;
	nextpid++;

	return pid;
}

static uint64_t task_shell() {

	while (TRUE);// {

		kdebug("Running shell pid#: ");

		kdebug_base(current->pid, 10);
		kdebug_nl();

		uint8_t console = task_get_current()->console;

		//video_clear_screen(console);

		video_write_string(console, "Console #: ");
		video_write_dec(console, console);
		video_write_nl(console);

		kdebug("Ejecutando shell en 0x");
		kdebug_base(shellCodeModuleAddress, 16);
		kdebug_nl();


		//intson();
		//WHY????
		//intson();
		//TODO Ver si se deberia copiar el codigo por cada task
		//((task_entry_point)shellCodeModuleAddress)(argc, argv);
		while(TRUE); //Simulamos la consola
	//}

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
	context_t *context;
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
	stack = stack + STACK_SIZE-sizeof(context_t)-1;
	task->stack = stack;

	context = (context_t *) stack;

	context->gs =		0x001;
	context->fs =		0x002;
	context->r15 =	0x003;
	context->r14 =	0x004;
	context->r13 =	0x005;
	context->r12 =	0x006;
	context->r11 =	0x007;
	context->r10 =	0x008;
	context->r9 =		0x009;
	context->r8 =		0x00A;
	context->rsi =	0x00B;
	context->rdi =	0x00C;
	context->rbp =	0x00D;
	context->rdx =	0x00E;
	context->rcx =	0x00F;
	context->rbx =	0x010;
	context->rax =	0x011;
	context->rip =	(uint64_t)task_shell;
	context->cs =		0x008;
	context->rflags = 0x202;
	context->rsp =	(uint64_t)&(context->base);
	context->ss = 	0x000;
	context->base =	0x000;

	task_add(task);

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

uintptr_t task_save_stack(uintptr_t stack){
	current->stack = stack;

	return kernel_stack;
}

uintptr_t task_restore_stack(){
	return current->stack;
}

task_t* task_next() {
	task_t *task = current->next;
	task_t *first = current;

	while (task->state != TASK_RUNNING && task != current) {
		task = task->next;
	}

	if(task==current){
		kdebug("No hay tareas para ejecutar...\n");
		//TODO Retornar null task
	}

	current = task;
}

task_t* task_get_current() {
	return current;
}
