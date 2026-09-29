# C Photo Compressor — build file
# Will be filled in as the codec is implemented.

CC ?= cc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -O2

.PHONY: all clean

all:
	@echo "Scaffold ready. Implement the codec files in src/ before building."

clean:
	@rm -f cphotoc src/*.o
