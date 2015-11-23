#include <stdint.h>
#include "types.h"
#include "task.h"
#include "signal.h"
#include "kernel.h"

static context_t* sig_aux;
static uint64_t sig_cr3;

void signal_set(task_t *task, signal_t sig, sighandler_t handler) {

	if (sig == SIGKILL) {
		task_errno(EINV_OP);
		return;
	}

	task->sighandlers[sig] = handler;
}

static void sigwrapper(task_t* task, signal_t sig, task_state_t oldstate) {

	kdebug("Executing signal handler\n");

	if (task->sighandlers[sig] != NULL) {

		task_atomic(task);
		task->sighandlers[sig](sig);
		task_unatomic(task);

	} else {
		video_write_line(video_current_console(), "Signal handler not set!");
	}

	task->state = oldstate;

	//Esperamos a que llegue la proxima interrupcion. Si llamamos a reschedule, se estaria pusheando
	//un contexto invalido.
	halt();
}

static void signal_dispatch(task_t *dest, signal_t sig) {
	sig_aux = (context_t*)malloc(sizeof(context_t));
	sig_aux->gs = 0x001;
	sig_aux->fs = 0x002;
	sig_aux->r15 =	0x003;
	sig_aux->r14 =	0x004;
	sig_aux->r13 =	0x005;
	sig_aux->r12 =	0x006;
	sig_aux->r11 =	0x007;
	sig_aux->r10 =	0x008;
	sig_aux->r9 = 0x009;
	sig_aux->r8 = 0x00A;
	sig_aux->rsi =	(uint64_t)sig;
	sig_aux->rdi =	(uint64_t)dest;
	sig_aux->rbp =	0x00D;
	sig_aux->rdx =	(uint64_t)dest->state;
	sig_aux->rcx =	0x00F;
	sig_aux->rbx =	0x010;
	sig_aux->rax =	0x011;
	sig_aux->rip =	(uint64_t)sigwrapper;
	sig_aux->cs = 0x008;
	sig_aux->rflags = 0x202;
	sig_aux->rsp =	(uint64_t) & (sig_aux->base);
	sig_aux->ss = 0x000;

	kdebug("1\n");
	sig_cr3 = readCR3();
	kdebug("2\n");

	switch_u2k();
	kdebug("3\n");

	writeCR3(dest->cr3);
	kdebug("4\n");

	memcpy((char*)(dest->stack - sizeof(context_t) + sizeof(uint64_t)), (char*)sig_aux, sizeof(context_t) - sizeof(uint64_t));
	kdebug("5\n");
	dest->stack = dest->stack - sizeof(context_t) + sizeof(uint64_t); //Eliminamos context->base que no sirve para el contexto
	kdebug("6\n");

	writeCR3(sig_cr3);
	kdebug("7\n");
	switch_k2u();


	task_ready(dest);
}

void signal_send(task_t *dest, signal_t sig) {

	switch (sig) {
	case SIGKILL:
		task_schedule_removal(dest);
		break;

	case SIGINT:
		signal_dispatch(dest, sig);
		task_schedule_removal(dest);
		break;

	default:
		signal_dispatch(dest, sig);
	}

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