#include <stdint.h>
#include <syscalls.h>
#include <libc.h>

#include <commands.h>

#define MAX_ARGS 				256
#define CMD_BUFFER_SIZE 		2*MAX_ARGS

#define MAX_HISTORY_SIZE 		256

#define LEFT_STRIP(str)			while (*(++command) == ' ')

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

static char* shell_history[MAX_HISTORY_SIZE] = {0};
static int current_history = 0;
static int max_history = 0;

static int cmd_count = 7;
char** cmd_list;

void keyboard_uparrow_handler(uint64_t s);
void keyboard_downarrow_handler(uint64_t s);

void command_dispatcher(char* command);

int main() {

	static char buffer[CMD_BUFFER_SIZE] = {0};

	memset(&bss, 0, &endOfBinary - &bss);

	initialize_command_list();

	fprintf(FD_STDERR, "Ejecutando \"ShellModule...numero que me gusta: %i.\n", 50);
	printf("Este es un caracter %c, y este es un numero %i.\n", 'A', 78);

	printf("Este numero deberia tener 5 digitos %05i. Y este 3: %03i\n", 453, 78);


	sys_keyboard_catch(0x48, keyboard_uparrow_handler);
	sys_keyboard_catch(0x50, keyboard_downarrow_handler);

	while (1) {

		printf("\nuser@localhost $ ");

		if (scanf(buffer, CMD_BUFFER_SIZE) == 0) {
			continue;
		}

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

	int argc = 0;
	char** argv = calloc(MAX_ARGS * sizeof(char*));

	//backup en el historial
	shell_history[max_history] = calloc(strlen(command) + 1);

	strcpy(shell_history[max_history], command);

	//current_history++;
	max_history++;

	current_history = max_history;

	//Vamos a sacarle todos los espacion al principio del comando
	if (*command == ' ') {
		//printf("Limpiando espacios\n");
		LEFT_STRIP(command);
	}

	//printf("Parseando argumentos...\n");

	while (*command != 0) {

		//alocamos espacio para el argumento que estamos parseando
		argv[argc] = calloc(CMD_BUFFER_SIZE * sizeof(char));


		//copiamos el puntero a la cadena por comodidad, para poder modificarlo
		char* pos = argv[argc];

		bool comillas = (*command == '"');

		//printf("Parseando argumento: %i\n", argc);
		//printf("Cadena que resta por procesar: %s\n", command);

		if (comillas)
			command++;

		while (((!comillas && *command != ' ') || (comillas && *command != '"')) && *command != 0) {

			*pos = *command;
			pos++;

			command++;
		}

		if (comillas && *command == '"') {
			command++;
			comillas = FALSE;
		}

		if (comillas) {
			fprintf(FD_STDERR, "Comando mal formateado. Contiene comillas sin cerrar!\n");
		}

		//si al argumento le siguen espacios los limpiamos
		if (*command == ' ') {
			LEFT_STRIP(command);
		}

		argc++;

	}



	// printf("Comando a ejecutar: <%s>\n", argv[0]);

	// for (int i = 1; i < argc; i++) {
	// 	printf("Argumento %i: <%s>\n", i, argv[i]);
	// }
/*
	if (strcmp(argv[0], "echo") == 0) {
		//printf("Ejecutando echo: \n");
		command_echo(argc, argv);
	} else if (strcmp(argv[0], "help") == 0) {
		command_help(argc, argv);
	} else if (strcmp(argv[0], "time") == 0) {
		command_time(argc, argv);
	} else {
		printf("%s: Comando no encontrado", argv[0]);
	}
*/
	 int cmd = 0;

	for (; cmd < cmd_count; cmd++) {
	 	if (strcmp(argv[0], cmd_list[cmd]) ==0) {
	 		break;
	 	}
	 }
	 
	 switch (cmd) {
	
	 case 0: //echo
	 	printf("\nEjecutando echo...\n");
	 	command_echo(argc, argv);
	 	break;

	 case 1: //help

	 	command_help(argc, argv);
	 	break;

	 case 2: //time

	 	//command_time();
	 	break;

	 case 3: //color

	 	break;

	 case 4: //exit

	 	//command_exit();
	 	break;

	 case 5: //clear

	 	//command_clean();
	 	break;

	 case 6: //restart

	 	//command_restart();
	 	break;

	// //other functions....

	 default:

	 	printf("\nComando no encontrado: ");
	}

}

void keyboard_uparrow_handler(uint64_t s) {

	if (current_history == 0) {
		return;
	}

	current_history--;

	sys_clear_indexed_line(0);

	printf("user@localhost $ %s", shell_history[current_history]);

	sys_keyboard_replace_buffer(shell_history[current_history]);

}

void keyboard_downarrow_handler(uint64_t s) {

	if (current_history == max_history-1 || max_history==0) {

		sys_clear_indexed_line(0);
		printf("user@localhost $ ");
		sys_keyboard_replace_buffer("");

		return;
	}

	current_history++;

	sys_clear_indexed_line(0);

	printf("user@localhost $ %s", shell_history[current_history]);

	sys_keyboard_replace_buffer(shell_history[current_history]);

}

void initialize_command_list(){
	cmd_list = calloc(cmd_count * sizeof(char*));
	calloc_cmd(0,"echo");
	calloc_cmd(1,"help");
	calloc_cmd(2,"time");
	calloc_cmd(3,"color");
	calloc_cmd(4,"exit");
	calloc_cmd(5,"clear");
	calloc_cmd(6,"restart");
}

void calloc_cmd(int i, char* str){
	int len= strlen(str);
	cmd_list[i] = calloc(len * sizeof(char));
	int j=0;
	for (; j < len; j++)
	{
		cmd_list[i][j]=str[j];
	}
	cmd_list[i][j]=0;

}

