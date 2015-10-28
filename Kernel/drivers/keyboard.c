//TODO: fix caps-shift for SO

#include <stdint.h>
#include "video.h"
#include "keyboard.h"
#include "mem.h"
#include "types.h"
#include "msgqueue.h"
#include "input.h"
#include "kernel.h"

#define FIRST_BIT_ON(c) (0x80 | c)

typedef struct {
	char scancode;
	char ascii;
	char caps;
} scancode_t;

scancode_t keyboard_scancodes[][256] = {
	{	//USA
		{0x00 , NOCHAR, NOCHAR}, //empty,
		{0x01 , NOCHAR, NOCHAR}, //esc
		{0x02 , '1', '!'},
		{0x03 , '2', '@'},
		{0x04 , '3', '#'},
		{0x05 , '4', '$'},
		{0x06 , '5', '%'},
		{0x07 , '6', '^'},
		{0x08 , '7', '&'},
		{0x09 , '8', '*'},
		{0x0A , '9', '('},
		{0x0B , '0', '"'},
		{0x0C , '-', '_'},
		{0x0D , '=', '+'},
		{0x0E , NOCHAR, NOCHAR}, //backspace
		{0x0F , NOCHAR, NOCHAR}, //tab
		{0x10 , 'q', 'Q'},
		{0x11, 'w', 'W'},
		{0x12, 'e', 'E'},
		{0x13, 'r', 'R'},
		{0x14, 't', 'T'},
		{0x15, 'y', 'Y'},
		{0x16, 'u', 'U'},
		{0x17, 'i', 'I'},
		{0x18, 'o', 'O'},
		{0x19, 'p', 'P'},
		{0x1a, '[', '{'},
		{0x1b, ']', '}'},
		{0x1c, '\n', '\n'},//enter
		{0x1d, NOCHAR, NOCHAR},//left ctrl
		{0x1e, 'a', 'A'},
		{0x1f, 's', 'S'},
		{0x20, 'd', 'D'},
		{0x21, 'f', 'F'},
		{0x22, 'g', 'G'},
		{0x23, 'h', 'H'},
		{0x24, 'j', 'J'},
		{0x25, 'k', 'K'},
		{0x26, 'l', 'L'},
		{0x27, ';', ':'},
		{0x28, '\'', '"'},
		{0x29, '`', '~'},
		{0x2a, NOCHAR, NOCHAR},//left shift
		{0x2b, '\\', '|'},
		{0x2c, 'z', 'Z'},
		{0x2d, 'x', 'X'},
		{0x2e, 'c', 'C'},
		{0x2f, 'v', 'V'},
		{0x30, 'b', 'B'},
		{0x31, 'n', 'N'},
		{0x32, 'm', 'M'},
		{0x33, ',', '<'},
		{0x34, '.', '>'},
		{0x35, '/', '?'},
		{0x36, NOCHAR, NOCHAR},//right shift
		{0x37, '*', NOCHAR},//keypad *
		{0x38, NOCHAR, NOCHAR},//left alt
		{0x39, ' ', ' '},
		{0x3a, NOCHAR, NOCHAR},//caps
		{0x3b, NOCHAR, NOCHAR},//f1
		{0x3c, NOCHAR, NOCHAR},//f2
		{0x3d, NOCHAR, NOCHAR},//f3
		{0x3e, NOCHAR, NOCHAR},//f4
		{0x3f, NOCHAR, NOCHAR},//f5
		{0x40, NOCHAR, NOCHAR},//f6
		{0x41, NOCHAR, NOCHAR},//f7
		{0x42, NOCHAR, NOCHAR},//f8
		{0x43, NOCHAR, NOCHAR},//f9
		{0x44, NOCHAR, NOCHAR},//f10
		{0x45, NOCHAR, NOCHAR},//num
		{0x46, NOCHAR, NOCHAR},//scroll
		{0x47, '7', NOCHAR},//keypad 7
		{0x48, '8', NOCHAR},//keypad 8
		{0x49, '9', NOCHAR},//keypad 9
		{0x4a, '-', NOCHAR},//keypad -
		{0x4b, NOCHAR, NOCHAR},//keypad 4
		{0x4c, '5', NOCHAR},//keypad 5
		{0x4d, NOCHAR, NOCHAR},//keypad 6
		{0x4e, '+', NOCHAR},//keypad +
		{0x4f, '1', NOCHAR},//keypad 1
		{0x50, '2', NOCHAR},//keypad 2
		{0x51, '3', NOCHAR},//keypad 3
		{0x52, '0', NOCHAR},//keypad 0
		{0x53, '.', NOCHAR},//keypad 0
		{0x57, NOCHAR, NOCHAR},//f11
		{0x58, NOCHAR, NOCHAR}//f12
	},
	{	//LATIN
		{0x00 , NOCHAR, NOCHAR}, //empty,
		{0x01 , NOCHAR, NOCHAR}, //esc
		{0x02 , '1', '!'},
		{0x03 , '2', '"'},
		{0x04 , '3', '#'},
		{0x05 , '4', '$'},
		{0x06 , '5', '%'},
		{0x07 , '6', '&'},
		{0x08 , '7', '/'},
		{0x09 , '8', '('},
		{0x0A , '9', ')'},
		{0x0B , '0', '='},
		{0x0C , '-', '?'},
		{0x0D , NOCHAR, NOCHAR},
		{0x0E , NOCHAR, NOCHAR}, //backspace
		{0x0F , NOCHAR, NOCHAR}, //tab
		{0x10 , 'q', 'Q'},
		{0x11, 'w', 'W'},
		{0x12, 'e', 'E'},
		{0x13, 'r', 'R'},
		{0x14, 't', 'T'},
		{0x15, 'y', 'Y'},
		{0x16, 'u', 'U'},
		{0x17, 'i', 'I'},
		{0x18, 'o', 'O'},
		{0x19, 'p', 'P'},
		{0x1a, NOCHAR, NOCHAR},
		{0x1b, '+', '*'},
		{0x1c, '\n', '\n'},//enter
		{0x1d, NOCHAR, NOCHAR},//left ctrl
		{0x1e, 'a', 'A'},
		{0x1f, 's', 'S'},
		{0x20, 'd', 'D'},
		{0x21, 'f', 'F'},
		{0x22, 'g', 'G'},
		{0x23, 'h', 'H'},
		{0x24, 'j', 'J'},
		{0x25, 'k', 'K'},
		{0x26, 'l', 'L'},
		{0x27, ';', ':'},
		{0x28, '\'', '"'},
		{0x29, '|', NOCHAR},
		{0x2a, NOCHAR, NOCHAR},//left shift
		{0x2b, '}', ']'},
		{0x2c, 'z', 'Z'},
		{0x2d, 'x', 'X'},
		{0x2e, 'c', 'C'},
		{0x2f, 'v', 'V'},
		{0x30, 'b', 'B'},
		{0x31, 'n', 'N'},
		{0x32, 'm', 'M'},
		{0x33, ',', ';'},
		{0x34, '.', ':'},
		{0x35, '-', '_'},
		{0x36, NOCHAR, NOCHAR},//right shift
		{0x37, '*', NOCHAR},//keypad *
		{0x38, NOCHAR, NOCHAR},//left alt
		{0x39, ' ', ' '},
		{0x3a, NOCHAR, NOCHAR},//caps
		{0x3b, NOCHAR, NOCHAR},//f1
		{0x3c, NOCHAR, NOCHAR},//f2
		{0x3d, NOCHAR, NOCHAR},//f3
		{0x3e, NOCHAR, NOCHAR},//f4
		{0x3f, NOCHAR, NOCHAR},//f5
		{0x40, NOCHAR, NOCHAR},//f6
		{0x41, NOCHAR, NOCHAR},//f7
		{0x42, NOCHAR, NOCHAR},//f8
		{0x43, NOCHAR, NOCHAR},//f9
		{0x44, NOCHAR, NOCHAR},//f10
		{0x45, NOCHAR, NOCHAR},//num
		{0x46, NOCHAR, NOCHAR},//scroll
		{0x47, '7', NOCHAR},//keypad 7
		{0x48, '8', NOCHAR},//keypad 8
		{0x49, '9', NOCHAR},//keypad 9
		{0x4a, '-', NOCHAR},//keypad -
		{0x4b, '4', NOCHAR},//keypad 4
		{0x4c, '5', NOCHAR},//keypad 5
		{0x4d, '6', NOCHAR},//keypad 6
		{0x4e, '+', NOCHAR},//keypad +
		{0x4f, '1', NOCHAR},//keypad 1
		{0x50, '2', NOCHAR},//keypad 2
		{0x51, '3', NOCHAR},//keypad 3
		{0x52, '0', NOCHAR},//keypad 0
		{0x53, NOCHAR, NOCHAR}, //keypad 0
		{0x54, '.', NOCHAR},//keypad 0
		{0x55, '.', NOCHAR},//keypad 0
		{0x56, '<', '>'},//keypad 0

		{0x57, NOCHAR, NOCHAR},//f11
		{0x58, NOCHAR, NOCHAR}//f12
	}
};

static keyboard_distrib keyboard_distribution = KEYBOARD_USA;

static kstatus keyboard_status = {.caps = FALSE, .ctrl = FALSE, .alt =  FALSE};

static msgqueue_t* kbdqueue;

static dka_catch* dka_catched_scancodes[256] = {NULL};
static int dka_catched_len = 0;


static void keyboard_caps_handler(uint64_t s) {
	keyboard_status.caps = !keyboard_status.caps;
}

static void keyboard_backspace_handler(uint64_t s) {

	if (input_size() == 0) {
		kdebug("Cola vacia. No se desencola nada.\n");
		return;
	}

	screen_t *screen = get_screen(video_current_console());

	if (screen->column == 0) {
		screen->column = SCREEN_WIDTH - 1;
		screen->row--;
	} else {
		screen->column--;
	}

	input_undo();

	video_write_char_at(video_current_console(), ' ', screen->row, screen->column);

	video_update_cursor();
}

static inline bool keyboard_is_scode_in_range(uint64_t range, uint64_t scode) {

	uint32_t low, high;

	// kdebug("Full rango: 0x");
	// kdebug_base(range, 16);
	// _kdebug("\t");

	low = (uint32_t) range;
	high = (uint32_t) (range >> 32);

	// kdebug("Analizando rango: 0x");
	// kdebug_base(low, 16);
	// _kdebug(" <= ");
	// kdebug_base(scode, 16);
	// _kdebug(" <= 0x");
	// kdebug_base(high, 16);

	// if (low <= scode && scode <= high) {
	// 	_kdebug("\t TRUE");
	// } else {
	// 	_kdebug("\t FALSE");
	// }

	// kdebug_nl();

	return (low <= scode && scode <= high);
}

static bool keyboard_run_handlers(uint64_t scode) {

	if (dka_catched_len > 0) {

		bool catched = FALSE;

		for (int i = 0; i < dka_catched_len; i++) {

			if (dka_catched_scancodes[i] == NULL) {
				continue;
			}

			bool is_wildcard = (dka_catched_scancodes[i]->flags & KEYBOARD_WILDCARD);
			bool is_range = (dka_catched_scancodes[i]->flags & KEYBOARD_RANGE);
			bool is_all_consoles = (dka_catched_scancodes[i]->flags & KEYBOARD_ALLCONSOLES);
			bool is_console_equal = (video_current_console() == dka_catched_scancodes[i]->console);

			if (!is_wildcard) {
				if (is_range) {
					if (!keyboard_is_scode_in_range(dka_catched_scancodes[i]->scancode, scode)) {
						continue;
					}
				} else {
					if (dka_catched_scancodes[i]->scancode != scode) {
						continue;
					}
				}
			}

			if (!is_all_consoles && !is_console_equal) {
				continue;
			}

			kdebug("Ejecutando handler en consola: ");
			kdebug_base(video_current_console(), 10);
			kdebug_nl();

			dka_catched_scancodes[i]->handler(scode);

			if (dka_catched_scancodes[i]->flags & KEYBOARD_IGNORE) {
				continue;
			} else {
				catched = TRUE;
			}
		}

		if (catched) {
			return TRUE;
		}
	}

	return FALSE;
}

static volatile bool running = FALSE;

static void keyboard_dispatch() {

	uint64_t *scancode;
	uint64_t res = 0;
	int reps;
	char c;

	if (running) {
		return;
	}

	running = TRUE;

	if (msgqueue_size(kbdqueue) == 0) {
		running = FALSE;
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
	free(scancode);

	for (int i = 0; i < reps; i++) {
		scancode = msgqueue_deq(kbdqueue);

		kdebug("Leido scancode: 0x");
		kdebug_base(*scancode, 16);
		kdebug_nl();

		res = (res << 8 ) | (*scancode);
		free(scancode);
	}

	if (keyboard_run_handlers(res)) {
		kdebug("Scancode atrapado por un handler\n");
		running = FALSE;
		return;
	}

	scancode_t t = keyboard_scancodes[keyboard_distribution][res];

	if (t.ascii == NOCHAR) {
		//kdebug("Recibido caracter NOCHAR\n");
		running = FALSE;
		return;
	}

	if (keyboard_status.caps) {
		c = t.caps;
	} else {
		c = t.ascii;
	}

	input_add(c);

	video_write_char(video_current_console(), c);
	video_update_cursor();

	running = FALSE;
}

void keyboard_init() {
	kbdqueue = msgqueue_create(KEYBOARD_BUFFER_SIZE);

	keyboard_catch(0x3A, keyboard_caps_handler, 0, 0, KEYBOARD_ALLCONSOLES);
	keyboard_catch(0x2A, keyboard_caps_handler, 0, 0, KEYBOARD_ALLCONSOLES);
	keyboard_catch(0x36, keyboard_caps_handler, 0, 0, 0);
	keyboard_catch(FIRST_BIT_ON(0x2A), keyboard_caps_handler, 0, 0, KEYBOARD_ALLCONSOLES);
	keyboard_catch(FIRST_BIT_ON(0x36), keyboard_caps_handler, 0, 0, KEYBOARD_ALLCONSOLES);

	keyboard_catch(0x0E, keyboard_backspace_handler, 0, 0, KEYBOARD_ALLCONSOLES);
}

void keyboard_irq_handler(uint64_t s) {

	//kdebug("IRQ del teclado\n");

	msgqueue_add(kbdqueue, &s, sizeof(uint64_t));

	keyboard_dispatch();
}

int keyboard_catch(uint64_t scancode, dka_handler handler, console_t console, pid_t pid, uint64_t flags) {

	int index;

	dka_catch* tmp = (dka_catch*)calloc(sizeof(dka_catch));

	tmp->scancode = scancode;
	tmp->handler = handler;
	tmp->pid = pid;
	tmp->console = console;
	tmp->flags = flags;

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
