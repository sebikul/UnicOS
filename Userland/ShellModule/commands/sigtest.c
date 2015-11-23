#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(sigsend, "[sigsend] <pid> <signal>:  Envia una senal al programa con pid <pid>");

COMMAND_START(sigsend) {

	if (argc != 3) {
		fprintf(stderr, "Comando invalido. Debe ingresar un pid.\n");
		return -1;
	} else {
		pid_t pid = ctoi(argv[1]);
		signal_t sig;

		if (strcmp(argv[2], "SIGUSR1") == 0) {
			sig = SIGUSR1;
		} else if (strcmp(argv[2], "SIGUSR2") == 0) {
			sig = SIGUSR2;
		} else if (strcmp(argv[2], "SIGINT") == 0) {
			sig = SIGINT;
		} else if (strcmp(argv[2], "SIGKILL") == 0) {
			sig = SIGKILL;
		} else {
			fprintf(stderr, "Senal invalida.\n");
		}

		sys_signal_kill(pid, sig);
	}

	return 0;
}

COMMAND_HELP(sigrcv, "[sigrcv] <signal>: Recibe una senal");

COMMAND_START(sigrcv) {

	int32_t index;
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

	if (argc != 2) {
		fprintf(stderr, "Comando invalido. Debe ingresar una senal.\n");
		return 1;
	}

	ksysdebug("Seteando handler de señales\n");
	printf("PID: %ld\n", sys_task_get_pid());

	signal_t sig;

	if (strcmp(argv[1], "SIGUSR1") == 0) {
		sig = SIGUSR1;
	} else if (strcmp(argv[1], "SIGUSR2") == 0) {
		sig = SIGUSR2;
	} else if (strcmp(argv[1], "SIGINT") == 0) {
		sig = SIGINT;
	} else if (strcmp(argv[1], "SIGKILL") == 0) {
		sig = SIGKILL;
	} else {
		fprintf(stderr, "Senal invalida.\n");
	}

	sys_signal_set(sig, sighandler);

	printf("Installing keyboard handler...\n");
	index = sys_keyboard_catch(0x00, keyboard_handler, KEYBOARD_WILDCARD, "esc key");

	while (!_exit);

	printf("Exiting...\n");

	sys_keyboard_clear_handler(index);

	return 0;
}
