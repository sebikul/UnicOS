#include <stdint.h>
#include <syscalls.h>
#include <libc.h>

#include <commands.h>

char * v = (char*)0xB8000 + 79 * 2;

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

void * memset(void * destiny, int32_t c, uint64_t length);

int main() {

	static char buffer[256] = {0};
	int len;

	memset(&bss, 0, &endOfBinary - &bss);

	printf("Ejecutando ShellModule...\n");

	while (1) {

		printf("\n user@localhost $ ");

		len = scanf(buffer, 100);

		command_dispatcher(buffer);

	}


	//sys_write(FD_STDOUT, "Ejecutando ShellModule", 4);

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}

void command_dispatcher(char* command) {

	char* args;

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



	if (strcmp(command, "echo")) {
		//printf("Ejecutando echo: \n");
		command_echo(args);
	}else{
		printf("\nComando no encontrado: ");
		printf(command);
	}


}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while (length--)
		dst[length] = chr;

	return destiation;
}