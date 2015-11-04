#include <stdint.h>
#include <mem.h>

static const uint64_t page_size = 0x1000;

static uint64_t* pmm_stack_start = (uint64_t*)(6*0x100000);
static uint64_t* pmm_stack_end = (uint64_t*)(14*0x100000);
static uint64_t* pmm_stack_current;

static void* mallocBuffer = (void*)0x600000;

static void* lastMalloc;

void* malloc(int len) {

	lastMalloc = mallocBuffer;

	mallocBuffer += len * sizeof(char);

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

void pmm_initialize() {
	// End of the system memory is 512 MB
	uint64_t end_of_memory = 512*0x100000;
	uint64_t start_of_free_memory = (uint64_t)pmm_stack_end;
	pmm_stack_current = pmm_stack_start;
	uint64_t nframes = (end_of_memory - start_of_free_memory)/page_size;
	uint64_t i;
	for(i=0 ; i < nframes; i++){
		*pmm_stack_current = start_of_free_memory+(i*page_size);
		pmm_stack_current += sizeof(uint64_t*);
	}
}

void* pmm_page_alloc(){
	pmm_stack_current -= sizeof(uint64_t*);
	return (void*)(*pmm_stack_current);
}

void pmm_page_free(void* dir){
	*pmm_stack_current = (uint64_t)dir;
	pmm_stack_current += sizeof(uint64_t*);
}