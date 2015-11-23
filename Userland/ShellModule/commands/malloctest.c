#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(mtest, "[mtest] Prueba de malloc de tarea.");

COMMAND_START(mtest) {

	if (argc != 1) {
		fprintf(stderr, "Comando invalido.\n");
	} else {
		uint64_t* aux;
		while(1) {
			aux = sys_malloc(sizeof(uint64_t));
			*aux = 0xDEADBEEF;
		}
	}
	return 0;
}

