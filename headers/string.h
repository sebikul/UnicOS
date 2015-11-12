#include <types.h>

#ifndef STRING_H
#define STRING_H

void * memcpy(void * destination, const void * source, uint64_t length);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, int n);
char* strcpy(char* dest, const char* src);
int strpos(const char* s, char n);
void * memset(void * destiation, char c, uint64_t length);
int strlen(const char* str);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

#endif
