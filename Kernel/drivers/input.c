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
	msgqueue_add(input_focus, &scancode, sizeof(char));
}

char input_get() {
	char *c = msgqueue_deq(input_focus);
	return *c;
}

void input_clear(){
	msgqueue_clear(input_focus);
}