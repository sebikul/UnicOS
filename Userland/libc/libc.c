
#include <syscalls.h>
#include <libc.h>

static void * mallocBuffer = (void*)0x600000;

void* malloc(int len) {

	mallocBuffer += len;

	return mallocBuffer;

}

void free(void* m) {

}


void printf(char* arg) {
	//TODO: ver stdarg.h <- argumentos variables

	sys_write(FD_STDOUT, arg, strlen(arg));

}

void putchar(char c) {

	sys_write(FD_STDOUT, &c, 1);
}

int strlen(char* str) {
	int size;
	for (size = 0; *str != '\0' ; str++) {
		size++;
	}

	return size;
}

int getchar() {

	static char buffer[256] = {0};


	//printf("Llamando a getchar.");

	// char test[1]={c};

	int read = sys_read(FD_STDOUT, buffer, 6);

	buffer[read] = 0;

	printf(buffer);
	// if (test==c)
	// 	return -1; //no pudo leer el caracter -diferenciar motivos?-

	// return 1;
}

//len debe ser menor al buffer de teclado
int scanf(char* c, int len) {

	int size = 0;

	while ((*c = getchar()) != EOF || size < len) {
		c++;
		size++;
	}

	return size;
}

void printTime() {

	int timer[3] = {0};
	char timerc[6] = {0};

	sys_rtc_time(&timer[0], &timer[1], &timer[2]);

	intToChar(timer[0], timerc); //*timerc & *timerc+1 -> hours
	intToChar(timer[1], timerc + 2); //*timerc+2 & *timerc+3 -> minutes
	intToChar(timer[2], timerc + 4); //*timerc+4 & *timerc+5 -> seconds

	for (int i = 0; i < 6; i++)
	{
		putchar(timerc[i]);
		if (i % 2 != 0 && i != 5)
			putchar(':');
	}

}

char* intToChar(int number, char* c) {

	int i = 0;
	int j = 0;
	int cnt = 0;

	if (number < 0) {
		number = -number;
		c[i++] = '-';
	}

	while (number >= 10 ) {
		int dig = number % 10;
		number /= 10;
		c[i++] = dig + '0';
		cnt++;
	}
	c[i] = number + '0';

	while (cnt >= j) {
		char aux;
		aux = c[cnt];
		c[cnt--] = c[j];
		c[j++] = aux;
	}

	return c;

}
