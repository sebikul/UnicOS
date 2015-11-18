#include <libc.h>
#include <types.h>
#include <syscalls.h>
#include "commands.h"
#include "string.h"

COMMAND_HELP(set_distribution, "[keyboard set] Recibe una distribucion de teclado [usa | latin] y la setea.");

COMMAND_START(set_distribution) {

	if (argc != 3) {
		fprintf(stderr, "Cantidad invalida de argumentos.\n");
		return 1;
	}

	if (strcmp(argv[1], "set") == 0) {

		if (strcmp(argv[2], "latin") == 0) {
			sys_kbd_set_distribution(KEYBOARD_LATIN);
		} else if (strcmp(argv[2], "usa") == 0) {
			sys_kbd_set_distribution(KEYBOARD_USA);
		} else {
			fprintf(stderr, "Distribucion de teclado invalida. Distribuciones disponibles [usa] [latin].\n");
		}

	} else {
		fprintf(stderr, "Comando invalido. Comando disponible [keyboard set].\n");
	}

	return 0;

}
