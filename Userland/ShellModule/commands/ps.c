#include <libc.h>
#include "types.h"
#include "syscalls.h"
#include "commands.h"

COMMAND_HELP(ps, "[ps] Lista las tareas del sistema.\n");

static char* state_to_string(task_state_t state) {
	switch (state) {
	case TASK_ZOMBIE:
		return "ZOMBIE";

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

	bool _exit = 0;

	void keyboard_handler(uint64_t s) {
		if (s == 0x01) {
			printf("ESC presionado. Saliendo...\n");
			_exit = TRUE;
		}
	}

	int index = sys_keyboard_catch(0x00, keyboard_handler, KEYBOARD_WILDCARD, "esc key");

	_exit = FALSE;

	while (!_exit) {
		task_t *first = sys_task_getall();
		task_t *task = first;

		sys_clear_screen();

		//ksysdebug("Imprimiendo tareas.\n");

		printf("\t\t\t%20s%s\t %21s\n", "NAME", "PID", "STATE");


		do {
			printf("Name:\t%20s pid=%d\tstate=%21s\n", task->name, task->pid, state_to_string(task->state));

			task = task->next;
		} while (task != first);

		uint64_t uptime = sys_uptime();

		printf("\nUPTIME: %d.%ds\n", uptime / 1000, uptime % 1000);

		sys_sleep(1000);
	}

	ksysdebug("Terminamos de imprimir tareas.\n");

	sys_keyboard_clear_handler(index);

	return 0;
}

