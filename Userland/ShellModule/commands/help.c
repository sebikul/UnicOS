
#include <libc.h>


void command_help(int argc, char** argv, char** cmd_list, int cmd_count) {

	if (argc == 1) {

		printf("Comandos disponibles:\n");
		printf("\thelp  : Muestra una lista de los comandos disponibles.\n");
		printf("\techo  : Muestra un mensaje en pantalla.\n");
		printf("\ttime  : Muestra o setea la hora del sistema en pantalla.\n");
		printf("\tcolor : Cambia el color de lo que se va a imprimir en pantalla.\n");
		printf("\tkeyboard : Setea la distribucion del teclado.\n");
		printf("\tclear : Limpia la pantalla.\n");
		printf("\trestart : Restaura los colores por defecto.\n");
		printf("\texit  : Finaliza el programa.\n");

		printf("\nEjecute 'help [comando]' para obtener mas ayuda.\n");
	} else {

		int cmd = 0;
		for (; cmd < cmd_count; cmd++) {
			if (strcmp(argv[1], cmd_list[cmd]) == 0) {
				break;
			}
		}

		printf("\n");
		switch (cmd) {

		case 0: //echo
			printf("\t[echo] Permite mostrar un mensaje en pantalla.\n");
			printf(" Ej: echo Prueba. muestra Prueba en la pantalla.\n");
			break;

		case 1: //help

			printf("\t[help] Muestra en pantalla la lista de comandos disponibles.\n");
			printf("Si se ejecuta help [comando] muestra el funcionamiento del comando elegido.\n");
			break;

		case 2: //time
			printf("\t[time] Muestra la hora en pantalla.\n");
			printf("\t[time set] Permite setear fecha y hora del sistema.\n");
			printf(" Ej: time set 21 10 5 20 3 1993 setea la hora a las 21:10:05 del 20/03/1993.\n");
			break;

		case 3: //color
			printf("\t[color] Muestra en pantalla el color actual de texto y de fondo.\n");
			printf("\t[color set front] Recibe un color y lo setea como color de texto.\n");
			printf("\t[color set background] Recibe un color y lo setea como color de fondo.\n");
			printf("Lista de colores disponibles:\n");
			printf("\tBlack, Blue, Green, Cyan, Red, Magenta, Brown, Light Grey, Dark Grey, White\n\tLight Blue, Light Green, Light Cyan, Light Red, Light Magenta, Light Brown.\n");
			break;

		case 4: //keyboard
			printf("\t[keyboard set] Recibe una distribucion de teclado [usa | latin] y la setea.\n");
			break;

		case 5: //exit
			printf("\t[exit] Finaliza la ejecucion de la terminal.\n");
			break;

		case 6: //clear
			printf("\t[clear] Limpia la pantalla (no limpia el historial de comandos).\n");
			break;
		// //other functions....

		case 7: //restart

			printf("\t");
			break;

		default:

			printf("\nNo se encontro el comando.");
		}
	}


}