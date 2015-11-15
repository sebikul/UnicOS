#include <libc.h>
#include "string.h"
#include "commands.h"

extern char* user_name;
extern char* host_name;
extern int USER_SIZE;
extern int HOST_SIZE;

COMMAND_HELP(user_name, "[user] Recibe el nombre de usuario y lo setea.");

COMMAND_START(user_name) {

	if (argc != 2) {
		fprintf(stderr, "Comando invalido. Debe ingresar un unico nombre de usuario.\n");
	} else {
		if (strlen(argv[1]) > USER_SIZE) {
			printf("El nombre de usuario es demasiado largo, el tamaño maximo es: %d", USER_SIZE);
		} else {
			strcpy(user_name, argv[1]);
		}
	}

	return 0;
}

COMMAND_HELP(host_name, "[host] Recibe el nombre del host y lo setea.");

COMMAND_START(host_name) {

	if (argc != 2) {
		fprintf(stderr, "Comando invalido. Debe ingresar un unico nombre de host.\n");
	} else {
		if (strlen(argv[1]) > HOST_SIZE) {
			printf("El nombre del host es demasiado largo, el tamaño maximo es: %d", HOST_SIZE);
		} else {
			strcpy(host_name, argv[1]);
		}
	}

	return 0;

}
