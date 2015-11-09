#include "types.h"
#include "task.h"

#ifndef SIGNAL_H
#define SIGNAL_H

void signal_set(task_t *task, signal_t sig, sighandler_t handler);
void signal_send(task_t *dest, signal_t sig);

void signal_func_witharg(task_t *dest, dka_handler func, uint64_t arg);


#endif