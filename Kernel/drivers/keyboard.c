#include <stdint.h>
#include <video.h>
#include <keyboard.h>

extern int video_column;

void keyboard_irq_handler(uint64_t s) {

	scancode t = keyboard_scancodes[s];


	if (t.ascii == NULL) {

		switch (t.scancode) {
		case 0x3a://right shift
			keyboard_status.caps = !keyboard_status.caps;
			break;

		case 0x1c: //enter
			video_write_nl();
			video_write_prompt();
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