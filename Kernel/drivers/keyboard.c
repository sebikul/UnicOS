#include <stdint.h>
#include <video.h>
#include <keyboard.h>

#define FIRST_BITE_ON(c) (0x80 | c)

extern int video_column;
extern int video_row;

char keyboard_kbuffer[KEYBOARD_BUFFER_SIZE] = {0};

int keyboard_rpos = 0;
int keyboard_wpos = 0;

bool keyboard_buffer_loop = FALSE;


kstatus keyboard_status = {FALSE};

scancode keyboard_scancodes[256] = {
	{0x00 , NULL}, //empty,
	{0x01 , NULL}, //esc
	{0x02 , '1'},
	{0x03 , '2'},
	{0x04 , '3'},
	{0x05 , '4'},
	{0x06 , '5'},
	{0x07 , '6'},
	{0x08 , '7'},
	{0x09 , '8'},
	{0x0A , '9'},
	{0x0B , '0'},
	{0x0C , '-'},
	{0x0D , '='},
	{0x0E , NULL}, //backspace
	{0x0F , NULL}, //tab
	{0x10 , 'q'},
	{0x11, 'w'},
	{0x12, 'e'},
	{0x13, 'r'},
	{0x14, 't'},
	{0x15, 'y'},
	{0x16, 'u'},
	{0x17, 'i'},
	{0x18, 'o'},
	{0x19, 'p'},
	{0x1a, '['},
	{0x1b, ']'},
	{0x1c, NULL},//enter
	{0x1d, NULL},//left ctrl
	{0x1e, 'a'},
	{0x1f, 's'},
	{0x20, 'd'},
	{0x21, 'f'},
	{0x22, 'g'},
	{0x23, 'h'},
	{0x24, 'j'},
	{0x25, 'k'},
	{0x26, 'l'},
	{0x27, ';'},
	{0x28, '\''},
	{0x29, '`'},
	{0x2a, NULL},//left shift
	{0x2b, '\\'},
	{0x2c, 'z'},
	{0x2d, 'x'},
	{0x2e, 'c'},
	{0x2f, 'v'},
	{0x30, 'b'},
	{0x31, 'n'},
	{0x32, 'm'},
	{0x33, ','},
	{0x34, '.'},
	{0x35, '/'},
	{0x36, NULL},//right shift
	{0x37, '*'},//keypad *
	{0x38, NULL},//left alt
	{0x39, ' '},
	{0x3a, NULL},//caps
	{0x3b, NULL},//f1
	{0x3c, NULL},//f2
	{0x3d, NULL},//f3
	{0x3e, NULL},//f4
	{0x3f, NULL},//f5
	{0x40, NULL},//f6
	{0x41, NULL},//f7
	{0x42, NULL},//f8
	{0x43, NULL},//f9
	{0x44, NULL},//f10
	{0x45, NULL},//num
	{0x46, NULL},//scroll
	{0x47, '7'},//keypad 7
	{0x48, '8'},//keypad 8
	{0x49, '9'},//keypad 9
	{0x4a, '-'},//keypad -
	{0x4b, '4'},//keypad 4
	{0x4c, '5'},//keypad 5
	{0x4d, '6'},//keypad 6
	{0x4e, '+'},//keypad +
	{0x4f, '1'},//keypad 1
	{0x50, '2'},//keypad 2
	{0x51, '3'},//keypad 3
	{0x52, '0'},//keypad 0
	{0x53, '.'},//keypad 0
	{0x57, NULL},//f11
	{0x58, NULL}//f12


};
// extern char keyboard_kbuffer[KEYBOARD_BUFFER_SIZE];
// extern int keyboard_rpos;
// extern int keyboard_wpos;
// extern bool keyboard_buffer_loop;

static bool keyboard_buffer_write(char c) {

	if (!(keyboard_buffer_loop && keyboard_wpos == keyboard_rpos) || (!keyboard_buffer_loop && keyboard_rpos == 0 && keyboard_wpos == 0)) {
		keyboard_kbuffer[keyboard_wpos] = c;
		keyboard_wpos++;

		if (keyboard_wpos == KEYBOARD_BUFFER_SIZE - 1) {
			keyboard_buffer_loop = TRUE;
			keyboard_wpos = 0;
		} 

		return TRUE;

	} else {

		return FALSE;
	}



}

static void keyboard_write_char(char c) {

	if (keyboard_buffer_write(c)) {
		video_write_char(c);

		if (video_column == 0) {
			video_indent_line();
		}
	}

	// video_write_string("Buff rpos :");
	// video_write_dec((uint64_t)keyboard_rpos);
	// video_write_nl();

	// video_write_string("Buff wpos :");
	// video_write_dec((uint64_t)keyboard_wpos);
	// video_write_nl();

}

void keyboard_irq_handler(uint64_t s) {

	scancode t = keyboard_scancodes[s];

	if (t.ascii == NULL) {

		switch (s) {
		case 0x3a://caps
			keyboard_status.caps = !keyboard_status.caps;
			break;

		case 0x1c: //enter
			keyboard_buffer_write('\n');
			//video_write_nl();
			//video_write_prompt();
			break;

		case 0x2a:
		case 0x36:
			keyboard_status.caps = !keyboard_status.caps;
			break;

		case FIRST_BITE_ON(0x2a):
		case 0xb6:
			keyboard_status.caps = !keyboard_status.caps;
			break;

		}

	} else {

		if (keyboard_status.caps) {
			keyboard_write_char(t.ascii - 'a' + 'A');
		} else {
			keyboard_write_char(t.ascii);
		}



		video_update_cursor();

	}

}
