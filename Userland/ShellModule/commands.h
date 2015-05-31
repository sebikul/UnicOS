
#ifndef COMMANDS_H
#define COMMANDS_H 

void command_echo(int argc, char** argv);
void command_help(int argc, char** argv);
void command_time(int argc, char** argv);


void command_settime(int hours, int minutes, int seconds);
void command_backcolor(int color);
void command_fontcolor(int color);
void command_exit();
void command_clean();
void command_refresh();
void command_restart();
#endif