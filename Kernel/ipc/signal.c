#include <stdint.h>
#include "types.h"
#include "task.h"
#include "signal.h"
#include "kernel.h"

void signal_set(task_t *task, signal_t sig, sighandler_t handler) {
	task->sighandlers[sig] = handler;
}

static void sigwrapper(task_t* task, signal_t sig, task_state_t oldstate) {

	kdebug("Executing signal handler\n");

	if (task->sighandlers[sig] != NULL) {
		task->sighandlers[sig](sig);
	} else {
		video_write_line(video_current_console(), "Signal handler not set!");
	}

	task->state = oldstate;

	//Esperamos a que llegue la proxima interrupcion. Si llamamos a reschedule, se estaria pusheando
	//un contexto invalido.
	halt();
}

void signal_send(task_t *dest, signal_t sig) {

	context_t *context;

	dest->stack = dest->stack - sizeof(context_t) + sizeof(uint64_t); //Eliminamos context->base que no sirve para el contexto

	context = (context_t *) dest->stack;

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
	context->rsi =	(uint64_t)sig;
	context->rdi =	(uint64_t)dest;
	context->rbp =	0x00D;
	context->rdx =	(uint64_t)dest->state;
	context->rcx =	0x00F;
	context->rbx =	0x010;
	context->rax =	0x011;

	context->rip =	(uint64_t)sigwrapper;
	context->cs = 0x008;
	context->rflags = 0x202;
	context->rsp =	(uint64_t) & (context->base);
	context->ss = 0x000;

	task_ready(dest);
}

static void sigwrapper_witharg(task_t* task, task_state_t oldstate, dka_handler func, uint64_t arg) {

	kdebug("Executing signal handler\n");

	task_atomic(task);

	func(arg);

	task_unatomic(task);

	task->state = oldstate;

	//Esperamos a que llegue la proxima interrupcion. Si llamamos a reschedule, se estaria pusheando
	//un contexto invalido.
	halt();
}

void signal_func_witharg(task_t *dest, dka_handler func, uint64_t arg) {
	context_t *context;

	dest->stack = dest->stack - sizeof(context_t) + sizeof(uint64_t); //Eliminamos context->base que no sirve para el contexto

	context = (context_t *) dest->stack;

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
	context->rsi =	(uint64_t)dest->state;
	context->rdi =	(uint64_t)dest;
	context->rbp =	0x00D;
	context->rdx =	(uint64_t)func;
	context->rcx =	(uint64_t)arg;
	context->rbx =	0x010;
	context->rax =	0x011;

	context->rip =	(uint64_t)sigwrapper_witharg;
	context->cs = 0x008;
	context->rflags = 0x202;
	context->rsp =	(uint64_t) & (context->base);
	context->ss = 0x000;

	task_ready(dest);
}