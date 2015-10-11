#include <types.h>

#ifndef STRING_H
#define STRING_H

void * memcpy(void * destination, const void * source, uint64_t length);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
int strpos(const char* s, char n);
void * memset(void * destiation, int32_t c, uint64_t length);
int strlen(const char* str);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

#endif
