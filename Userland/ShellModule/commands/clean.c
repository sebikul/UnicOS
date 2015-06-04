#include <libc.h>

void command_clear(int argc){
	if(argc==1){
		clear_screen();
	}else{
		printf("Comando invalido, no se esperan parametros.\n");
	}
}