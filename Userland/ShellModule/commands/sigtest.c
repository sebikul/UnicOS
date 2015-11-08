#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(sigsend, "[sigsend] Envia una señal al programa con pid <pid>");

COMMAND_START(sigsend) {

	if (argc != 2) {
		fprintf(FD_STDERR, "Comando invalido. Debe ingresar un pid.\n");
	} else {
		pid_t pid = ctoi(argv[1]);

		sys_signal_kill(pid, SIGKILL);
	}

	return 0;
}

COMMAND_HELP(sigrcv, "[host] Recibe una señal");

COMMAND_START(sigrcv) {

	int index;
	bool _exit = FALSE;

	void sighandler(signal_t s) {
		ksysdebug("Senal recibida!\n");
		printf("Senal recibida!\n");
		_exit = TRUE;
	}

	void keyboard_handler(uint64_t s) {
		if (s == 0x01) {
			printf("ESC presionado. Saliendo...\n");
			_exit = TRUE;
		}
	}

	ksysdebug("Seteando handler de señales\n");
	printf("PID: %ld\n", sys_task_get_pid());

	sys_signal_set(SIGKILL, sighandler);

	printf("Installing keyboard handler...\n");
	index = sys_keyboard_catch(0x00, keyboard_handler, KEYBOARD_WILDCARD, "esc key");

	while (!_exit);

	printf("Exiting...\n");

	sys_keyboard_clear_handler(index);

	return 0;
}
