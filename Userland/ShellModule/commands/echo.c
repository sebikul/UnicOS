
#include <libc.h>

void command_echo(int argc, char** argv) {

	if (argc != 1) {
		for (int i = 1; i < argc; i++) {
			printf("%s ", argv[i]);
		}
	} else {
		printf("\n");
	}

}

//TODO: programar y sacar

void command_exit() {

	printf("\n");
	printf("Saliendo..");

}


