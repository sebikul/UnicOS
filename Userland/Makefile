include ../Makefile.variables
include Makefile.inc

all: syscalls libc shellCodeModule shellDataModule

shellCodeModule:
	cd ShellModule; make

shellDataModule:
	printf "This is sample data." >> shellDataModule.bin && dd if=/dev/zero bs=1 count=1 >> shellDataModule.bin

libc:
	cd libc; make

clean:
	cd ShellModule; make clean
	cd syscalls; make clean
	cd libc; make clean
	rm -rf *.bin

syscalls:
	cd syscalls; make

.PHONY: syscalls libc shellCodeModule all clean
