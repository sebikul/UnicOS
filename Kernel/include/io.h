#include <stdint.h>

#ifndef IO_H
#define IO_H

void outb(uint16_t port, uint8_t data);

uint8_t inb(uint16_t port);

#endif
