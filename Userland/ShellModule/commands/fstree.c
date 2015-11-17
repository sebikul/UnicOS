#include <libc.h>
#include "commands.h"
#include "syscalls.h"

COMMAND_HELP(fstree, "[fstree] Lista el contenido del filesystem.\n");

COMMAND_START(fstree) {

	sys_atomic();
	sys_fstree();
	sys_unatomic();

	return 0;
}
