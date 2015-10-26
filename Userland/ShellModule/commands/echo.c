#include <libc.h>
#include "commands.h"

COMMAND_HELP(echo,"[echo] Permite mostrar un mensaje en pantalla.\nEj: echo Prueba. muestra Prueba en la pantalla.");

COMMAND_START(echo) {

	if (argc != 1) {
		for (int i = 1; i < argc; i++) {
			printf("%s ", argv[i]);
		}
	} else {
		printf("\n");
	}
}
