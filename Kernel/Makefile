include ../Makefile.variables
include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c) drivers/video.c drivers/keyboard.c
OBJECTS=$(SOURCES:.c=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -DMACOS=$(MACOS) -I./include -c $< -o $@

$(LOADEROBJECT):
	$(ASM) -felf64 $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf *.o *.bin

.PHONY: all clean
