#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(edit, "[edit] <!opt:append|create> <path> <data>: Crea un nuevo archivo.\n");

COMMAND_START(edit) {

	if (argc != 3 && argc != 4) {
		fprintf(stderr, "Comando invalido. Debe ingresar un path.\n");
		return 1;
	}

	bool opt = (argc == 4);
	uint64_t flags = O_WRONLY;

	int pindex = 1;
	int dataindex = 2;

	if (opt) {
		pindex++;
		dataindex++;

		if (strcmp(argv[1], "append") == 0) {

			flags |= O_APPEND;
			printf("Appending to file...\n");

		} else if (strcmp(argv[1], "create") == 0) {

			flags |= O_CREAT;
			printf("Cerating file...\n");

		} else {
			fprintf(stderr, "Comando invalido.\n");
			return 1;
		}
	}

	int fd = sys_open(argv[pindex], flags);

	if (fd < 0) {
		fprintf(stderr, "Error: %d\n", fd);
		return 1;
	}

	char* data = argv[dataindex];

	int size = sys_write(fd, data, strlen(data));

	printf("Wrote %d bytes to %s\n", size, argv[pindex]);

	return 0;
}
