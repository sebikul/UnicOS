
#include <syscalls.h>
#include <video.h>

void sys_write(int fd, char* s, int len) {

	uint16_t colorbk;

	video_write_line("Llamando a sys_write.");

	video_indent_line();
	video_write_string("File Descriptor: ");
	video_write_dec((uint64_t)fd);
	video_write_nl();


	video_indent_line();
	video_write_string(" String address: 0x");
	video_write_hex((uint64_t)s);
	video_write_nl();

	video_indent_line();
	video_write_string("String: ");
	video_write_string(s);
	video_write_nl();

	video_indent_line();
	video_write_string("Length: ");
	video_write_dec((uint64_t)len);
	video_write_nl();

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

void sys_read(int fd, char* s, int len) {

}