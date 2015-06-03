
#include <libc.h>

static void command_echo_usage();

//DEBERIAMOS PONERLE COMANDOS.C O ALGO ASI, NO LE MODIFIQUE EL NOMBRE POR SI ALGUNO ESTABA TOCANDO EL ARCHIVO

void command_echo(int argc, char** argv) {

	if (argc == 1) {
		command_echo_usage();
	} else {
		for (int i = 1; i < argc; i++) {
			printf("%s ", argv[i]);
		}
	}



}

static void command_echo_usage() {

	printf("uso: echo <cadena que se desea imprimir>\n");

}

//void command_backcolor(int color) {

//	printf("\n");
	/*if(color invalido){
		return;
		}
	*/

	//CREO -HABRIA QUE PREGUNTAR- que tendriamos que definir nuestras propias syscalls
	//para poder llamar a las funciones de kernel que permiten cambiar colores

	//por ejemplo nose la interrupcion 55 recibe un parametro que es el numero de color y llama a la funcion set_bkg_color
//	printf("Color cambiado");

//}
//void command_fontcolor(int color) {

	//printf("\n");
	/*if(color invalido){
		return;
		}
	*/

	//idem fc anterior
//	printf("Color cambiado");

//}
void command_exit() {

	printf("\n");
	printf("Saliendo..");

}

void command_restart() {

	//resetea los colores por default de la pantalla

}


void command_clean() {

	//idem colores, llamar a la funcion clear_screen de kernel

}
void command_refresh() {

	//idem idem idem idem idem, llama a refresh de kernel

}
