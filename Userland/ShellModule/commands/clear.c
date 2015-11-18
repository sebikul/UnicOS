#include <libc.h>
#include "commands.h"

COMMAND_HELP(clear, "[clear] Limpia la pantalla (no limpia el historial de comandos).\n");

COMMAND_START(clear) {

	if (argc == 1) {
		clear_screen();
	} else {
		fprintf(stderr, "Comando invalido, no se esperan parametros.\n");
	}

	return 0;
}

