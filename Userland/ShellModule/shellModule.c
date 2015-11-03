#include <stdint.h>
#include <types.h>
#include "syscalls.h"
#include "libc.h"
#include "string.h"
#include "commands.h"

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

static int arrows_handlers[2] = {0};

command_list_t* cmdlist;

char* user_name;
char* host_name;
int USER_SIZE = 20;
int HOST_SIZE = 20;

void keyboard_uparrow_handler(uint64_t s);
void keyboard_downarrow_handler(uint64_t s);

void command_dispatcher(char* command);

static void print_commands_struct();
static void initialize_names();

uint64_t main(int argc, char** argv) {

	static char buffer[CMD_BUFFER_SIZE] = {0};

	memset(&bss, 0, &endOfBinary - &bss);

	ksysdebug("Initializing shell\n");

	cmdlist = malloc(sizeof(command_list_t));
	cmdlist->count = 0;
	cmdlist->commands = malloc(32 * sizeof(command_t));

	COMMAND_INIT(help);
	COMMAND_INIT(echo);
	COMMAND_INIT(set_distribution);
	COMMAND_INIT(time);
	COMMAND_INIT(user_name);
	COMMAND_INIT(host_name);
	COMMAND_INIT(color);
	COMMAND_INIT(refresh);
	COMMAND_INIT(screensaver);
	COMMAND_INIT(exit);
	COMMAND_INIT(clear);
	COMMAND_INIT(rawkbd);
	COMMAND_INIT(ps);

	initialize_names();

	print_commands_struct();

	arrows_handlers[0] = sys_keyboard_catch(0x48, keyboard_uparrow_handler, 0, "up arrow");
	arrows_handlers[1] = sys_keyboard_catch(0x50, keyboard_downarrow_handler, 0, "down arrow");

	printf("Registrando handlers: up=%d down=%d\n", arrows_handlers[0], arrows_handlers[1]);

	while (TRUE) {

		printf("\n%s@%s $ ", user_name, host_name);

		if (scanf(buffer, CMD_BUFFER_SIZE) == 0) {
			continue;
		}

		ksysdebugs(buffer);

		command_dispatcher(buffer);
	}

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

	max_history++;

	current_history = max_history;

	//Vamos a sacarle todos los espacion al principio del comando
	if (*command == ' ') {
		LEFT_STRIP(command);
	}

	while (*command != 0) {

		//alocamos espacio para el argumento que estamos parseando
		argv[argc] = calloc(CMD_BUFFER_SIZE * sizeof(char));

		//copiamos el puntero a la cadena por comodidad, para poder modificarlo
		char* pos = argv[argc];

		bool comillas = (*command == '"');

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

	for (int cmd = 0; cmd < cmdlist->count; cmd++) {
		// ksysdebug("Comparando ");
		// ksysdebugs(cmdlist->commands[cmd]->name);
		// ksysdebug(" con ");
		// ksysdebugs(argv[0]);

		if (strcmp(cmdlist->commands[cmd]->name, argv[0]) == 0) {
			pid_t shellpid = sys_task_get_pid();

			pid_t taskpid = sys_task_create(cmdlist->commands[cmd]->func, cmdlist->commands[cmd]->name, argc, argv);

			sys_task_ready(taskpid);
			sys_task_join(taskpid, shellpid);
			return;
		}
	}

	fprintf(FD_STDERR, "Comando no encontrado.");
}

void keyboard_uparrow_handler(uint64_t s) {

	ksysdebug("Flecha arriba\n");

	if (current_history == 0) {
		return;
	}

	current_history--;

	sys_clear_indexed_line(0);

	printf("%s@%s $ %s", user_name, host_name, shell_history[current_history]);

	sys_keyboard_replace_buffer(shell_history[current_history]);
}

void keyboard_downarrow_handler(uint64_t s) {

	ksysdebug("Flecha abajo\n");

	if (current_history == max_history - 1 || max_history == 0) {

		sys_clear_indexed_line(0);
		printf("%s@%s $ ", user_name, host_name);
		sys_keyboard_replace_buffer("");
		if (current_history == max_history - 1) {
			current_history = max_history;
		}
		return;
	} else if (current_history == max_history) {
		return;
	}

	current_history++;

	sys_clear_indexed_line(0);

	printf("%s@%s $ %s", user_name, host_name, shell_history[current_history]);

	sys_keyboard_replace_buffer(shell_history[current_history]);
}

static void print_commands_struct() {

	printf("Commands: [\n");
	for (int i = 0; i < cmdlist->count; i++) {
		printf("\t%s\n", cmdlist->commands[i]->name);
	}
	printf("]\n");
}

static void initialize_names() {
	user_name = calloc(USER_SIZE * sizeof(char));
	host_name = calloc(HOST_SIZE * sizeof(char));
	strcpy(user_name, "user");
	strcpy(host_name, "localhost");
}