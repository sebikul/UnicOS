include ../../Makefile.variables
include ../Makefile.inc

MODULE=shellCodeModule.bin
SOURCES=shellModule.c $(wildcard commands/*.c)

HEADERS=-I../../headers -I../libc/include -I./
LIBRARIES=-L../syscalls/lib -L../libc/lib

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T shellModule.ld $(SOURCES) $(HEADERS) $(LIBRARIES) -lc -lsyscalls ../../common/string.c -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
