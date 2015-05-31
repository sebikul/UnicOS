
#ifndef LIBC_H
#define LIBC_H

#include <syscalls.h>

#define FD_STDERR 		0
#define FD_STDOUT		1

#define EOF 	-1

#define TRUE 1
#define FALSE 0

#define MAX_PRINTF_LEN 1024

typedef char bool;

void* malloc(int len);
void* calloc(int len);
void free(void* m);
int strpos(char* s, char n);

void fprintf(FD fd, char* fmt, ...) __attribute__ ((format (printf, 2, 3)));;

void printf(char* fmt, ...) __attribute__ ((format (printf, 1, 2)));

void putchar(char c);

int strlen(char* str);

int getchar();

int scanf(char* c, int len);

char* intToChar(int number) ;

time_t* time();

int strcmp(const char* s1, const char* s2);

char* strcpy(char* src, char* dest);

void * memset(void * destiny, int32_t c, uint64_t length);

#endif