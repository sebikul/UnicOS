#include <libc.h>
#include <types.h>
#include <syscalls.h>

static void usage();

void command_set_distribution(int argc , char** argv) {
	if (argc != 3) {
		fprintf(FD_STDERR, "Comando invalido.\n");
		usage();
	}

	if (strcmp(argv[1], "set") == 0) {

		if (strcmp(argv[2], "latin") == 0) {
			sys_kbd_set_distribution(KEYBOARD_LATIN);
		} else if (strcmp(argv[2], "usa") == 0) {
			sys_kbd_set_distribution(KEYBOARD_USA);
		} else {
			usage();
		}

	} else {
		usage();
	}
}

static void usage() {
	printf(" keyboard: Setea la distribucion del teclado.\n"
	       "$ keyboard set [usa|latin]");
}