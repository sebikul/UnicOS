
#include <libc.h>

//DEBERIAMOS PONERLE COMANDOS.C O ALGO ASI, NO LE MODIFIQUE EL NOMBRE POR SI ALGUNO ESTABA TOCANDO EL ARCHIVO

void command_echo(char * in) {

	printf("\n");
	printf(in);

}

void command_help() {

	printf("\n");
	printf("Help:\n");
	printf("Comandos disponibles:\n");
	printf("1. Help: Brinda informacion acerca de las funciones disponibles para el usuario.\n");
	printf("2. Time: Muestra la hora del sistema en pantalla.\n");
	printf("3. Set time: Recibe la hora actual (horas, minutos, segundos) y actualiza la hora del sistema.\n");
	printf("4. Backcolor: Recibe un color y cambia el color de fondo.\n");
	printf("5. Fontcolor: Recibe un color y cambia el color de letra.\n");
	printf("6. Refresh: Restaura la pantalla con sus colores por default.\n");
	printf("7. Clean: Limpia la pantalla.");
	printf("8. Exit: Finaliza el programa.\n");

}
void command_time() {

	printf("\n");
	printf("Mostrando la hora");

}
void command_settime(int hours, int minutes, int seconds) {

	printf("\n");
	if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59) {
		printf("Horario invalido.");
	}
	printf("Seteando la hora");
}
void command_backcolor(int color) {

	printf("\n");
	/*if(color invalido){
		return;
		}
	*/

	//CREO -HABRIA QUE PREGUNTAR- que tendriamos que definir nuestras propias syscalls
	//para poder llamar a las funciones de kernel que permiten cambiar colores

	//por ejemplo nose la interrupcion 55 recibe un parametro que es el numero de color y llama a la funcion set_bkg_color
	printf("Color cambiado");

}
void command_fontcolor(int color) {

	printf("\n");
	/*if(color invalido){
		return;
		}
	*/

	//idem fc anterior
	printf("Color cambiado");

}
void command_exit() {

	printf("\n");
	printf("Saliendo..");

}

void command_restart() {


}


void command_clean() {

	//idem colores, llamar a la funcion clear_screen de kernel

}
void command_refresh() {

	//idem idem idem idem idem, llama a refresh de kernel

}