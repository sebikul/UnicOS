
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

void fprintf(FD fd, char* fmt, ...);

void printf(char* fmt, ...) ;

void putchar(char c);

int strlen(char* str);

int getchar();

int scanf(char* c, int len);

char* intToChar(int number) ;

void printTime();

int strcmp(const char* s1, const char* s2);

char* strcpy(char* src, char* dest);

#endif