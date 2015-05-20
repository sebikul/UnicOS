include Makefile.inc

all: syscalls shellCodeModule shellDataModule

shellCodeModule:
	cd ShellModule; make

shellDataModule:
	printf "This is sample data." >> shellModule.bin && dd if=/dev/zero bs=1 count=1 >> shellModule.bin

clean:
	cd ShellModule; make clean
	rm -rf *.bin

syscalls:
	cd syscalls; make

.PHONY: shellCodeModule all clean
