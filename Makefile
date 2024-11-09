all: assemble simulate

assemble: assembler.c
	@gcc -o k2asm assembler.c
	@echo "Assembler built successfully."

simulate: simulator.c
	@gcc -o k2sim simulator.c
	@echo "Simulator built successfully."

clean:
	@rm -f k2asm k2sim *.bin
	@echo "Cleaned up all binaries."

help:
	@echo "Makefile targets:"
	@echo "  assemble - Build the assembler"
	@echo "  simulate - Build the simulator"
	@echo "  all - Build both programs"
	@echo "  clean - Remove compiled files"
	@echo "  help - Display this help"

