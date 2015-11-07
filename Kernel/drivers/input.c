#include "msgqueue.h"
#include "keyboard.h"
#include "video.h"
#include "kernel.h"

static volatile msgqueue_t* input_events[VIRTUAL_CONSOLES];
static volatile msgqueue_t* input_focus;

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

	kdebug("Encolando caracter en cola de input: ");
	kdebug_char(c);
	kdebug_nl();

	msgqueue_add(input_focus, &c, sizeof(char));

	if (c == '\n') {
		task_t *foreground_task = task_get_foreground(video_current_console());
		task_ready(foreground_task);
	}
}

char input_getc() {
	char *c = msgqueue_deq(input_focus);
	char rc = *c;
	free(c);

	kdebug("Desencolando caracter en cola de input: ");
	kdebug_char(rc);
	kdebug_nl();

	return rc;
}

void input_undo() {
	msgqueue_undo(input_focus);
}

void input_clear() {
	msgqueue_clear(input_focus);
}

void input_replace(const char* s) {

	kdebug("Reemplazando cola de input con: ");
	_kdebug(s);
	kdebug_nl();

	input_clear();

	while (*s != 0) {
		input_add(*s);
		s++;
	}
}

int input_size() {
	return msgqueue_size(input_focus);
}
