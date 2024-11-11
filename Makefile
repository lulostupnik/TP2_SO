MM=FROM_SCRATCH

all: toolchain bootloader kernel userland image

toolchain:
	cd Toolchain; make all
bootloader:
	cd Bootloader; make all
kernel:
	cd Kernel; make all MM=-D$(MM)
userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

buddy: MM=BUDDY
buddy: toolchain bootloader kernel userland image

clean:
	cd Toolchain; make clean
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: toolchain bootloader image kernel userland all clean buddy
