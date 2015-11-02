#include <libc.h>
#include "types.h"
#include "syscalls.h"
#include "commands.h"

COMMAND_HELP(ps, "[ps] Lista las tareas del sistema.\n");

static bool _exit = 0;

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

static void keyboard_handler(uint64_t s) {

	printf("ESC presionado. Saliendo...\n");
	_exit = TRUE;

}

COMMAND_START(ps) {

	int index = sys_keyboard_catch(0x01, keyboard_handler, 0, "esc key");

	_exit = 0;

	while (!_exit) {
		task_t *first = sys_task_getall();
		task_t *task = first;

		sys_clear_screen();

		//ksysdebug("Imprimiendo tareas.\n");

		do {
			printf("Name: %s\tpid=%d\tstate=%s\n", task->name, task->pid, state_to_string(task->state));

			task = task->next;
		} while (task != first);

		sys_sleep(1000);
	}

	ksysdebug("Terminamos de imprimir tareas.\n");

	sys_keyboard_clear_handler(index);

}

