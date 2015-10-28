#include <libc.h>
#include "types.h"
#include "syscalls.h"
#include "commands.h"

COMMAND_HELP(ps, "[ps] Lista las tareas del sistema.\n");

static char* state_to_string(task_state_t state) {
	switch (state) {
	case TASK_STOPPED:
		return "STOPPED";

	case TASK_SLEEPING:
		return "SLEEPING";

	case TASK_RUNNING:
		return "RUNNING";

	case TASK_PAUSED:
		return "PAUSED";

	case TASK_JOINING:
		return "JOINING";

	default:
		return "UNKNOWN";
	}
}

COMMAND_START(ps) {

	task_t *first = sys_task_getall();
	task_t *task = first;

	ksysdebug("Imprimiendo tareas.\n");

	do {
		printf("Name: %s\tpid=%d\tstate=%s\n", task->name, task->pid, state_to_string(task->state));

		task = task->next;
	} while (task != first);

	ksysdebug("Terminamos de imprimir tareas.\n");

}

