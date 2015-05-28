#include <stdint.h>
#include <syscalls.h>
#include <libc.h>

char * v = (char*)0xB8000 + 79 * 2;

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

char *hola = "Hola";
char *hola2 = "Hola2";


void * memset(void * destiny, int32_t c, uint64_t length);

int main() {

	static char buffer[256] = {0};
	int len;
	char c;

	memset(&bss, 0, &endOfBinary - &bss);

	printf("Ejecutando ShellModule\n");

	printf("Ingrese un texto: ");

	len = scanf(buffer, 7);

	printf("\nEl texto ingresado es: ");
	printf(buffer);



	//sys_write(FD_STDOUT, "Ejecutando ShellModule", 4);

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while (length--)
		dst[length] = chr;

	return destiation;
}