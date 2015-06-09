#include <libc.h>
#include <syscalls.h>


void command_screensaver(int argc, char** argv) {

	switch (argc) {
	case 1:

		sys_screensaver_trigger();
		break;

	case 3:
		if (strcmp(argv[1], "set") == 0) {

			int sec;

			if (!string_numeric(argv[2])) {

				printf("Debe ingresar una cantidad de segundos como parametro.\n");
				return;
			}
			sec = ctoi(argv[2]);
			if(sec==0){
				printf("Salva pantallas desactivado.\n");
			}

			sys_set_screensaver_timer(sec);

		} else {
			printf("Comando invalido. Comandos disponibles [screensaver set <segundos>]\n");
		}

		break;

	default:
		printf("Cantidad invalida de parametros.\n");
	}


}