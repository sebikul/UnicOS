
#include <syscalls.h>
#include <video.h>
#include <keyboard.h>
#include <rtc.h>
#include <lib.h>

#define EOF 	-1

void sys_rtc_time(time_t* t) {

	rtc_get_time(t);

}

void sys_write(FD fd, char* s, int len) {

	uint16_t colorbk;

	switch (fd) {
	case FD_STDOUT:
		video_write_string(s);
		break;

	case FD_STDERR:

		colorbk = video_get_color();

		video_set_color(COLOR_RED, COLOR_BLACK);

		video_write_string(s);

		video_set_full_color(colorbk);
		break;


	}

}
//aseguramos que len no sea mayor al tamaño maximo del buffer
//aseguramos el buen manejo del flag de loop

int sys_read(FD fd, char* s, int len) {

	int read = 0;

	int i = 0;

	read = keyboard_wait_for_buffer(len);

	while (i < read) {
		s[i] = keyboard_get_char_from_buffer();

		i++;

	}

	s[i] = 0;

	return read;

}

void* sys_malloc(int len) {

	return malloc(len);

}

void* sys_calloc(int len) {

	return calloc(len);

}

void sys_free(void* m) {
	free(m);
}

void sys_keyboard_catch(uint64_t scancode, dka_handler handler) {
	keyboard_catch(scancode, handler);
}



