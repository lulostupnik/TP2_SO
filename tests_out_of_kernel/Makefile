EXEC = test_mm

SRC = mm.c test_mm.c syscall.c test_util.c

CC = gcc
CFLAGS = -Wall

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC)
