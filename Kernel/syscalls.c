
#include <syscalls.h>
#include <video.h>

void sys_write(int fd, char* s, int len) {

	video_write_line("Llamando a sys_write.");

	if (fd == FD_WRITE) {
		video_write_line(s);
	}

}

void sys_read(int fd, char* s, int len) {

}