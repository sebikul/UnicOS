include ../../Makefile.variables
include ../Makefile.inc

MODULE=shellCodeModule.bin
SOURCES=shellModule.c echo.c help.c time.c

HEADERS=-I../../headers -I../libc/include -I./
LIBRARIES=-L../syscalls/lib -L../libc/lib

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T shellModule.ld $(SOURCES) $(HEADERS) $(LIBRARIES) -lc -lsyscalls  -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
