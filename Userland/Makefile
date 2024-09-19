include Makefile.inc

shell_DATA=0001-shellDataModule.bin

all: shellCodeModule shellDataModule

shellCodeModule:
	cd shellCodeModule; make

shellDataModule:
	printf "This is shell data." >> $(shell_DATA) && dd if=/dev/zero bs=1 count=1 >> $(shell_DATA)

clean:
	cd shellCodeModule; make clean
	rm -rf *.bin


.PHONY: shellCodeModule all clean
