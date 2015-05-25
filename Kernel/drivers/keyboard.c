#include <stdint.h>
#include <video.h>
#include <keyboard.h>

void keyboard_irq_handler(uint64_t scancode) {


	video_write_char(keyboard_scancodes[scancode].ascii);

	video_update_cursor();


}