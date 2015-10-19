#include <stdint.h>

// Codigo adaptado de https://github.com/mohamed-anwar/os

typedef struct {
	uint32_t limit_l 	: 16;
	uint32_t base_l  	: 24;
	uint8_t  type		: 4;
	uint8_t  sys		: 1;
	uint8_t	 dpl		: 2;
	uint8_t	 present	: 1;
	uint16_t limit_h	: 4;
	uint8_t  avil 		: 1;
	uint8_t  longmode	: 1;
	uint8_t	 size		: 1;
	uint8_t  gran		: 1;
	uint32_t base_h 	: 8;
} __attribute__((packed)) gdt_entry_t;

static gdt_entry_t gdt[128]={0};

typedef struct {
	uint16_t limit;
	uint64_t base;
} gdtptr_t;

static gdtptr_t gdtptr = {
	.limit = sizeof(gdt) - 1,
	.base  = (uint64_t)&gdt,
};

void gdt_set_entry(uint32_t id, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {

	gdt_entry_t entry = {0};

	entry.limit_l = limit & 0xFFFF;
	entry.base_l = base  & 0xFFFFFF;
	entry.type = access & 0xF;
	entry.sys = 1;
	entry.dpl = (access >> 5) & 3;
	entry.present = (access >> 7) & 1;
	entry.limit_h = (limit >> 16) & 0xF;
	entry.avil = 1;
	entry.longmode = (flags >> 1) & 1;
	entry.size = ((flags >> 1) & 1) ? 0 : ((flags >> 2) & 1);
	entry.gran = (flags >> 3) & 1;
	entry.base_h  = base >> 24;

	gdt[id] = entry;
}

#define ACC_TYPE_CS 0xA
#define ACC_TYPE_DS 0x2
#define ACC_DPL0	0
#define ACC_DPL3	0x60
#define ACC_PRESENT 0x80

#define FLAGS_L		0x2
#define FLAGS_32	0x4
#define FLAGS_gran	0x8

extern void gdt_flush(gdtptr_t gdtptr);

void gdt_init(){
	gdt_set_entry(0, 0, 0, 0, 0);	// Null decriptor
	gdt_set_entry(1, 0, -1, ACC_TYPE_CS | ACC_DPL0 | ACC_PRESENT, FLAGS_L); // Kernel CS
	gdt_set_entry(2, 0, -1, ACC_TYPE_DS | ACC_DPL0 | ACC_PRESENT, FLAGS_L); // Kernel DS
	gdt_set_entry(3, 0, -1, ACC_TYPE_CS | ACC_DPL3 | ACC_PRESENT, FLAGS_L);	// User CS
	gdt_set_entry(4, 0, -1, ACC_TYPE_DS | ACC_DPL3 | ACC_PRESENT, FLAGS_L); // User DS

	gdt_flush(gdtptr);
}