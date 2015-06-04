
#ifndef LIBC_H
#define LIBC_H

#include <types.h>

#define MAX_PRINTF_LEN 1024

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
char* itoc(int number) ;
int ctoi(char* c);
color_t get_color();
void set_color(vga_color fg, vga_color bg);
time_t* time();
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, char* src);
void * memset(void * destiny, int32_t c, uint64_t length);
bool stringNumeric(char* str);
bool isNumeric(char c);
char* strcat(char* str1, char* str2);



#endif
