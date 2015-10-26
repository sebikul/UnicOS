#include <libc.h>

#include "string.h"
#include "syscalls.h"

static bool _exit = 0;

static void keyboard_handler(uint64_t s) {

	printf("Scancode: %x\n", (uint32_t) s);

	if (s == 0x01) {
		printf("ESC presionado. Saliendo...\n");
		_exit = TRUE;
	}

}


void command_rawkbd(int argc, char** argv) {

	int index;

	_exit = 0;

	printf("Installing keyboard handler...\n");
	index = sys_keyboard_catch(0x0, keyboard_handler, KEYBOARD_IGNORE|KEYBOARD_WILDCARD);

	while (!_exit);

	sys_keyboard_clear_handler(index);
}
