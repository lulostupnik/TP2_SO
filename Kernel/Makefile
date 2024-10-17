
# # # include Makefile.inc
# # # KERNEL=kernel.bin
# # # SOURCES=$(wildcard *.c)
# # # SOURCES_ASM=$(wildcard asm/*.asm)
# # # SOURCES_DRIVERS=$(wildcard Drivers/*.c)
# # # SOURCES_INTERRUPTS=$(wildcard interrupts/*.c)
# # # SOURCES_LIBS=$(wildcard libs/*.c) 
# # # SOURCES_TESTS=$(wildcard tests/*.c)  
# # # SOURCES_ADTS=$(wildcard ADTs/*.c)
# # # OBJECTS=$(SOURCES:.c=.o)
# # # OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
# # # OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
# # # OBJECTS_INTERRUPTS=$(SOURCES_INTERRUPTS:.c=.o)
# # # OBJECTS_LIBS=$(SOURCES_LIBS:.c=.o)  
# # # OBJECTS_TESTS=$(SOURCES_TESTS:.c=.o)
# # # OBJECTS_ADTS=$(SOURCES_ADTS:.c=.o)
# # # LOADERSRC=loader.asm

# # # LOADEROBJECT=$(LOADERSRC:.asm=.o)

# # # SHAREDLIBS_DIR=../SharedLibs
# # # SHAREDLIBS=$(SHAREDLIBS_DIR)/libshared.a  


# # # INCLUDE_DIRS=$(shell find include -type d)
# # # INCLUDE_FLAGS=$(addprefix -I,$(INCLUDE_DIRS))

# # # all: $(KERNEL)

# # # $(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS)
# # # 	$(LD) $(LDFLAGS) -T kernel.ld -z noexecstack -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS)
# # # 	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -z noexecstack -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS) # Debug

# # # %.o: %.c
# # # 	$(GCC) $(GCCFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# # # %.o : %.asm
# # # 	$(ASM) $(ASMFLAGS) $< -o $@

# # # $(LOADEROBJECT):
# # # 	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

# # # clean:
# # # 	rm -rf asm/*.o *.o *.bin Drivers/*.o interrupts/*.o libs/*.o tests/*.o ADTs/*.o
# # # 	rm -rf *.elf

# # # .PHONY: all clean

# # include Makefile.inc
# # KERNEL=kernel.bin
# # SOURCES=$(wildcard *.c)
# # SOURCES_ASM=$(wildcard asm/*.asm)
# # SOURCES_DRIVERS=$(wildcard Drivers/*.c)
# # SOURCES_INTERRUPTS=$(wildcard interrupts/*.c)
# # SOURCES_LIBS=$(wildcard libs/*.c)
# # SOURCES_TESTS=$(wildcard tests/*.c)
# # SOURCES_ADTS=$(wildcard ADTs/*.c)
# # OBJECTS=$(SOURCES:.c=.o)
# # OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
# # OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
# # OBJECTS_INTERRUPTS=$(SOURCES_INTERRUPTS:.c=.o)
# # OBJECTS_LIBS=$(SOURCES_LIBS:.c=.o)
# # OBJECTS_TESTS=$(OBJECTS_TESTS:.c=.o)
# # OBJECTS_ADTS=$(OBJECTS_ADTS:.c=.o)
# # LOADERSRC=loader.asm
# # LOADEROBJECT=$(LOADERSRC:.asm=.o)

# # # Path to SharedLibs and its include folder
# # SHAREDLIBS_DIR=../SharedLibs
# # SHAREDLIBS=$(SHAREDLIBS_DIR)/libshared.a
# # SHAREDLIBS_INCLUDE=$(SHAREDLIBS_DIR)/include

# # # Include SharedLibs' include folder in the include flags
# # INCLUDE_DIRS=$(shell find include -type d)
# # INCLUDE_FLAGS=$(addprefix -I,$(INCLUDE_DIRS)) -I$(SHAREDLIBS_INCLUDE)

# # all: $(KERNEL)

# # $(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS)
# # 	$(LD) $(LDFLAGS) -T kernel.ld -z noexecstack -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS)
# # 	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -z noexecstack -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS) # Debug

# # %.o: %.c
# # 	$(GCC) $(GCCFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# # %.o : %.asm
# # 	$(ASM) $(ASMFLAGS) $< -o $@

# # $(LOADEROBJECT):
# # 	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

# # clean:
# # 	rm -rf asm/*.o *.o *.bin Drivers/*.o interrupts/*.o libs/*.o tests/*.o ADTs/*.o
# # 	rm -rf *.elf

# # .PHONY: all clean


# include Makefile.inc
# KERNEL=kernel.bin
# SOURCES=$(wildcard *.c)
# SOURCES_ASM=$(wildcard asm/*.asm)
# SOURCES_DRIVERS=$(wildcard Drivers/*.c)
# SOURCES_INTERRUPTS=$(wildcard interrupts/*.c)
# SOURCES_LIBS=$(wildcard libs/*.c)
# SOURCES_TESTS=$(wildcard tests/*.c)
# SOURCES_ADTS=$(wildcard ADTs/*.c)
# OBJECTS=$(SOURCES:.c=.o)
# OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
# OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
# OBJECTS_INTERRUPTS=$(SOURCES_INTERRUPTS:.c=.o)
# OBJECTS_LIBS=$(SOURCES_LIBS:.c=.o)
# OBJECTS_TESTS=$(SOURCES_TESTS:.c=.o)  # Fixed recursive issue
# OBJECTS_ADTS=$(SOURCES_ADTS:.c=.o)
# LOADERSRC=loader.asm

# LOADEROBJECT=$(LOADERSRC:.asm=.o)

# # Path to SharedLibs
# SHAREDLIBS_DIR=../SharedLibs
# SHAREDLIBS=$(SHAREDLIBS_DIR)/libshared.a
# SHAREDLIBS_INCLUDE=$(SHAREDLIBS_DIR)/include

# # Include directories
# INCLUDE_DIRS=$(shell find include -type d)
# INCLUDE_FLAGS=$(addprefix -I,$(INCLUDE_DIRS)) -I$(SHAREDLIBS_INCLUDE)

# all: $(KERNEL)

# $(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS)
# 	$(LD) $(LDFLAGS) -T kernel.ld -z noexecstack -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS)
# 	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -z noexecstack -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS) # Debug

# %.o: %.c
# 	$(GCC) $(GCCFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# %.o : %.asm
# 	$(ASM) $(ASMFLAGS) $< -o $@

# $(LOADEROBJECT):
# 	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

# clean:
# 	rm -rf asm/*.o *.o *.bin Drivers/*.o interrupts/*.o libs/*.o tests/*.o ADTs/*.o
# 	rm -rf *.elf

# .PHONY: all clean

include Makefile.inc
KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_DRIVERS=$(wildcard Drivers/*.c)
SOURCES_INTERRUPTS=$(wildcard interrupts/*.c)
SOURCES_LIBS=$(wildcard libs/*.c)
SOURCES_TESTS=$(wildcard tests/*.c)
SOURCES_ADTS=$(wildcard ADTs/*.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm:.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c:.o)
OBJECTS_INTERRUPTS=$(SOURCES_INTERRUPTS:.c:.o)
OBJECTS_LIBS=$(SOURCES_LIBS:.c:.o)
OBJECTS_TESTS=$(SOURCES_TESTS:.c:.o)
OBJECTS_ADTS=$(SOURCES_ADTS:.c:.o)
LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm:.o)

# Paths to SharedLibs and the static library
SHAREDLIBS_DIR=../SharedLibs
SHAREDLIBS_LIB=$(SHAREDLIBS_DIR)/libshared.a  # Adjust the library name if it's libc.a
SHAREDLIBS_INCLUDE=$(SHAREDLIBS_DIR)/include  # Include directory

# Include flags
INCLUDE_FLAGS = -Iinclude -I../SharedLibs/include

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS) $(SHAREDLIBS_LIB)
	$(LD) $(LDFLAGS) -T kernel.ld -z noexecstack -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS) $(SHAREDLIBS_LIB)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -z noexecstack -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_INTERRUPTS) $(OBJECTS_LIBS) $(OBJECTS_TESTS) $(OBJECTS_ADTS) $(SHAREDLIBS_LIB) # Debug

%.o: %.c
	$(GCC) $(GCCFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin Drivers/*.o interrupts/*.o libs/*.o tests/*.o ADTs/*.o
	rm -rf *.elf

.PHONY: all clean
