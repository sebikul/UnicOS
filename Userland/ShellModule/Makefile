include ../Makefile.inc

MODULE=shellModule.bin
SOURCES=$(wildcard *.c)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T shellModule.ld $(SOURCES) -I../syscalls/include -L../syscalls/lib  -lsyscalls -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
