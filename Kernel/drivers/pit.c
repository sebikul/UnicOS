#include "io.h"

// Inicializa el timer y establece el período entre
// interrupciones en milisegundos.
// Codigo extraido de mtask version 24
void pit_setup(unsigned msecs) {
	uint32_t count = 1193182 * msecs;
	count /= 1000;

	outb(0x43, 0x36);
	outb(0x40, count);
	outb(0x40, count >> 8);
}
