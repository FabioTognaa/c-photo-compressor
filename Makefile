# C Photo Compressor — build file

CC ?= cc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -O2 -Ithird-party

SRC ?= src/main.c src/stb_impl.c src/colorspace.c
BIN ?= main

.PHONY: all clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(BIN) src/*.o
