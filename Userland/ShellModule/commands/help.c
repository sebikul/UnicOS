
#include <libc.h>


void command_help(int argc, char** argv) {

	printf("Comandos disponibles:\n");
	printf("\thelp  : Muestra una lista de los comandos disponibles.\n");
	printf("\techo  : Muestra un mensaje en pantalla.\n");
	printf("\ttime  : Muestra o setea la hora del sistema en pantalla.\n");
	//printf("3. Set time: Recibe la hora actual (horas, minutos, segundos) y actualiza la hora del sistema.\n");
	printf("\tcolor : Cambia el color de lo que se va a imprimir en pantalla.\n");
	//printf("5. Fontcolor: Recibe un color y cambia el color de letra.\n");
	//printf("6. Refresh: Restaura la pantalla con sus colores por default.\n");
	printf("\tclear : Limpia la pantalla.\n");
	printf("\texit  : Finaliza el programa.\n");

	printf("\nEjecute 'help [comando]' para obtener mas ayuda.\n");

}