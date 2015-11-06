#include <libc.h>
#include <syscalls.h>
#include "commands.h"
#include "string.h"

COMMAND_HELP(screensaver, 			"[screensaver] Activa el salva pantallas.\n"
             "\t[screensaver set] Setea el tiempo que tarda en activarse el salva pantallas."
             "Por defecto 20 segundos, el tiempo se ingresa en segundos.");

COMMAND_START(screensaver) {

	switch (argc) {
	case 1:

		sys_screensaver_trigger();
		//TODO Esperar a que se despierte del screensaver para devolver. Sino la shell, al escribir en la consola vuelve a despertar al sistema
		break;

	case 3:
		if (strcmp(argv[1], "set") == 0) {

			int sec;

			if (!string_numeric(argv[2])) {

				fprintf(FD_STDERR, "Debe ingresar una cantidad de segundos como parametro.\n");
				return 1;
			}
			sec = ctoi(argv[2]);
			if (sec == 0) {
				printf("Salva pantallas desactivado.\n");
			}

			sys_set_screensaver_timer(sec);

		} else {
			fprintf(FD_STDERR, "Comando invalido. Comandos disponibles [screensaver set <segundos>]\n");
		}

		break;

	default:
		fprintf(FD_STDERR, "Cantidad invalida de parametros.\n");
	}

	return 0;
}
