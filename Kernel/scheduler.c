
#include <stdint.h>
#include "task.h"
#include "kernel.h"

extern void* kernel_stack;

void* scheduler_u2k(void* rsp) {
	task_t *current = task_get_current();

	if (current != NULL) {
		current->stack = rsp;
	}

	return kernel_stack;
}

void* scheduler_k2u() {
	task_t *task = task_get_current();
	task->quantum = QUANTUM;

	return task->stack;
}
