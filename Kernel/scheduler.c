
#include <stdint.h>
#include "task.h"

extern uintptr_t kernel_stack;

uintptr_t scheduler_u2k(uintptr_t rsp) {
	task_t *current = task_get_current();
	
	if(current!=NULL){
		current->stack = rsp;	
	}

	return kernel_stack;
}

uintptr_t scheduler_k2u() {
	task_t *task;
	task_next();
	task = task_get_current();

	return task->stack;
}