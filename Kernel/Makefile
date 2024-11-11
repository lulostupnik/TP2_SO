
include Makefile.inc
KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_DRIVERS=$(wildcard Drivers/*.c)
SOURCES_INTERRUPTS=$(wildcard interrupts/*.c)
SOURCES_LIBS=$(wildcard libs/*.c) 
SOURCES_TESTS=$(wildcard tests/*.c)  
SOURCES_ADTS=$(wildcard ADTs/*.c)
SOURCES_SHARED=$(wildcard ../Shared/libs/*.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_SHARED=$(SOURCES_SHARED:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_INTERRUPTS=$(SOURCES_INTERRUPTS:.c=.o)
OBJECTS_LIBS=$(SOURCES_LIBS:.c=.o)  
OBJECTS_TESTS=$(SOURCES_TESTS:.c=.o)
OBJECTS_ADTS=$(SOURCES_ADTS:.c=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

INCLUDE_DIRS=$(shell find include -type d) $(shell find ../Shared/include -type d)
INCLUDE_FLAGS=$(addprefix -I,$(INCLUDE_DIRS))



all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS) $(OBJECTS_SHARED)
	$(LD) $(LDFLAGS) -T kernel.ld -z noexecstack -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS) $(OBJECTS_SHARED)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -z noexecstack -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS) $(OBJECTS_SHARED) # Debug

%.o: %.c
	$(GCC) $(GCCFLAGS) $(INCLUDE_FLAGS) $(MM) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin Drivers/*.o interrupts/*.o libs/*.o tests/*.o ADTs/*.o
	@if [ -e ../Shared/libs/*.o ]; then rm -rf ../Shared/libs/*.o; fi
	rm -rf *.elf

.PHONY: all clean