
#include <syscalls.h>
#include <video.h>

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

void sys_read(int fd, char* s, int len) {

}