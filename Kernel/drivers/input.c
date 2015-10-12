#include "msgqueue.h"
#include "keyboard.h"
#include "video.h"

static msgqueue_t* input_events[VIRTUAL_CONSOLES];
static msgqueue_t* input_focus;

void input_init() {

	for (int i = 0; i < VIRTUAL_CONSOLES; i++) {
		input_events[i] = msgqueue_create(KEYBOARD_BUFFER_SIZE);
	}

	input_focus = input_events[0];
}

void input_change_console(console_t console) {
	input_focus = input_events[console];
}

void input_add(char c) {
	msgqueue_add(input_focus, &c, sizeof(char));
}

char input_getc() {
	char *c = msgqueue_deq(input_focus);
	return *c;
}

void input_undo(){
	msgqueue_undo(input_focus);
}

void input_clear(){
	msgqueue_clear(input_focus);
}

void input_replace(const char* s) {

	input_clear();

	while (*s != 0) {
		input_add(*s);
		s++;
	}
}

int input_size(){
	return msgqueue_size(input_focus);
}