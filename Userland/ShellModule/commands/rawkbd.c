#include <libc.h>
#include "string.h"
#include "syscalls.h"
#include "commands.h"

static bool _exit = 0;

COMMAND_HELP(rawkbd, "[rawkbd] Imprime los scancodes presionados del teclado.");

static void keyboard_handler(uint64_t s) {

	printf("Scancode: %x\n", (uint32_t) s);

	if (s == 0x01) {
		printf("ESC presionado. Saliendo...\n");
		_exit = TRUE;
	}
}

COMMAND_START(rawkbd) {

	int index;

	_exit = 0;

	printf("Installing keyboard handler...\n");
	index = sys_keyboard_catch(0x0, keyboard_handler, KEYBOARD_WILDCARD, "esc key");

	while (!_exit);

	sys_keyboard_clear_handler(index);

}
