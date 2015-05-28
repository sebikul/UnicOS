
#ifndef HCOMMANDS_H
#define HCOMMANDS_H 

void command_echo(char * in);
void command_help();
void command_time();
void command_settime(int hours, int minutes, int seconds);
void command_backcolor(int color);
void command_fontcolor(int color);
void command_exit();
void command_clean();
void command_refresh();
#endif