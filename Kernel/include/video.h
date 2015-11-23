#include <stdint.h>
#include <io.h>
#include <types.h>

#ifndef VIDEO_H
#define VIDEO_H

#define VIRTUAL_CONSOLES 7
#define KERNEL_CONSOLE 0

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define SCREEN_SIZE (SCREEN_WIDTH*SCREEN_HEIGHT)

typedef struct {
	int row;
	int column;
	uint16_t screen[SCREEN_SIZE];
	unsigned short cursor;
	color_t color;
} screen_t;

typedef uint8_t console_t;

void video_init();
void video_preinit() ;
screen_t* get_screen(console_t console);

color_t video_get_color(console_t console);
void video_set_full_color(console_t console, color_t color);
void video_set_color(console_t console, vga_color fg, vga_color bg);
void video_write_char_at(console_t console, const char c, int row, int col);
void video_clear_screen(console_t console);
void video_write_char(console_t console, const char c);
void video_write_string(console_t console, const char * s);
void video_write_nl(console_t console);
void video_write_line(console_t console, const char * s);
void video_update_cursor();
void video_clear_indexed_line(console_t console, int index);
void video_update_screen_color(console_t console);
void video_reset_cursor(console_t console);

void video_write_dec(console_t console, uint64_t value);
void video_write_hex(console_t console, uint64_t value);
void video_write_bin(console_t console, uint64_t value);
void video_write_base(console_t console, uint64_t value, uint32_t base);

bool screensaver_reset_timer();

void video_trigger_restore();
void video_trigger_screensaver();

void video_change_console_nosync(console_t console);
void video_change_console(console_t console);

console_t video_current_console();

#endif
