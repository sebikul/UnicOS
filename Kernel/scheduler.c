
#include <stdint.h>
#include "task.h"

extern void* kernel_stack;

void* scheduler_u2k(void* rsp) {
	task_t *current = task_get_current();
	
	if(current!=NULL){
		current->stack = rsp;	
	}

	return kernel_stack;
}

void* scheduler_k2u() {
	task_t *task;
	task_next();
	task = task_get_current();

	return task->stack;
}