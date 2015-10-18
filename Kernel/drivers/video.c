#include <video.h>
#include <io.h>
#include <types.h>
#include "string.h"
#include "kernel.h"
#include "input.h"
#include "keyboard.h"

static char buffer[128] = { 0 };

static screen_t consoles[VIRTUAL_CONSOLES + 1] = {{0}};
static uint8_t current_console = 0;

static uint8_t screensaver_backup;

static uint16_t* screen_mem = (uint16_t*)0xB8000;

static uint16_t video_get_full_char_at(console_t console, int row, int col);
static void video_write_full_char_at(console_t console, uint16_t c, int row, int col);
static void video_scroll(console_t console);

screen_t* get_screen(console_t console) {
	return &consoles[console];
}

static inline void video_sync_console_at(console_t console, int row, int col) {

	screen_t *screen = get_screen(console);

	screen_mem[row * SCREEN_WIDTH + col] = screen->screen[row * SCREEN_WIDTH + col];
}

static inline void video_sync_console(console_t console) {

	screen_t *screen = get_screen(console);

	for (int i = 0; i < SCREEN_HEIGHT; i++) {

		for (int j = 0; j < SCREEN_WIDTH; j++) {
			screen_mem[i * SCREEN_WIDTH + j] = screen->screen[i * SCREEN_WIDTH + j];
		}
	}
}

static void video_fn_handler(uint64_t s) {
	if (0x3b <= s && s <= 0x41) {
		int newconsole = s - 0x3b;

		kdebug("New virtual console: ");
		kdebug_base(newconsole, 10);
		kdebug_nl();

		video_change_console(newconsole);
		input_change_console(newconsole);
	}
}

void video_init() {

	for (int i = 0; i < VIRTUAL_CONSOLES; i++) {

		screen_t *screen = &consoles[i];

		screen->color = BUILD_COLOR(COLOR_WHITE, COLOR_BLACK);

		screen->row = 0;
		screen->column = 0;

		video_clear_screen(i);
	}

// //SCREENSAVER

// 	video_set_color(VIRTUAL_CONSOLES, COLOR_LIGHT_BROWN, COLOR_RED);

// 	video_write_string(VIRTUAL_CONSOLES,
// 	                   "                  ___                                                           "
// 	                   "      ___        /  /\\                                                          "
// 	                   "     /  /\\      /  /::\\                                                         "
// 	                   "    /  /:/     /  /:/\\:\\                                                        "
// 	                   "   /  /:/     /  /:/~/:/                                                        "
// 	                   "  /  /::\\    /__/:/ /:/                                                         "
// 	                   " /__/:/\\:\\   \\  \\:\\/:/                                                          "
// 	                   " \\__\\/  \\:\\   \\  \\::/                                                           "
// 	                   "      \\  \\:\\   \\  \\:\\                                                           "
// 	                   "       \\__\\/    \\  \\:\\                                                          "
// 	                   "                 \\__\\/                                                          "
// 	                   "      ___           ___                         ___                             "
// 	                   "     /  /\\         /  /\\          ___          /__/\\        ___                 "
// 	                   "    /  /::\\       /  /::\\        /  /\\         \\  \\:\\      /  /\\                "
// 	                   "   /  /:/\\:\\     /  /:/\\:\\      /  /::\\         \\  \\:\\    /  /:/                "
// 	                   "  /  /:/~/::\\   /  /:/~/:/     /  /:/\\:\\    ___  \\  \\:\\  /__/::\\                "
// 	                   " /__/:/ /:/\\:\\ /__/:/ /:/___  /  /:/~/::\\  /__/\\  \\__\\:\\ \\__\\/\\:\\__             "
// 	                   " \\  \\:\\/:/__\\/ \\  \\:\\/:::::/ /__/:/ /:/\\:\\ \\  \\:\\ /  /:/    \\  \\:\\/\\            "
// 	                   "  \\  \\::/       \\  \\::/~~~~  \\  \\:\\/:/__\\/  \\  \\:\\  /:/      \\__\\::/            "
// 	                   "   \\  \\:\\        \\  \\:\\       \\  \\::/        \\  \\:\\/:/       /__/:/             "
// 	                   "    \\  \\:\\        \\  \\:\\       \\__\\/          \\  \\::/        \\__\\/              "
// 	                   "     \\__\\/         \\__\\/                       \\__\\/                            "
// 	                   "                                                                                "
// 	                   "                                                                                "
// 	                  );

//	video_update_screen_color(VIRTUAL_CONSOLES);

	video_change_console(0);

	keyboard_catch(0x00, video_fn_handler, TRUE, 0, 0);


}

void video_update_screen_color(console_t console) {

	screen_t *screen = get_screen(console);

	for (int c = 1; c < SCREEN_HEIGHT * SCREEN_WIDTH * 2; c += 2) {
		((uint8_t*)screen->screen)[c] = screen->color;
	}
}

void video_set_color(console_t console, vga_color fg, vga_color bg) {
	get_screen(console)->color = BUILD_COLOR(fg, bg);
}

void video_set_full_color(console_t console, color_t color) {
	get_screen(console)->color = color;
}

color_t video_get_color(console_t console) {
	return get_screen(console)->color;
}

static inline uint16_t video_get_full_char_at(console_t console, int row, int col) {
	return get_screen(console)->screen[row * SCREEN_WIDTH + col];
}

static inline void video_write_full_char_at(console_t console, uint16_t c, int row, int col) {
	get_screen(console)->screen[row * SCREEN_WIDTH + col] = c;

	video_sync_console_at(console, row, col);
}

void video_clear_screen(console_t console) {

	screen_t *screen = get_screen(console);

	for (int i = 0; i < SCREEN_HEIGHT; i++) {

		for (int j = 0; j < SCREEN_WIDTH; j++) {
			video_write_char(console, ' ');
		}
	}

	screen->row = 0;
	screen->column = 0;

	video_update_screen_color(console);
}

static void video_clear_line(console_t console, int row) {

	for (int i = 0; i < SCREEN_WIDTH; i++) {
		video_write_char_at(console, ' ', row, i);
	}

	get_screen(console)->column = 0;
}

void video_clear_indexed_line(console_t console, int index) {
	video_clear_line(console, get_screen(console)->row + index);
}

static void video_write_full_char(console_t console, uint16_t c) {

	screen_t *screen = get_screen(console);

	video_write_full_char_at(console, c, screen->row, screen->column);

	screen->column++;

	if (screen->column == SCREEN_WIDTH) {
		screen->column = 0;
		screen->row++;
	}

	if (screen->row == SCREEN_HEIGHT) {
		video_scroll(console);
	}
}

void video_write_char_at(console_t console , const char c, int row, int col) {

	//para evitar que se trunquen los valores haciendo toda la operacion en una linea,
	//se necesitan guardar los valores en uint16_t
	uint16_t c_16 = c;
	uint16_t color_16 = get_screen(console)->color;

	video_write_full_char_at(console, c_16 | (color_16 << 8), row, col);
}

void video_write_char(console_t console, const char c) {

	//para evitar que se trunquen los valores haciendo toda la operacion en una linea,
	//se necesitan guardar los valores en uint16_t
	uint16_t c_16 = c;
	uint16_t color_16 = get_screen(console)->color;

	if (c == '\n') {
		video_write_nl(console);
	} else {
		video_write_full_char(console, c_16 | (color_16 << 8));
	}
}

void video_write_string(console_t console, const char * s) {

	while (*s != 0) {

		switch (*s) {
		case '\t':
			video_write_string(console, "    ");
			break;

		default:
			video_write_char(console, *s);
			break;
		}

		s++;
	}

	video_update_cursor();
}

void video_write_nl(console_t console) {

	screen_t *screen = get_screen(console);

	int line_start = (screen->column == 0);

	while (screen->column != 0 || line_start) {

		video_write_char(console, ' ');
		line_start = 0;
	}
}

void video_write_line(console_t console, const char * s) {

	if (get_screen(console)->column != 0) {
		video_write_nl(console);
	}

	video_write_string(console, s);

	video_write_nl(console);
}

static void video_scroll(console_t console) {

	screen_t *screen = get_screen(console);

	for (int row = 1; row <= SCREEN_HEIGHT; row++) {

		for (int column = 0; column < SCREEN_WIDTH; column++) {

			uint16_t c = video_get_full_char_at(console, row, column);

			video_write_full_char_at(console, c, row - 1, column);
		}
	}

	screen->column = 0;
	screen->row--;

	//video_update_screen_color(console);
}

void video_update_cursor() {

	screen_t *screen = get_screen(current_console);

	unsigned short position = (screen->row * 80) + screen->column;

	// cursor LOW port to vga INDEX register
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(position & 0xFF));
	// cursor HIGH port to vga INDEX register
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char )((position >> 8) & 0xFF));
}

void video_write_dec(console_t console, uint64_t value) {
	video_write_base(console, value, 10);
}

void video_write_hex(console_t console, uint64_t value) {
	video_write_base(console, value, 16);
}

void video_write_bin(console_t console, uint64_t value) {
	video_write_base(console, value, 2);
}

void video_write_base(console_t console, uint64_t value, uint32_t base) {
	uintToBase(value, buffer, base);
	video_write_string(console, buffer);
}


void video_change_console(uint8_t console) {

	screen_t *screen = &consoles[console];

	current_console = console;

	memcpy(screen_mem, screen->screen, 2 * SCREEN_SIZE);

	// for (int i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH); i++) {
	// 	screen_mem[i] = screen->screen[i];
	// }

	video_update_cursor();
}

void video_trigger_restore() {

	video_change_console(screensaver_backup);

}

void video_trigger_screensaver() {

	video_change_console(VIRTUAL_CONSOLES);

}
