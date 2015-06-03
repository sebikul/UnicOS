
#include <video.h>
#include <io.h>
#include <syscalls.h>

int video_row = 0;
int video_column = 0;
static char buffer[128] = { 0 };

static color_t current_color = 0;

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);


void video_initialize() {
	video_reset_color();


//preguntar por que es necesario, y la variable no esta inicializada
	video_column = 0;
	video_row = 0;

}

//todo static
void video_reset_color() {
	current_color = build_color_value(COLOR_WHITE, COLOR_BLACK);
}

void video_set_color(vga_color fg, vga_color bg) {
	current_color = build_color_value(fg, bg);
}

void video_set_full_color(color_t color) {
	current_color = color;
}

color_t video_get_color() {
	return current_color;
}

/**
 *	Devuelve 8 bites, formato bgfg
 */
color_t build_color_value(vga_color fg, vga_color bg) {

	return  (bg << 4) | fg;

}

uint16_t video_get_full_char_at(int row, int col) {

	return SCREEN_START[row * SCREEN_WIDTH + col];

}

void video_write_full_char_at(uint16_t c, int row, int col) {

	SCREEN_START[row * SCREEN_WIDTH + col] = c;

}

//todo static
void video_clear_screen() {

	uint8_t colorbk = current_color;

	video_reset_color();

	for (int i = 0; i < SCREEN_HEIGHT; i++) {

		for (int j = 0; j < SCREEN_WIDTH; j++) {
			video_write_char(' ');
		}

		//cursor[i*2]=' ';
		//cursor[i*2+1]=0; //TOASK: default?

	}

	video_row = 0;
	video_column = 0;

	current_color = colorbk;

}

static void video_clear_line(int row) {

	for (int i = 0; i < SCREEN_WIDTH; i++) {
		video_write_char_at(' ', row, i);
	}

	video_column = 0;
}

void video_clear_indexed_line(int index) {
	video_clear_line(video_row + index);
}

/*//todo static
void video_write_char_with_color(const char c, vga_color fg, vga_color bg) {

	uint8_t color = build_color_value(fg, bg);

	uint16_t scrpos = c | color << 8;

	video_write_full_char(scrpos);

}*/

static void video_write_full_char(uint16_t c) {

	video_write_full_char_at(c, video_row, video_column);

	video_column++;

	if (video_column == SCREEN_WIDTH) {
		video_column = 0;
		video_row++;
	}

	if (video_row == SCREEN_HEIGHT) {
		video_scroll();
	}


}

void video_write_char_at(const char c, int row, int col) {

	//para evitar que se trunquen los valores haciendo toda la operacion en una linea,
	//se necesitan guardar los valores en uint16_t
	uint16_t c_16 = c;
	uint16_t color_16 = current_color;

	video_write_full_char_at(c_16 | (color_16 << 8), row, col);
}

//todo static
void video_write_char(const char c) {

	//para evitar que se trunquen los valores haciendo toda la operacion en una linea,
	//se necesitan guardar los valores en uint16_t
	uint16_t c_16 = c;
	uint16_t color_16 = current_color;

	video_write_full_char(c_16 | (color_16 << 8));
}

//todo static
void video_write_string(const char * s) {

	while (*s != 0) {

		switch (*s) {
		case '\n':
			video_write_nl();
			//video_write_prompt();
			break;

		case '\t':
			video_write_string("    ");
			break;

		default:
			video_write_char(*s);
			break;
		}

		s++;

		// if (video_column == 0) {
		// 	video_indent_line();
		// }

	}

	video_update_cursor();

}

//todo static
void video_write_nl() {

	int line_start = (video_column == 0);

	while (video_column != 0 || line_start) {

		video_write_char(' ');
		line_start = 0;
	}

}

// void video_indent_line() {
// 	video_write_string(" >  ");
// }

void video_write_line(const char * s) {

	//video_write_prompt();
	//
	if (video_column != 0) {
		video_write_nl();
	}

	video_write_string(s);

	video_write_nl();

}

// void video_write_prompt() {

// 	if (video_column != 0) {
// 		video_write_nl();
// 	}

// 	video_write_string(" root# ");
// }

// void video_write_pline(const char * s) {

// 	if (video_column != 0) {
// 		video_write_nl();
// 	}

// 	video_write_string(s);

// 	video_write_nl();

// }


void video_scroll() {

	for (int row = 1; row <= SCREEN_HEIGHT; row++) {

		for (int column = 0; column < SCREEN_WIDTH; column++) {

			uint16_t c = video_get_full_char_at(row, column);

			video_write_full_char_at(c, row - 1, column);

		}

	}

	video_column = 0;
	video_row--;

}


void video_update_cursor() {

	unsigned short position = (video_row * 80) + video_column;

	// cursor LOW port to vga INDEX register
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(position & 0xFF));
	// cursor HIGH port to vga INDEX register
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char )((position >> 8) & 0xFF));

}

void video_write_dec(uint64_t value)
{
	video_write_base(value, 10);
}

void video_write_hex(uint64_t value)
{
	video_write_base(value, 16);
}

void video_write_bin(uint64_t value)
{
	video_write_base(value, 2);
}

void video_write_base(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	video_write_string(buffer);
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
