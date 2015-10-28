#include <libc.h>
#include "string.h"
#include "commands.h"

extern command_list_t *cmdlist;

COMMAND_HELP(help, "[help] Muestra en pantalla la lista de comandos disponibles.\n"
             "Si se ejecuta help [comando] muestra el funcionamiento del comando elegido.\n");

COMMAND_START(help) {

	if (argc == 1) {

		printf("Comandos disponibles:\n");
		printf("\thelp \t\t: Muestra la lista de los comandos disponibles.\n");
		printf("\techo \t\t: Muestra un mensaje en pantalla.\n");
		printf("\ttime \t\t: Muestra o setea la hora del sistema en pantalla.\n");
		printf("\tcolor\t\t: Cambia el color de lo que se va a imprimir en pantalla.\n");
		printf("\tkeyboard \t: Setea la distribucion del teclado.\n");
		printf("\tclear\t\t: Limpia la pantalla.\n");
		printf("\trefresh  \t: Restaura los colores por defecto.\n");
		printf("\tscreensaver  : Activa o configura el salvapantallas.\n");
		printf("\tuser \t\t: Modifica el nombre de usuario.\n");
		printf("\thost \t\t: Modifica el nombre del host.\n");
		printf("\texit \t\t: Finaliza el programa.\n");

		printf("\nEjecute 'help [comando]' para obtener mas ayuda.\n");
	} else {

		for (int cmd = 0; cmd < cmdlist->count; cmd++) {
			if (strcmp(cmdlist->commands[cmd]->name, argv[0]) == 0) {
				printf("\t%s\n", cmdlist->commands[cmd]->help);
			}
		}

	}
}
