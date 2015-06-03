#include <stdint.h>

#if ! MACOS
#include <string.h>
#endif

#include <lib.h>
#include <moduleLoader.h>
#include <video.h>
#include <keyboard.h>
//#include <rtc.h>


extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

extern char keyboard_kbuffer[KEYBOARD_BUFFER_SIZE];

static const uint64_t PageSize = 0x1000;

static void * const shellCodeModuleAddress = (void*)0x400000;
static void * const shellDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void load_kernel_modules();

void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void*)(
	           (uint64_t)&endOfKernel
	           + PageSize * 8				//The size of the stack itself, 32KiB
	           - sizeof(uint64_t)			//Begin at the top of the stack
	       );
}

void * initializeKernelBinary() {

	load_kernel_modules();

	clearBSS(&bss, &endOfKernel - &bss);


	video_initialize();
	video_clear_screen();

	//rtc_init();

	video_write_line("[x64BareBones]");

	video_write_string("  text: 0x");
	video_write_hex((uint64_t)&text);
	video_write_nl();

	video_write_string("  rodata: 0x");
	video_write_hex((uint64_t)&rodata);
	video_write_nl();

	video_write_string("  data: 0x");
	video_write_hex((uint64_t)&data);
	video_write_nl();

	video_write_string("  bss: 0x");
	video_write_hex((uint64_t)&bss);
	video_write_nl();

	video_write_line("[Done]");

	video_write_line("Kernel cargado.");

	return getStackBase();
}

void load_kernel_modules() {

	//video_write_pline("[Loading modules]");

	void * moduleAddresses[] = {
		shellCodeModuleAddress,
		shellDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	//video_write_pline("[Done]");

}

int main() {

	video_write_string("Keyboard buffer at: 0x");
	video_write_hex((uint64_t)&keyboard_kbuffer);
	video_write_nl();
	video_write_string("Keyboard buffer size: ");
	video_write_dec((uint64_t)KEYBOARD_BUFFER_SIZE);
	video_write_nl();

	video_write_line("[Kernel Main]");

	video_write_string("  Shell code module at 0x");
	video_write_hex((uint64_t)shellCodeModuleAddress);
	video_write_nl();

	video_write_line("Calling shell module...");
	video_write_nl();
	((EntryPoint)shellCodeModuleAddress)();

	return 0;
}
