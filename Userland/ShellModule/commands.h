
#ifndef COMMANDS_H
#define COMMANDS_H

#include "string.h"

typedef struct {
	char *name;
	char *help;
	task_entry_point func;
} command_t;

typedef struct {
	command_t **commands;
	uint64_t count;
} command_list_t;

#define DECLARE_COMMAND(command) void command_ ##command ##_init();\
void command_ ##command(int argc, char** argv);

#define COMMAND_HELP(command, _help) static const char* COMMAND_##command##_HELP = _help

#define COMMAND_START(command) \
void command_ ##command ##_init() {\
	extern command_list_t *cmdlist;\
	cmdlist->commands[cmdlist->count] = (command_t*) malloc(sizeof(command_t)); \
	cmdlist->commands[cmdlist->count]->func =  (task_entry_point) command_ ##command;\
	cmdlist->commands[cmdlist->count]->name = (char*) malloc(strlen(#command)+1);\
	cmdlist->commands[cmdlist->count]->help = (char*) malloc(strlen(COMMAND_##command##_HELP)+1);\
	strcpy(cmdlist->commands[cmdlist->count]->name, #command);\
	strcpy(cmdlist->commands[cmdlist->count]->help, COMMAND_##command##_HELP);\
	cmdlist->count++;\
}\
void command_ ##command(int argc, char** argv)

#define COMMAND_END() }

#define COMMAND_INIT(command) command_ ##command ##_init()

#define COMMAND_RUN(command, argc, argv) command_ ##command(argc, argv)

DECLARE_COMMAND(echo);
DECLARE_COMMAND(set_distribution);
DECLARE_COMMAND(time);
DECLARE_COMMAND(user_name);
DECLARE_COMMAND(host_name);
DECLARE_COMMAND(color);
DECLARE_COMMAND(refresh);
DECLARE_COMMAND(screensaver);
DECLARE_COMMAND(exit);
DECLARE_COMMAND(clear);
DECLARE_COMMAND(rawkbd);
DECLARE_COMMAND(help);

#endif
