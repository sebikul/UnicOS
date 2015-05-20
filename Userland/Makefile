include Makefile.inc

all: shellCodeModule shellDataModule

shellCodeModule:
	cd ShellModule; make

shellDataModule:
	printf "This is sample data." >> ShellModule.bin && dd if=/dev/zero bs=1 count=1 >> ShellModule.bin

clean:
	cd ShellModule; make clean
	rm -rf *.bin


.PHONY: shellCodeModule all clean
