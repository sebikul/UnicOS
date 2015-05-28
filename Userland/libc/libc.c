
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

	static char buffer[2] = {0};

	char read = sys_read(FD_STDOUT, buffer, 1);

	if (read == EOF) {
		return EOF;
	}

	return (int) buffer[0];
}

int scanf(char* c, int len) {

	char tmp;

	int size = 0;

	while ((tmp = getchar()) != EOF && size < len) {

		*c = tmp;

		c++;
		size++;

		printf(c);
	}

	*c = 0;

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
