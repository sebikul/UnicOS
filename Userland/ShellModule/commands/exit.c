#include <libc.h>
#include "commands.h"

COMMAND_HELP(exit, "[exit] Finaliza la ejecucion de la terminal.");

COMMAND_START(exit) {

	clear_screen();
	exit();

}
