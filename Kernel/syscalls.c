
#include <syscalls.h>
#include <video.h>

#include <keyboard.h>

#define EOF 	-1

void sys_time(int *hours, int* minutes, int* seconds){
	
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

	//video_write_line("Esperando al buffer: ");

	read = keyboard_wait_for_buffer(len);

	//video_write_line("Procesando buffer del teclado...");

	while (i < read) {
		s[i] = keyboard_get_char_from_buffer();

		// video_write_string("Leyendo caracter del buffer: ");
		// video_write_char(s[i]);
		// video_write_string(" en la posicion: ");
		// video_write_dec((uint64_t)i);
		// video_write_string(" / ");
		// video_write_dec((uint64_t)read);
		// video_write_nl();

		i++;

	}

	s[i] = 0;

	// for (int i = 0 int pos = keyboard_wpos; i < read; i++, keyboard_wpos++) {

	// 	pos = (keyboard_wpos + i) % KEYBOARD_BUFFER_SIZE;

	// 	s[i] = keyboard_kbuffer[pos];

	// }



	// video_write_string("Cadena ingresada: ");
	// video_write_string(s);
	// video_write_nl();

	return read;

	// while (read < len && !eof) {

	// 	while (keyboard_rpos == keyboard_wpos && keyboard_buffer_loop == FALSE) {

	// 	}

	// 	c = keyboard_kbuffer[keyboard_rpos];

	// 	if (c == '\n') {

	// 		eof = TRUE;

	// 	} else {

	// 		s[read] = c;


	// 		read++;

	// 	}

	// 	keyboard_rpos++;

	// 	if (keyboard_rpos == KEYBOARD_BUFFER_SIZE - 1) {
	// 		keyboard_buffer_loop = FALSE;
	// 		keyboard_rpos = 0;
	// 	}

	// }

	// if (eof) {
	// 	return EOF; //ojo con esto, sys read SIEMPRE deberia retornar la cantidad de caracteres que leyo,
	// 	//sin importar la condicion de corte eso deberia analizarlo el receptor del buffer.
	// }

	// return read;
}

