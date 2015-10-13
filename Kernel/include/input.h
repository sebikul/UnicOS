#ifndef INPUT_H
#define INPUT_H 

void input_init();
void input_change_console(console_t console);
void input_add(uint64_t scancode);
void input_undo();
void input_clear();
char input_getc();
void input_replace(const char* s);
int input_size();
void input_waitforline() ;

#endif
