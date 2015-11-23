#include <stdint.h>
#include "task.h"
#include "kernel.h"
#include "paging.h"

extern void* kernel_stack;

void* scheduler_u2k(void* rsp) {
	task_t *current = task_get_current();

  if (current != NULL) {
    // kdebug("Saving stack at 0x");
    // kdebug_base((uint64_t) rsp, 16);
    // kdebug_nl();
    current->stack = rsp;
    current->malloc_current = get_process_malloc();
    //writeCR3(get_kernel_cr3());
  }

  return kernel_stack;
}

void* scheduler_k2u() {
	task_t *task = task_get_current();
	task->quantum = QUANTUM;
  writeCR3(task->cr3);

  // kdebug("Restoring stack at 0x");
  // kdebug_base((uint64_t) task->stack, 16);
  // kdebug_nl();
	return task->stack;
}
