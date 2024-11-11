include ../Makefile.inc

MODULE=0000-shellCodeModule
SOURCES=$(wildcard */*.c) $(wildcard ../../Shared/libs/*.c)

SOURCES_ASM=$(wildcard */*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=_loader.c
LOADEROBJECT=$(LOADERSRC:.c=.o)

INCLUDE_DIRS=$(shell find include -type d) $(shell find ../../Shared/include -type d)
INCLUDE_FLAGS=$(addprefix -I,$(INCLUDE_DIRS))

all: $(MODULE)

$(MODULE):$(OBJECTS) $(OBJECTS_ASM) $(LOADEROBJECT)
	$(LD) $(LDFLAGS) -T shellCodeModule.ld -z noexecstack -o ../$(MODULE).bin $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T shellCodeModule.ld -z noexecstack --oformat=elf64-x86-64 -o ../$(MODULE).elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)

%.o: %.c
	$(GCC) $(GCCFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(GCC) $(GCCFLAGS) $(LOADERSRC) -c -o $(LOADEROBJECT)


clean:
	rm -rf */*.o *.o *.bin 
	rm -rf *.elf

.PHONY: all clean print