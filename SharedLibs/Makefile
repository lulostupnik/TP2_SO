# SOURCES=$(wildcard libs/*.c)
# OBJECTS=$(SOURCES:.c=.o)

# all: libshared.a  # If it's a static library

# libshared.a: $(OBJECTS)
# 	$(AR) rcs libshared.a $(OBJECTS)

# libs/%.o: libs/%.c
# 	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

# clean:
# 	rm -rf libs/*.o libshared.a

# .PHONY: all clean

SOURCES=$(wildcard libs/*.c)
OBJECTS=$(SOURCES:.c=.o)

all: libshared.a  # Building the static library

libshared.a: $(OBJECTS)
	ar rcs libshared.a $(OBJECTS)

libs/%.o: libs/%.c
	# Directly specify the -I flag for include directory
	gcc -I./include -c $< -o $@

clean:
	rm -rf libs/*.o libshared.a

.PHONY: all clean
