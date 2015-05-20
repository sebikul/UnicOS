include ../Makefile.inc

MODULE=shellModule.bin
SOURCES=$(wildcard *.c)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T shellModule.ld $(SOURCES) ../syscalls/syscalls.o -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
