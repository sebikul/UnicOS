#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(filesend, "[filesend] <pid> <path> <msg>:  Envia datos a otro proceso mediante archivos");

COMMAND_START(filesend) {

	if (argc != 4) {
		fprintf(stderr, "Comando invalido. Debe ingresar un pid.\n");
	} else {
		pid_t pid = ctoi(argv[1]);

		int fd = sys_open(argv[2], O_CREAT | O_WRONLY | O_TRUNC);

		sys_write(fd, argv[3], strlen(argv[3]) + 1);

		printf("Sending signal to process: %ld\n", pid);

		sys_signal_kill(pid, SIGUSR1);
	}

	return 0;
}

COMMAND_HELP(filercv, "[filercv] <path>: Recibe una senal");

COMMAND_START(filercv) {

	int32_t index;
	bool _exit = FALSE;

	void sighandler(signal_t s) {
		ksysdebug("Senal recibida!\n");
		printf("Senal recibida!\n");

		int fd = sys_open(argv[1], O_RDONLY);

		int size = sys_size(fd);

		char* buf = malloc(size + 1);

		sys_read(fd, buf, size);

		printf("Leido: %s\n", buf);
	}

	void keyboard_handler(uint64_t s) {
		if (s == 0x01) {
			printf("ESC presionado. Saliendo...\n");
			_exit = TRUE;
		}
	}

	if (argc != 2) {
		fprintf(stderr, "Comando invalido. Debe ingresar un archivo.\n");
		return 1;
	}

	ksysdebug("Seteando handler de señal\n");
	printf("PID: %ld\n", sys_task_get_pid());

	sys_signal_set(SIGUSR1, sighandler);

	printf("Installing keyboard handler...\n");
	index = sys_keyboard_catch(0x00, keyboard_handler, KEYBOARD_WILDCARD, "esc key");

	while (!_exit);

	printf("Exiting...\n");

	sys_keyboard_clear_handler(index);

	return 0;
}
