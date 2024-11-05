#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MEMORY_SIZE 256
#define MAX_INSTRUCTIONS 1000  // Limit to avoid infinite loop

unsigned char RA = 0, RB = 0, R0 = 0;
unsigned char PC = 0;
unsigned char carry = 0;
unsigned char memory[MEMORY_SIZE] = {0};

// Load program from a .bin file into memory and display it for debugging
void load_program(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }
    size_t bytes_read = fread(memory, sizeof(unsigned char), MEMORY_SIZE, file);
    fclose(file);
    printf("Loading binary file: %s\n", filename);
    printf("Program loaded (%zu bytes):\n", bytes_read);
    for (size_t i = 0; i < bytes_read; i++) {
        printf("%02X ", memory[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

// ALU function for addition and subtraction
unsigned char alu(unsigned char a, unsigned char b, int op, unsigned char *carry_out) {
    unsigned char result = 0;
    *carry_out = 0;

    if (op == 0) {  // Addition
        result = a + b;
        *carry_out = (result < a) ? 1 : 0;
    } else if (op == 1) {  // Subtraction
        result = a - b;
        *carry_out = (a < b) ? 1 : 0;
}

    return result;
}
/ Execute one instruction based on the opcode
bool execute_instruction(unsigned char opcode) {
    switch (opcode) {
        case 0x00:  // RA = RA + RB
            RA = alu(RA, RB, 0, &carry);
            break;
        case 0x01:  // RB = RA + RB
            RB = alu(RA, RB, 0, &carry);
            break;
        case 0x02:  // RA = RA - RB
            RA = alu(RA, RB, 1, &carry);
            break;
        case 0x03:  // RB = RA - RB
            RB = alu(RA, RB, 1, &carry);
            break;
        case 0x04:  // R0 = RA
            R0 = RA;
            printf("R0=RA -> R0=%d\n", R0);  // Print whenever R0 is updated
            break;
        case 0x06:  // Sample assumed operation: RA = RB (Load RB into RA)
            RA = RB;
            break;
        case 0x07:  // Sample assumed operation: RB = RA (Load RA into RB)
            RB = RA;
            break;
        case 0x0D:  // JC = imm (Jump if Carry)
            if (carry) {
                PC = memory[PC];  // Assume the next byte is the jump address
            } else {
                PC++;
            }
            break;
        case 0x0E:  // J = imm (Unconditional Jump)
            PC = memory[PC];  // Assume the next byte is the jump address
            break;
        case 0xFF:  // End of program
            printf("Program terminated by opcode 0xFF.\n");
            return false;  // Signal to stop the simulator
        default:
            printf("Error: Unknown opcode %02X at PC: %02X\n", opcode, PC - 1);
            return false;  // Stop on unknown opcode
   }
  return true;  // Continue execution
}

// Run simulator in the selected mode (continuous or step-by-step)
void run_simulator(int step_by_step) {
    int instruction_counter = 0;

    while (PC < MEMORY_SIZE && instruction_counter < MAX_INSTRUCTIONS) {
        unsigned char opcode = memory[PC++];
        printf("Instruction %d: Opcode %02X at PC %02X\n", instruction_counter++, opcode, PC - 1);

        if (!execute_instruction(opcode)) {
            // Stop execution if we encounter an end-of-program opcode or unknown opcode
            break;
        }

        // Print current state after each instruction
        printf("PC: %02X, RA: %02X, RB: %02X, R0: %02X, Carry: %d\n", PC, RA, RB, R0, carry);

        if (step_by_step) {
            printf("Press Enter to execute the next instruction...\n");
            getchar();  // Wait for user input in step-by-step mode
        }
 }

    if (PC >= MEMORY_SIZE) {
        printf("Reached end of memory. Program terminated.\n");
    } else if (instruction_counter >= MAX_INSTRUCTIONS) {
        printf("Instruction limit reached. Possible infinite loop. Terminating.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <program.bin>\n", argv[0]);
        return 1;
    }

    load_program(argv[1]);

    char mode;
    printf("Select one of the following modes:\nR - Run in continuous mode\nS - Run step-by-step\nSelect mode: ");
    scanf(" %c", &mode);
    getchar();  // To consume the newline character

    if (mode == 'R' || mode == 'r') {
        printf("Starting Simulator in continuous mode...\n");
        run_simulator(0);  // Run in continuous mode
    } else if (mode == 'S' || mode == 's') {
        printf("Starting Simulator in step-by-step mode...\n");
        run_simulator(1);  // Run in step-by-step mode
    } else {
        printf("Invalid mode selection. Exiting.\n");
    }

    return 0;
}

    
