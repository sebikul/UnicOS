
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

void command_exit() {

	printf("\n");
	printf("Saliendo..");

}

void command_clean() {

	//idem colores, llamar a la funcion clear_screen de kernel

}
void command_refresh() {

	//idem idem idem idem idem, llama a refresh de kernel

}
