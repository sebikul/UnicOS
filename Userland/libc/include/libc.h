#include <types.h>

#ifndef LIBC_H
#define LIBC_H

#define MAX_PRINTF_LEN 1024

#define errno sys_errno();

void* malloc(int len);
void* calloc(int len);
void free(void* m);
void fprintf(int fd, char* fmt, ...) __attribute__ ((format (printf, 2, 3)));;
void printf(char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
void putchar(char c);
int getchar();
int scanf(char* c, int len);
char* itoc(int number) ;
int ctoi(char* c);
color_t get_color();
void set_color(vga_color fg, vga_color bg);
time_t* time();
bool string_numeric(char* str);
bool is_numeric(char c);
void clear_screen();
void set_time(time_t * t);
void exit();

//fixme
char* strcat(char* str1, char* str2);

#endif
