include ../Makefile.variables
include Makefile.inc

KERNEL=kernel.bin
SOURCES=drivers/keyboard.c $(wildcard *.c) drivers/video.c drivers/rtc.c
OBJECTS=$(SOURCES:.c=.o)

LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)

IODRIVERSRC=drivers/io.asm
IODRIVEROBJECT=$(IODRIVERSRC:.asm=.o)

STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(IODRIVEROBJECT) $(OBJECTS) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(IODRIVEROBJECT) $(OBJECTS) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -DMACOS=$(MACOS) -I./include -I../headers -c $< -o $@

$(LOADEROBJECT):
	$(ASM) -felf64 $(LOADERSRC) -o $(LOADEROBJECT)

$(IODRIVEROBJECT):
	$(ASM) -felf64 $(IODRIVERSRC) -o $(IODRIVEROBJECT)

clean:
	rm -rf *.o *.bin
	rm -rf drivers/*.o 

.PHONY: all clean
