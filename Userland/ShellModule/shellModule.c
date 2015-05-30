#include <stdint.h>
#include <syscalls.h>
#include <libc.h>

#include <commands.h>

char * v = (char*)0xB8000 + 79 * 2;

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

// static char* cmd_list[] = {"echo", "help", "time", "set time", "backcolor", "fontcolor", "exit", "clean", "restart", 0};
// int cmd_count;

void * memset(void * destiny, int32_t c, uint64_t length);
void command_dispatcher(char* command);

int main() {

	static char buffer[256] = {0};
	int len, cmd_count;

	memset(&bss, 0, &endOfBinary - &bss);

	// for (int i = 1;; i++) {
	// 	if (*cmd_list[i] == 0) {
	// 		cmd_count = i - 2;
	// 		break;
	// 	}
	// }

	// printf("# de comandos para ejecutar: ");
	// printf(intToChar(cmd_count));
	// printf("\n");



	fprintf(FD_STDERR, "Ejecutando ShellModule...numero que me gusta: %i.\n", 50);
	printf("Este es un caracter %c, y este es un numero %i.\n", 'A',78);

	while (1) {

		printf("\n user@localhost $ ");

		len = scanf(buffer, 100);
		putchar('\n');

		command_dispatcher(buffer);

	}

	//sys_write(FD_STDOUT, "Ejecutando ShellModule", 4);

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}

void command_dispatcher(char* command) {

	char* args; //estaria bueno consumir todo el buffer porque hay funciones que reciben mas de un parametro
	//o podria pasar que hago backcolor red ÑADGUHASGJAGNHLK y cosas asi

	char* c = command;
	while (*c != ' ' && *c != 0) {
		c++;
	}

	if (*c == ' ') {
		*c = 0;
		args = c + 1;
	}


	// printf("\nComando a ejecutar: <");
	// printf(command);
	// printf(">\n");
	// printf("Argumentos: <");
	// printf(args);
	// printf(">\n");
	if(strcmp(command,"time")==0){
		command_time();
	}

	if (strcmp(command, "echo") == 0) {
		//printf("Ejecutando echo: \n");
		command_echo(args);
	} else if(strcmp(command, "help") == 0){
		command_help(args);
	}else{
		printf("\nComando no encontrado: %s", command);
	}

	// int cmd = -1;

	// for (; cmd < cmd_count; cmd++) {
	// 	if (strcmp(command, cmd_list[cmd])) {
	// 		break;
	// 	}
	// }

	// switch (cmd) {

	// case 1: //echo
	// 	printf("\nEjecutando echo...\n");
	// 	command_echo(args);
	// 	break;

	// case 2: //help

	// 	command_help();
	// 	break;

	// case 3: //time

	// 	command_time();
	// 	break;

	// case 4: //set time


	// 	int h=charToInt(arg[0]);
	// 	int m=charToInt(arg[1]);
	// 	int s=charToInt(arg[2]);

	// 	//que cada funcion valide sus parametros

	// 	command_settime(h,m,s);


	// 	break;

	// case 5: //backcolor

	// 	/*
	// 	int color=charToInt(arg[0]);
	// 	command_backcolor(color);
	// 	*/

	// 	break;

	// case 6: //fontcolor

	// 	/*
	// 	int color=charToInt(arg[0]);
	// 	command_fontcolor(color);
	// 	*/
	// 	break;

	// case 7: //exit

	// 	command_exit();
	// 	break;

	// case 8: //clean

	// 	command_clean();
	// 	break;

	// case 9: //restart

	// 	command_restart();
	// 	break;

	// //other functions....

	// default:

	// 	printf("\nComando no encontrado: ");
	// 	printf(command);


	// }

}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while (length--)
		dst[length] = chr;

	return destiation;
}