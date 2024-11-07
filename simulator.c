#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <string.h>
#include <stdbool.h>

#define MEM_SIZE 256

// Processor state
typedef struct {
    unsigned char ra;     // Register A
    unsigned char rb;     // Register B
    unsigned char ro;     // Output Register
    unsigned char pc;     // Program Counter
    bool carry;          // Carry flag
    unsigned char memory[MEM_SIZE]; // Instruction memory
    int mem_size;        // Actual size of loaded program
} ProcessorState;

// Initialize processor state
void init_processor(ProcessorState *state) {
    state->ra = 0;
    state->rb = 0;
    state->ro = 0;
    state->pc = 0;
    state->carry = false;
    state->mem_size = 0;
    memset(state->memory, 0, MEM_SIZE);
}

// ALU operations
void alu_operation(ProcessorState *state, bool subtract, bool store_in_ra) {
    unsigned char result;
    if (subtract) {
        result = state->ra - state->rb;
        state->carry = state->ra < state->rb;
    } else {
        result = state->ra + state->rb;
        state->carry = (result < state->ra); // Check for overflow
    }
    
    if (store_in_ra)
        state->ra = result;
    else
        state->rb = result;
}

// Execute single instruction
void execute_instruction(ProcessorState *state) {
    unsigned char inst = state->memory[state->pc];
    bool j = (inst >> 7) & 1;    // Jump bit
    bool c = (inst >> 6) & 1;    // Carry jump bit
    bool d1 = (inst >> 5) & 1;   // Register select D1
    bool d0 = (inst >> 4) & 1;   // Register select D0
    bool sreg = (inst >> 3) & 1; // ALU/immediate select
    unsigned char imm = inst & 0x07; // Immediate value
    
    printf("Instruction %d: ", state->pc);

    // Process instruction
    if (j) {
        printf("J=%d (Jump to Instruction %d)\n", imm, imm);
        state->pc = imm;
        return;
    }
    
    if (c && state->carry) {
        printf("JC=%d (Jump to Instruction %d due to carry)\n", imm, imm);
        state->pc = imm;
        return;
    }

    if (!d1 && !d0) { // RA operations
        if (sreg) {
            printf("RA=%d\n", imm);
            state->ra = imm;
        } else {
            if (inst & 0x04) { // Check S bit for subtract
                printf("RA=RA-RB\n");
                alu_operation(state, true, true);
            } else {
                printf("RA=RA+RB\n");
                alu_operation(state, false, true);
            }
        }
    } else if (!d1 && d0) { // RB operations
        if (sreg) {
            printf("RB=%d\n", imm);
            state->rb = imm;
        } else {
            if (inst & 0x04) { // Check S bit for subtract
                printf("RB=RA-RB\n");
                alu_operation(state, true, false);
            } else {
                printf("RB=RA+RB\n");
                alu_operation(state, false, false);
            }
        }
    } else if (d1 && !d0) { // RO operations
        printf("RO=RA -> RO=%d\n", state->ra);
        state->ro = state->ra;
    }

    state->pc++;
=======

unsigned char RA = 0, RB = 0, R0 = 0;
unsigned char PC = 0;
unsigned char carry = 0;

#define MEMORY_SIZE 256
unsigned char memory[MEMORY_SIZE];

void load_program(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }
    fread(memory, sizeof(unsigned char), MEMORY_SIZE, file);
    fclose(file);
}

void execute_instruction(unsigned char opcode) {
    switch (opcode) {
        case 0b00000000:
            RA = RA + RB;
            carry = (RA < RB) ? 1 : 0;
            break;
        case 0b00000001:
            RB = RA + RB;
            carry = (RB < RA) ? 1 : 0;
            break;
        case 0b00000010:
            RA = RA - RB;
            carry = (RA > 255) ? 1 : 0;
            break;
        case 0b00000011:
            RB = RA - RB;
            carry = (RB > 255) ? 1 : 0;
            break;
        case 0b00000100:
            R0 = RA;
            break;
        case 0b00000110:
            RA = memory[PC++];
            break;
        case 0b00000111:
            RB = memory[PC++];
            break;
        case 0b00001101:
            if (carry) {
                PC = memory[PC];
            } else {
                PC++;
            }
            break;
        case 0b00001110:
            PC = memory[PC];
            break;
        default:
            printf("Error: Unknown opcode %02X\n", opcode);
            exit(1);
    }
}

void run_simulator() {
    while (PC < MEMORY_SIZE) {
        unsigned char opcode = memory[PC++];
        execute_instruction(opcode);
        printf("PC: %02X, RA: %02X, RB: %02X, R0: %02X, Carry: %d\n", PC, RA, RB, R0, carry);
        if (PC >= MEMORY_SIZE || opcode == 0xFF) {
            printf("Program terminated.\n");
            break;
        }
    }
>>>>>>> main
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
<<<<<<< HEAD
        printf("Usage: %s <binary_file>\n", argv[0]);
        return 1;
    }

    // Initialize processor
    ProcessorState state;
    init_processor(&state);

    // Load binary file
    FILE *fin = fopen(argv[1], "rb");
    if (!fin) {
        printf("Error: Cannot open input file %s\n", argv[1]);
        return 1;
    }

    printf("Loading binary file: %s\n", argv[1]);
    state.mem_size = fread(state.memory, 1, MEM_SIZE, fin);
    fclose(fin);

    // Get execution mode from user
    char mode;
    printf("Select one of the following mode\n");
    printf("R - Run in continuous mode\n");
    printf("S - Run step-by-step\n");
    printf("Select mode:");
    scanf(" %c", &mode);

    if (mode == 'R' || mode == 'r') {
        printf("Starting Simulator in continuous mode...\n");
        printf("Execution (Register RO output):\n");
        
        while (state.pc < state.mem_size) {
            execute_instruction(&state);
        }
    } else if (mode == 'S' || mode == 's') {
        printf("Starting Simulator in step-by-step mode...\n");
        
        while (state.pc < state.mem_size) {
            execute_instruction(&state);
            printf("[Press Enter to continue]");
            getchar(); // Clear previous newline
            getchar(); // Wait for Enter
        }
    } else {
        printf("Invalid mode selected\n");
        return 1;
    }

    return 0;
}
=======
        printf("Usage: %s <program.bin>\n", argv[0]);
        return 1;
    }
    load_program(argv[1]);
    run_simulator();
    return 0;
}

>>>>>>> main
