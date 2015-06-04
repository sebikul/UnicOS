
#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>
#include <io.h>
#include <types.h>

#define SCREEN_START ((uint16_t*)0xB8000)

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

typedef struct {
	int row;
	int column;
	uint16_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];
	color_t color;
} screen_t;

void video_initialize();
void video_reset_color();
color_t video_get_color();
void video_set_full_color(color_t color);
void video_set_color(vga_color fg, vga_color bg);
uint16_t video_get_full_char_at(int row, int col);
void video_write_full_char_at(uint16_t c, int row, int col);
void video_write_char_at(const char c, int row, int col);
void video_clear_screen();
void video_write_char(const char c);
void video_write_string(const char * s);
void video_write_nl();
void video_write_line(const char * s);
void video_scroll();
void video_update_cursor();
void video_clear_indexed_line(int index);
void video_update_screen_color();

void video_write_dec(uint64_t value);
void video_write_hex(uint64_t value);
void video_write_bin(uint64_t value);
void video_write_base(uint64_t value, uint32_t base);

void screensaver_reset_timer();

void video_trigger_backup();
void video_trigger_restore();
void video_trigger_screensaver();

#endif
