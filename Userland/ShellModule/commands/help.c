#include <libc.h>
#include "string.h"
#include "commands.h"

extern command_list_t *cmdlist;

COMMAND_HELP(help, "[help] Muestra en pantalla la lista de comandos disponibles.\n"
             "Si se ejecuta help [comando] muestra el funcionamiento del comando elegido.\n");

COMMAND_START(help) {

	sys_atomic();

	cmdlist = malloc(sizeof(command_list_t));
	cmdlist->count = 0;
	cmdlist->commands = malloc(COMMANDS_LIST_SIZE * sizeof(command_t));

	COMMAND_INIT(help);
	COMMAND_INIT(echo);
	COMMAND_INIT(set_distribution);
	COMMAND_INIT(time);
	COMMAND_INIT(user_name);
	COMMAND_INIT(host_name);
	COMMAND_INIT(color);
	COMMAND_INIT(refresh);
	COMMAND_INIT(screensaver);
	COMMAND_INIT(exit);
	COMMAND_INIT(clear);
	COMMAND_INIT(rawkbd);
	COMMAND_INIT(ps);
	COMMAND_INIT(sigsend);
	COMMAND_INIT(sigrcv);
	COMMAND_INIT(cat);
	COMMAND_INIT(ls);
	COMMAND_INIT(filesend);
	COMMAND_INIT(filercv);
	COMMAND_INIT(mkdir);
	COMMAND_INIT(edit);
	COMMAND_INIT(mtest);
	COMMAND_INIT(shmget);
	COMMAND_INIT(shmwrite);
	COMMAND_INIT(shmread);
	COMMAND_INIT(shmctl);
	COMMAND_INIT(shmstatus);
	COMMAND_INIT(shmfree);
	COMMAND_INIT(semget);
	COMMAND_INIT(semwait);
	COMMAND_INIT(semsig);
	COMMAND_INIT(semstatus);
	COMMAND_INIT(ipcs);
	COMMAND_INIT(producer);
	COMMAND_INIT(consumer);

	sys_unatomic();

	if (argc == 1) {

		for (int cmd = 0; cmd < cmdlist->count; cmd++) {
			printf("\t%s\n", cmdlist->commands[cmd]->name);
		}

		printf("\nEjecute 'help [comando]' para obtener mas ayuda.\n");
	} else {

		for (int cmd = 0; cmd < cmdlist->count; cmd++) {
			if (strcmp(cmdlist->commands[cmd]->name, argv[1]) == 0) {
				printf("\t%s\n", cmdlist->commands[cmd]->help);
				break;
			}
		}
	}

	return 0;
}
