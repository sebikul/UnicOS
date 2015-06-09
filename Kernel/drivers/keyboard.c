#include <stdint.h>
#include <video.h>
#include <keyboard.h>
#include <lib.h>
#include <types.h>

#define FIRST_BITE_ON(c) (0x80 | c)

extern int video_column;
extern int video_row;

extern bool screensaver_is_active;

char keyboard_kbuffer[KEYBOARD_BUFFER_SIZE] = {0};

int keyboard_wpos = 0;
int keyboard_written = 0;

static keyboard_distrib keyboard_distribution = KEYBOARD_USA;

static bool read_eof = FALSE;

static kstatus keyboard_status = {FALSE,//caps
                                  FALSE,//ctrl
                                  FALSE//alt
                                 };
#define NOCHAR (char)0

static bool screensaver_enter_flag = FALSE;

scancode keyboard_scancodes[][256] = {
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
		{0x1c, NOCHAR, NOCHAR},//enter
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
		{0x1c, NOCHAR, NOCHAR},//enter
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

static dka_catch* dka_catched_scancodes[256] = {NULL};

static int dka_catched_len = 0;


static bool keyboard_buffer_write(char c) {

	int pos = (keyboard_wpos + keyboard_written) % KEYBOARD_BUFFER_SIZE;

	if (pos == keyboard_wpos && keyboard_written != 0) {
		//buffer lleno
		return FALSE;
	}

	keyboard_kbuffer[pos] = c;
	keyboard_written++;

	return TRUE;
}

static void keyboard_buffer_delete() {


	if (keyboard_written == 0) {
		return;
	}

	if (video_column == 0) {
		video_column = SCREEN_WIDTH - 1;
		video_row--;
	} else {
		video_column--;
	}

	keyboard_written--;

	video_write_char_at(' ', video_row, video_column);

	video_update_cursor();


}

void keyboard_replace_last_written(char* s) {

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
		video_write_char(c);
	}

}

void keyboard_irq_handler(uint64_t s) {


	if (!screensaver_enter_flag && screensaver_reset_timer()) {
		screensaver_enter_flag = FALSE;
		return;
	}


	if (dka_catched_len > 0) {

		bool catched = FALSE;

		for (int i = 0; i < dka_catched_len; i++) {
			if (dka_catched_scancodes[i]->scancode == s) {

				dka_catched_scancodes[i]->handler(s);
				catched = TRUE;

			}
		}

		if (catched) {
			return;
		}
	}

	scancode t = keyboard_scancodes[keyboard_distribution][s];

	if (t.ascii == NOCHAR) {

		switch (s) {
		case 0x3a://caps
			keyboard_status.caps = !keyboard_status.caps;
			break;

		case FIRST_BITE_ON(0x1c):
			if (screensaver_enter_flag) {
				screensaver_enter_flag = FALSE;
				return;
			}

			break;

		case 0x1c: //enter
			read_eof = TRUE;
			screensaver_enter_flag = TRUE;
			break;

		case 0x0E://backspace
			keyboard_buffer_delete();
			break;

		case 0x2a:
		case 0x36://shift
			keyboard_status.caps = !keyboard_status.caps;
			break;

		case FIRST_BITE_ON(0x2a):
		case FIRST_BITE_ON(0x36)://shift
			keyboard_status.caps = !keyboard_status.caps;
			break;

		}

	} else {

		if (keyboard_status.caps) {
			keyboard_write_char(t.caps);
		} else {
			keyboard_write_char(t.ascii);
		}



		video_update_cursor();

	}

}

void keyboard_catch(uint64_t scancode, dka_handler handler) {

	dka_catch* tmp = (dka_catch*)calloc(sizeof(dka_catch));

	tmp->scancode = scancode;
	tmp->handler = handler;

	dka_catched_scancodes[dka_catched_len++] = tmp;



}

void keyboard_set_distribution(keyboard_distrib d) {
	keyboard_distribution = d;
}
