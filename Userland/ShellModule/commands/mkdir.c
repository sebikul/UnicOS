#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(mkdir, "[ls] <path>: Crea un nuevo directorio.\n");

COMMAND_START(mkdir) {

	if (argc != 2) {
		fprintf(stderr, "Comando invalido. Debe ingresar un path.\n");
		return 1;
	} else {
		sys_atomic();
		int val = sys_mkdir(argv[1]);
		sys_unatomic();
		return val;
	}

	return 0;
}
