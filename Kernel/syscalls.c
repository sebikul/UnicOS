
#include <syscalls.h>
#include <video.h>

#include <keyboard.h>

extern char keyboard_kbuffer[KEYBOARD_BUFFER_SIZE];
extern int keyboard_rpos;
extern int keyboard_wpos;
extern bool keyboard_buffer_loop;

void sys_write(int fd, char* s, int len) {

	uint16_t colorbk;

	switch (fd) {
	case FD_STDOUT:
		video_write_line(s);
		break;

	case FD_STDERR:

		colorbk = video_get_color();

		video_set_color(COLOR_RED, COLOR_BLACK);

		video_write_line(s);

		video_set_full_color(colorbk);
		break;


	}



}


int sys_read(int fd, char* s, int len) {

	int read = 0;
	bool eof = FALSE;

	char c;

	while (read < len && !eof) {

		video_write_string("Leyendo linea. ");
		video_write_string("Caracter: ");
		video_write_dec((uint64_t)read);
		video_write_string(" / ");
		video_write_dec((uint64_t)len);
		video_write_nl();


		while (keyboard_rpos == keyboard_wpos && keyboard_buffer_loop == FALSE) {

		}

		c = keyboard_kbuffer[keyboard_rpos];

		video_write_string("Leido caracter : ");
		video_write_char(c);
		video_write_nl();

		if (c == '\n') {
			eof = TRUE;

		} else {

			s[read] = c;


			read++;

		}

		keyboard_rpos++;


	}

	video_write_string("Caracteres leidos: ");
	video_write_dec((uint64_t)read);
	video_write_string(" Estado buffer: ");
	video_write_string(s);
	video_write_nl();

//buffer del teclado!!!!! lo recibe por parametro!
//buffer circular, dos punteros, r y w
//si r==w quedo esperando datos en un while 1

	return read;
}

