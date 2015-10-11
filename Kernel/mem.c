#include <stdint.h>
#include <mem.h>

static void* mallocBuffer = (void*)0x600000;

static void* lastMalloc;

void* malloc(int len) {

	lastMalloc = mallocBuffer;

	mallocBuffer += len * sizeof(void*);

	return lastMalloc;

}

void* calloc(int len) {
	char* space = (char*)malloc(len);

	for (int i = 0; i < len; i++) {
		space[i] = (char)0;
	}

	return (void*)space;
}

void free(void* m) {

	if (m == lastMalloc) {
		mallocBuffer = m;
	}

}