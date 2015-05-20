include ../Makefile.inc

LOADERSRC=syscall.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)

all: syscall syscalls

syscalls:
	$(GCC) $(GCCFLAGS) -I./include syscalls.c syscall.o

syscall:
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf *.o

.PHONY: all clean
