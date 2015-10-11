include ../../Makefile.variables
include ../Makefile.inc

HEADERS=-I../../headers -I./include
LIBRARIES=-L../syscalls/lib

all: libc

libc:
	$(GCC) -Wall -Wextra $(GCCFLAGS) $(HEADERS) $(LIBRARIES) -lsyscalls -c libc.c ../../common/string.c
	$(AR) rcs lib/libc.a libc.o

clean:
	rm -rf *.o liblib*.a

.PHONY: all clean
