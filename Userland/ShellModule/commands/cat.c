#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(cat, "[cat] <path> Imprime el contenido del archivo.\n");

COMMAND_START(cat) {

	if (argc != 2) {
		fprintf(stderr, "Ingrese un archivo para imprimir.\n");
		return 1;
	}

	int fd = sys_open(argv[1], O_RDONLY);
	uint32_t size = sys_size(fd);

	printf("Tamano del archivo: %d\nfd: %d\n", size, fd);

	char* data = malloc(size + 1);

	sys_read(fd, data, size);
	data[size] = 0;

	printf("%s\n", data);

	return 0;
}
