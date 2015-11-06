#include <stdint.h>
#include <moduleLoader.h>
#include "video.h"
#include "keyboard.h"
#include "types.h"
#include "mem.h"
#include "string.h"
#include "task.h"
#include "input.h"
#include "serial.h"
#include "kernel.h"
#include "syscalls.h"

#if ! MACOS
#include <string.h>
#endif

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const shellCodeModuleAddress = (void*)0x400000;
static void * const shellDataModuleAddress = (void*)0x500000;

#define MSPERTICK 	55

uint64_t pit_timer = 0;

void *kernel_stack = NULL;

void load_kernel_modules();

void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void* stack_init() {
	kernel_stack = malloc(STACK_SIZE);
	kernel_stack = kernel_stack + STACK_SIZE;

	return kernel_stack;
}

void initializeKernelBinary() {

	//gdt_init();

	load_kernel_modules();

	clearBSS(&bss, &endOfKernel - &bss);

	serial_init();
	keyboard_init();
	video_init();
	input_init();

	kdebug("Kernel inicializado\n");

	video_write_line(KERNEL_CONSOLE, "[x64BareBones]");

	video_write_string(KERNEL_CONSOLE, "  text: 0x");
	video_write_hex(KERNEL_CONSOLE, (uint64_t)&text);
	video_write_nl(KERNEL_CONSOLE);

	video_write_string(KERNEL_CONSOLE, "  rodata: 0x");
	video_write_hex(KERNEL_CONSOLE, (uint64_t)&rodata);
	video_write_nl(KERNEL_CONSOLE);

	video_write_string(KERNEL_CONSOLE, "  data: 0x");
	video_write_hex(KERNEL_CONSOLE, (uint64_t)&data);
	video_write_nl(KERNEL_CONSOLE);

	video_write_string(KERNEL_CONSOLE, "  bss: 0x");
	video_write_hex(KERNEL_CONSOLE, (uint64_t)&bss);
	video_write_nl(KERNEL_CONSOLE);

	video_write_line(KERNEL_CONSOLE, "[Done]");

	//screensaver_reset_timer();

	video_write_line(KERNEL_CONSOLE, "Kernel cargado.");
}

void load_kernel_modules() {

	void * moduleAddresses[] = {
		shellCodeModuleAddress,
		shellDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
}

void main() {

	video_write_string(KERNEL_CONSOLE, "-->Kernel Stack at: 0x");
	video_write_hex(KERNEL_CONSOLE, (uint64_t)kernel_stack);
	video_write_nl(KERNEL_CONSOLE);

	video_write_line(KERNEL_CONSOLE, "[Kernel Main]");

	video_write_string(KERNEL_CONSOLE, "  Shell code module at 0x");
	video_write_hex(KERNEL_CONSOLE, (uint64_t)shellCodeModuleAddress);
	video_write_nl(KERNEL_CONSOLE);

	video_write_line(KERNEL_CONSOLE, "Creando consolas...");
	task_init();

	pit_setup(10);

	// TAREAS DEL KERNEL
	//screensaver_init();

	// intson();

	// while(TRUE){
	// 	video_write_string(KERNEL_CONSOLE, "PIT Timer: ");
	// 	video_write_dec(KERNEL_CONSOLE,  pit_timer);
	// 	video_write_nl(KERNEL_CONSOLE);
	// }

	// for (console_t i = 0; i < VIRTUAL_CONSOLES; i++) {
	// 	video_write_string(i, "Console #: ");
	// 	video_write_dec(i, i);
	// 	video_write_nl(i);
	// }

	//intson();
	//while(TRUE);

	//video_write_line(KERNEL_CONSOLE, "Calling shell module...");
	//video_write_nl(KERNEL_CONSOLE);
	//((task_entry_point)shellCodeModuleAddress)(0, NULL);

	//while(TRUE);
}

void irq0_handler() {

}

void _kdebug(const char* s) {
	while (*s != 0) {
		serial_send(*s);
		s++;
	}
}

uint64_t get_ms_since_boot() {
	return pit_timer;
}

void kdebug_char(char c) {
	serial_send(c);
}

static char buffer[128] = { 0 };

void kdebug_base(uint64_t value, uint32_t base) {
	uintToBase(value, buffer, base);
	_kdebug(buffer);
}

void kdebug_nl() {
	serial_send('\n');
}

bool kset_ints(bool ints) {
	uint64_t flags = get_flags();
	if (ints) {
		intson();
	} else {
		intsoff();
	}
	return (flags & FLAGS_INT_BIT) != 0;
}
