
#include <libc.h>


void command_help(int argc, char** argv, char** cmd_list, int cmd_count) {

	if (argc == 1) {

		printf("Comandos disponibles:\n");
		printf("\thelp  : Muestra una lista de los comandos disponibles.\n");
		printf("\techo  : Muestra un mensaje en pantalla.\n");
		printf("\ttime  : Muestra o setea la hora del sistema en pantalla.\n");
		printf("\tcolor : Cambia el color de lo que se va a imprimir en pantalla.\n");
		printf("\keyboard : Setea la distribucion del teclado.\n");
		printf("\tclear : Limpia la pantalla.\n");
		printf("\texit  : Finaliza el programa.\n");

		printf("\nEjecute 'help [comando]' para obtener mas ayuda.\n");
	} else {

		int cmd = 0;
		for (; cmd < cmd_count; cmd++) {
			if (strcmp(argv[1], cmd_list[cmd]) == 0) {
				break;
			}
		}

		switch (cmd) {

		case 0: //echo
			printf("\tEl comando echo permite mostrar un mensaje en pantalla.\n");
			printf(" Ej: echo Prueba. muestra Prueba en la pantalla.\n");
			break;

		case 1: //help

			printf("\t[help] Muestra en pantalla la lista de comandos disponibles.\n");
			printf("Si se ejecuta help [comando] muestra el funcionamiento del comando elegido.\n");
			break;

		case 2: //time

			printf("\t[time] Muestra la hora en pantalla.\n");
			printf("\t[time set] Permite setear la hora.\n");
			printf(" Ej: time set 21 10 5 setea la hora a las 21:10:05.\n");
			break;

		case 3: //color

			break;

		case 4: //exit

			printf("\tEl comando exit finaliza la ejecucion de la terminal.\n");
			break;

		case 5: //clear

			printf("\tEl comando clear limpia la pantalla (no limpia el historial de comandos).\n");
			break;
		// //other functions....

		default:

			printf("\nNo se encontro el comando.");
		}
	}


}