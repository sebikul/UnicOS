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

static uint64_t pit_timer = 0;
//Screensaver, 20 segundos por defecto
uint64_t screensaver_wait_time = 20;
uint64_t screensaver_timer = 0;
bool screensaver_is_active = FALSE;

uintptr_t kernel_stack=NULL;

void load_kernel_modules();

void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

uintptr_t stack_init(){
	kernel_stack = malloc(STACK_SIZE);
	kernel_stack = kernel_stack+ STACK_SIZE;

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

	screensaver_reset_timer();

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

	// for (uint64_t i = 0; i < VIRTUAL_CONSOLES; i++) {
	// 	video_write_string(i, "Console #: ");
	// 	video_write_dec(i, i);
	// 	video_write_nl(i);
	// }

	//intson();
	//while(TRUE);

	//video_write_line(KERNEL_CONSOLE, "Calling shell module...");
	//video_write_nl(KERNEL_CONSOLE);
	//((task_entry_point)shellCodeModuleAddress)(0, NULL);
}

//retorna si se debe ignorar lo tecleado
bool screensaver_reset_timer() {

	bool ret = FALSE;

	if (screensaver_is_active) {
		ret = TRUE;
		screensaver_is_active = FALSE;
		video_trigger_restore();

	}
	screensaver_timer = 18 * screensaver_wait_time;

	return ret;
}

void active_screensaver() {
	screensaver_is_active = TRUE;
	video_trigger_screensaver();
}

void irq0_handler() {
	pit_timer++;
	screensaver_timer--;

	if (screensaver_timer == 0 && !screensaver_is_active) {
		active_screensaver();
	}
}

void _kdebug(const char* s) {
	while (*s != 0) {
		serial_send(*s);
		s++;
	}
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
