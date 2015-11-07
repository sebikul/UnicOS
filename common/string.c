#include <types.h>

int strlen(const char* str) {
	int size;
	for (size = 0; *str != '\0' ; str++) {
		size++;
	}

	return size;
}

void * memcpy(void * destination, const void * source, uint64_t length) {
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
	        (uint64_t)source % sizeof(uint32_t) == 0 &&
	        length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

int strcmp(const char* s1, const char* s2) {

	while (*s1 && *s1 == *s2) {
		s1++;
		s2++;
	}

	return *s1 - *s2;
}

char* strcpy(char* dest, const char* src) {

	char* bk = dest;

	while (*src != 0) {
		*dest = *src;
		src++;
		dest++;
	}

	*dest = 0;

	return bk;
}

void * memset(void * destiation, char c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	for (uint64_t i = 0; i < length; i++) {
		dst[i] = c;
	}

	return destiation;
}

int strpos(const char* s, char n) {

	for (int pos = 0; s[pos] != 0; pos++) {
		if (s[pos] == n) {
			return pos;
		}
	}

	return -1;
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base) {
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

