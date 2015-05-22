include ../../Makefile.variables
include ../Makefile.inc

all: libc

libc:
	$(GCC) $(GCCFLAGS) -I../syscalls/include -c libc.c
	$(AR) rcs liblibc.a libc.o

clean:
	rm -rf *.o liblib*.a

.PHONY: all clean
