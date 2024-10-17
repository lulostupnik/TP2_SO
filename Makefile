all: bootloader kernel userland sharedLibs image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

sharedLibs:
	cd SharedLibs; make all 

image: kernel bootloader userland sharedLibs
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean
	cd SharedLibs; make clean  

.PHONY: bootloader image collections kernel userland sharedLibs all clean


# all:  bootloader kernel userland image

# bootloader:
# 	cd Bootloader; make all

# kernel:
# 	cd Kernel; make all

# userland:
# 	cd Userland; make all

# image: kernel bootloader userland sharedLibs
# 	cd Image; make all

# clean:
# 	cd Bootloader; make clean
# 	cd Image; make clean
# 	cd Kernel; make clean
# 	cd Userland; make clean

# .PHONY: bootloader image collections kernel userland all clean
