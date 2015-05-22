include ../../Makefile.variables
include ../Makefile.inc

all: syscall_loader syscalls link

syscalls:
	$(GCC) $(GCCFLAGS) -c syscalls.c

syscall_loader:
	$(ASM) $(ASMFLAGS) syscall_loader.asm -o syscall_loader.o

link:
	$(AR) rcs lib/libsyscalls.a syscall_loader.o syscalls.o

clean:
	rm -rf *.o lib/*.a

.PHONY: all clean
