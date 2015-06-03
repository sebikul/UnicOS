
#ifndef COMMANDS_H
#define COMMANDS_H

void command_echo(int argc, char** argv);

void command_help(int argc, char** argv, char** cmd_list, int cmd_count);

void command_set_distribution(int argc , char** argv) ;

void command_time(int argc, char** argv);
void command_settime(int hours, int minutes, int seconds);

void command_user_name(int argc, char** argv);
void command_host_name(int argc, char** argv);

void command_backcolor(int color);
void command_fontcolor(int color);
void command_color(int argc, char** argv);
void command_restart();

void command_exit();
void command_clean();

#endif
