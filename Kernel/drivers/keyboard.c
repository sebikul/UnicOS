#include <stdint.h>
#include <video.h>
#include <keyboard.h>

#define FIRST_BITE_ON(c) (0x80 | c)

extern int video_column;

void keyboard_irq_handler(uint64_t s) {

	scancode t = keyboard_scancodes[s];


	if (t.ascii == NULL) {

		switch (s) {
		case 0x3a://caps
			keyboard_status.caps = !keyboard_status.caps;
			break;

		case 0x1c: //enter
			video_write_nl();
			video_write_prompt();
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
			video_write_char(t.ascii - 'a' + 'A');
		} else {
			video_write_char(t.ascii);
		}

		if (video_column == 0) {
			video_indent_line();
		}

		video_update_cursor();

	}








}