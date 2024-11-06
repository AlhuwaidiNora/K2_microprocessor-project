# Makefile for K2 Microprocessor Project

CC = gcc
CFLAGS = -Wall -Wextra

all: assemble simulate

assemble: assembler.c
	$(CC) $(CFLAGS) -o k2asm assembler.c

simulate: simulator.c
	$(CC) $(CFLAGS) -o k2sim simulator.c

.PHONY: clean help

clean:
	rm -f k2asm k2sim *.o *.bin

help:
	@echo "K2 Microprocessor Project Makefile"
	@echo "Available targets:"
	@echo "  make all         - Build both assembler and simulator"
	@echo "  make assemble    - Build and run assembler (use FILENAME=file.asm)"
	@echo "  make simulate    - Build and run simulator (use FILENAME=file.bin)"
	@echo "  make clean       - Remove compiled files"
	@echo "  make help        - Show this help message"
	@echo
	@echo "Example usage:"
	@echo "  make assemble FILENAME=fibonacci.asm"
	@echo "  make simulate FILENAME=fibonacci.bin"
