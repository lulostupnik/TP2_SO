include ../Makefile.inc

MODULE=0000-shellCodeModule.bin
SOURCES=$(wildcard */*.c)
SOURCES_ASM=$(wildcard */*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=_loader.c
LOADEROBJECT=$(LOADERSRC:.c=.o)

all: $(MODULE)

$(MODULE):$(OBJECTS) $(OBJECTS_ASM) $(LOADEROBJECT)
	$(LD) $(LDFLAGS) -T shellCodeModule.ld -o ../$(MODULE) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(GCC) $(GCCFLAGS) $(LOADERSRC) -c -o $(LOADEROBJECT)


clean:
	rm -rf */*.o *.o *.bin

.PHONY: all clean print
