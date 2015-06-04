#include <libc.h>
#include <types.h>
#include <syscalls.h>

void command_set_distribution(int argc , char** argv) {
	if (argc != 3) {
		printf("Cantidad invalida de argumentos.\n");
		return;
	}

	if (strcmp(argv[1], "set") == 0) {

		if (strcmp(argv[2], "latin") == 0) {
			sys_kbd_set_distribution(KEYBOARD_LATIN);
		} else if (strcmp(argv[2], "usa") == 0) {
			sys_kbd_set_distribution(KEYBOARD_USA);
		} else {
			printf("Distribucion de teclado invalida. Distribuciones disponibles [usa] [latin].\n");
		}

	} else {
		printf("Comando invalido. Comando disponible [keyboard set].\n");
	}
}
