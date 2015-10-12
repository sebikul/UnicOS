//TODO: fix caps-shift for SO

#include <stdint.h>
#include "video.h"
#include "keyboard.h"
#include "mem.h"
#include "types.h"
#include "msgqueue.h"

#define FIRST_BIT_ON(c) (0x80 | c)

extern bool screensaver_is_active;

static keyboard_distrib keyboard_distribution = KEYBOARD_USA;

static bool read_eof = FALSE;
static kstatus keyboard_status = {FALSE,//caps
                                  FALSE,//ctrl
                                  FALSE//alt
                                 };

static bool screensaver_enter_flag = FALSE;

static msgqueue_t* kbdqueue;

static dka_catch* dka_catched_scancodes[256] = {NULL};
static int dka_catched_len = 0;

static void keyboard_buffer_delete() {

	if (msgqueue_size(kbdqueue) == 0) {
		return;
	}

	screen_t *screen = get_screen(task_get_current()->console);

	if (screen->column == 0) {
		screen->column = SCREEN_WIDTH - 1;
		screen->row--;
	} else {
		screen->column--;
	}

	msgqueue_undo(msgqueue);

	video_write_char_at(KERNEL_CONSOLE, ' ', screen->row, screen->column);

	video_update_cursor();
}

static void keyboard_caps_handler(uint64_t s) {
	keyboard_status.caps = !keyboard_status.caps;
}

static void keyboard_backspace(uint64_t s) {
	keyboard_buffer_delete();
}

void keyboard_init() {
	kbdqueue = msgqueue_create(KEYBOARD_BUFFER_SIZE);

	keyboard_catch(0x3A, keyboard_caps_handler, 0, 0);
	keyboard_catch(0x2A, keyboard_caps_handler, 0, 0);
	keyboard_catch(0x36, keyboard_caps_handler, 0, 0);
	keyboard_catch(FIRST_BIT_ON(0x2A), keyboard_caps_handler, 0, 0);
	keyboard_catch(FIRST_BIT_ON(0x36), keyboard_caps_handler, 0, 0);

	keyboard_catch(0x0E, keyboard_backspace, 0, 0);
}

void keyboard_replace_buffer(char* s) {

	keyboard_written = 0;

	while (*s != 0) {
		keyboard_buffer_write(*s);
		s++;
	}
}

int keyboard_wait_for_buffer(int len) {

	keyboard_written = 0;
	read_eof = FALSE;

	while (keyboard_written < len && !read_eof) ;

	return keyboard_written;
}

char keyboard_get_char_from_buffer() {

	char ret = keyboard_kbuffer[keyboard_wpos];

	keyboard_wpos++;

	keyboard_wpos = keyboard_wpos % KEYBOARD_BUFFER_SIZE;

	return ret;
}

static void keyboard_write_char(char c) {

	if (keyboard_buffer_write(c)) {
		video_write_char(KERNEL_CONSOLE, c);
	}
}

void keyboard_irq_handler(uint64_t s) {

	if (!screensaver_enter_flag && screensaver_reset_timer()) {
		screensaver_enter_flag = FALSE;
		return;
	}

	msgqueue_add(kbdqueue, &s, sizeof(uint64_t));

	keyboard_dispatch();
}

void keyboard_dispatch() {

	uint64_t *scancode;
	uint64_t res = 0;
	int reps;
	char c;
	static volatile bool running = FALSE;

	if (running) {
		return;
	}

	running = TRUE;

	if (msgqueue_size(kbdqueue) == 0) {
		return;
	}

	scancode = msgqueue_deq(kbdqueue);

	switch (*scancode) {
	case 0xE0:
		reps = 1;
		break;

	case 0xE1:
		reps = 2;
		break;

	default:
		reps = 0;
		break;
	}

	res = *scancode;

	for (int i = 0; i < reps; i++) {
		res << sizeof(char);
		scancode = msgqueue_deq(kbdqueue);
		res |= (*scancode);
	}

	if (dka_catched_len > 0) {

		bool catched = FALSE;

		for (int i = 0; i < dka_catched_len; i++) {
			if (dka_catched_scancodes[i]->wildcard || dka_catched_scancodes[i]->scancode == s) {

				dka_catched_scancodes[i]->handler(s);
				catched = TRUE;
			}
		}

		if (catched) {
			running = FALSE;
			return;
		}
	}

	scancode t = keyboard_scancodes[keyboard_distribution][s];

	if (keyboard_status.caps) {
		c = t.caps;
	} else {
		c = t.ascii;
	}

	input_add(c);

	running = FALSE;
}

int keyboard_catch(uint64_t scancode, dka_handler handler, unsigned int console, pid_t pid) {

	int index;

	dka_catch* tmp = (dka_catch*)calloc(sizeof(dka_catch));

	tmp->scancode = scancode;
	tmp->handler = handler;
	tmp->pid = pid;
	tmp->console = console;
	tmp->wildcard = (scancode == 0);

	index = dka_catched_len;
	dka_catched_scancodes[index] = tmp;
	dka_catched_len++;

	return index;
}

void keyboard_clear_handler(int index) {
	free(dka_catched_scancodes[index]);
	dka_catched_scancodes[index] = NULL;
}

void keyboard_set_distribution(keyboard_distrib d) {
	keyboard_distribution = d;
}
