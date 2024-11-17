# Compiler settings
CC=gcc
CFLAGS=-Wall -Wextra

# Targets
.PHONY: all clean help assemble simulate

all: k2asm k2sim

k2asm: assembler.c
	$(CC) $(CFLAGS) -o k2asm assimblyEdt.c

k2sim: simulator.c
	$(CC) $(CFLAGS) -o k2sim simulator.c

assemble: k2asm
	./k2asm $(FILENAME)

simulate: k2sim
	./k2sim $(FILENAME)

clean:
	rm -f k2asm k2sim *.bin

help:
	@echo "K2 Processor Project Makefile"
	@echo "Available targets:"
	@echo "  make all         - Build both assembler and simulator"
	@echo "  make assemble FILENAME=<file.asm>  - Run assembler on assembly file"
	@echo "  make simulate FILENAME=<file.bin>  - Run simulator on binary file"
	@echo "  make clean       - Remove compiled files"
	@echo "  make help        - Show this help message"
