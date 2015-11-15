#include <libc.h>
#include "string.h"
#include "commands.h"

extern command_list_t *cmdlist;

COMMAND_HELP(help, "[help] Muestra en pantalla la lista de comandos disponibles.\n"
             "Si se ejecuta help [comando] muestra el funcionamiento del comando elegido.\n");

COMMAND_START(help) {

	if (argc == 1) {

		for (int cmd = 0; cmd < cmdlist->count; cmd++) {
			printf("\t%s\n", cmdlist->commands[cmd]->help);
		}

		printf("\nEjecute 'help [comando]' para obtener mas ayuda.\n");
	} else {

		for (int cmd = 0; cmd < cmdlist->count; cmd++) {
			if (strcmp(cmdlist->commands[cmd]->name, argv[1]) == 0) {
				printf("\t%s\n", cmdlist->commands[cmd]->help);
				break;
			}
		}

	}

	return 0;
}
