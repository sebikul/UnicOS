
#ifndef HLIBC_H
#define HLIBC_H

#include <syscalls.h>
#include <libc.h>

static void * mallocBuffer = (void*)0x600000;

void* malloc(int len) {

	mallocBuffer += len * sizeof(void*);

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

	char read = sys_read(FD_STDOUT, c, len);

	return read;
}


int strcmp(const char* s1, const char* s2) {

	while (*s1 && *s1 == *s2) {
		s1++;
		s2++;
	}

	return *s1 - *s2;
}


// Verificado
char* strcpy(char* src, char* dest) {

	while (*src != 0) {
		*dest = *src;
		src++;
		dest++;
	}

	*dest = 0;

	return dest;
}


//Anda todo.. lo que no pude probar es sys_rtc_time y el putchar  lo demas anda!
void printTime() {

	char* rtctime[3];

	int timer[3] = {0};

	sys_rtc_time(&timer[0], &timer[1], &timer[2]);

	rtctime[0] = intToChar(timer[0]); //*timerc & *timerc+1 -> hours
	rtctime[1] = intToChar(timer[1]); //*timerc+2 & *timerc+3 -> minutes
	rtctime[2] = intToChar(timer[2]); //*timerc+4 & *timerc+5 -> seconds

	for (int i = 0; i < 6; i++)
	{
		printf(rtctime[i]);
		if (i % 2 != 0 && i != 5)
			putchar(':');
	}

}

//Verificado
char* intToChar(int number) {

	int i = 0;
	int j = 0;
	int cnt = 0;

	char* c = malloc(10);

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

#endif
