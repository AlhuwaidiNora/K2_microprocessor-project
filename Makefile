all: assemble simulate

assemble: assembler.c
	gcc -o k2asm assembler.c

simulate: simulator.c
	gcc -o k2sim simulator.c

clean:
	rm -f k2asm k2sim *.bin

help:
	@echo "Makefile targets:"
	@echo "  assemble  - Build the assembler"
	@echo "  simulate  - Build the simulator"
	@echo "  all       - Build both programs"
	@echo "  clean     - Remove compiled files"
	@echo "  help      - Display this help"
	@echo ""
	@echo "Usage:"
	@echo "  make assemble FILENAME=program.asm"
	@echo "  make simulate FILENAME=program.bin"

