#include <libc.h>

extern char* user_name;
extern char* host_name;
extern int USER_SIZE;
extern int HOST_SIZE;

void command_user_name(int argc, char** argv){

	if(argc!=2){
		printf("Comando invalido. Debe ingresar un unico nombre de usuario.\n");
	}else{
		if(strlen(argv[1])>USER_SIZE){
			printf("El nombre de usuario es demasiado largo, el tamaño maximo es: %i",USER_SIZE);
		}else{
			strcpy(user_name, argv[1]);
		}
	}
}

void command_host_name(int argc, char** argv){

	if(argc!=2){
		printf("Comando invalido. Debe ingresar un unico nombre de host.\n");
	}else{
		if(strlen(argv[1])>HOST_SIZE){
			printf("El nombre del host es demasiado largo, el tamaño maximo es: %i",HOST_SIZE);
		}else{
			strcpy(host_name, argv[1]);
		}
	}	
}