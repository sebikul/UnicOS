#include <stdint.h>
#include "types.h"
#include "task.h"
#include "signal.h"
#include "kernel.h"

void signal_set(task_t *task, signal_t sig, sighandler_t handler) {
	task->sighandlers[sig] = handler;
}

static void sigwrapper(task_t* task, signal_t sig, task_state_t oldstate, void* origstack) {

	kdebug("Executing signal handler\n");

	task_atomic(task);

	task->sighandlers[sig](sig);

	kdebug("Returned from signal handler! Restoring stack to 0x");
	kdebug_base(origstack, 16);
	kdebug_nl();

	task->state = oldstate;
	task->stack = origstack;

	task_unatomic(task);
	halt();
}

void signal_send(task_t *dest, signal_t sig) {

	context_t *context;
	void* origstack = dest->stack;

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
	context->rcx =	origstack;
	context->rbx =	0x010;
	context->rax =	0x011;

	context->rip =	(uint64_t)sigwrapper;
	context->cs = 0x008;
	context->rflags = 0x202;
	context->rsp =	(uint64_t) & (context->base);
	context->ss = 0x000;

	task_ready(dest);

}