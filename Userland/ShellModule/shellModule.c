#include <stdint.h>
#include <syscalls.h>

char * v = (char*)0xB8000 + 79 * 2;

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

char *hola = "Hola";
char *hola2 = "Hola2";

void * memset(void * destiny, int32_t c, uint64_t length);

int main() {
	//Clean BSS
	//memset(&bss, 0, &endOfBinary - &bss);

	//All the following code may be removed


	//sys_write(FD_STDOUT, 0x6810, 4);
	//sys_write(FD_STDOUT, hola2, 4);

	//sys_write(FD_STDOUT,0x500000,4);

// 	hola = 0x500000;

	*v = 'X';
// *(v-2) = *(char*)0x500000;

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