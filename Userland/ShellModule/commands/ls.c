#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(ls, "[ls] Lista el contenido del filesystem.\n");

COMMAND_START(ls) {

	sys_atomic();
	sys_ls();
	sys_unatomic();

	return 0;
}
