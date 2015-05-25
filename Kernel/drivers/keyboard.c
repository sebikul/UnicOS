#include <stdint.h>
#include <video.h>
#include <keyboard.h>

void keyboard_irq_handler(uint64_t s) {

	scancode t = keyboard_scancodes[s];


	if (t.ascii == NULL) {

		switch (t.scancode) {
		case 0x3a://right shift
			keyboard_status.caps = !keyboard_status.caps;
			break;
		}

	} else {
		if (keyboard_status.caps) {
			video_write_char(t.ascii - 'a' + 'A', 1);
		} else {
			video_write_char(t.ascii, 1);
		}

		video_update_cursor();
	}








}