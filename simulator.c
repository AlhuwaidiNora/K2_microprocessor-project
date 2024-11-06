#include <stdio.h>
#include <stdlib.h>

unsigned char RA = 0, RB = 0, R0 = 0;
unsigned char PC = 0;
unsigned char carry = 0;

// creat array for memory 
#define MEMORY_SIZE 256
unsigned char memory[MEMORY_SIZE];
//
void load_program(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }
    fread(memory, sizeof(unsigned char), MEMORY_SIZE, file);
    fclose(file);
}

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

void execute_instruction(unsigned char opcode) {
    switch (opcode) {
        case 0b00000000:  // Initialize RA = 0
            RA = 0;
            printf("Instruction %02X: RA = 0 [Press Enter to continue]\n", opcode);
            break;
        case 0b00000001:  // Initialize RB = 1
                                                      RB = 1;
            printf("Instruction %02X: RB = 1 [Press Enter to continue]\n", opcode);
            break;
        case 0b00000100:  // R0 = RA
            R0 = RA;
            printf("Instruction %02X: RO = RA -> RO = %02X [Press Enter to continue]\n", opcode, R0);
            break;
        case 0b00000101:  // RB = RA + RB
            RB = alu(RA, RB, 0, &carry);
            printf("Instruction %02X: RB = RA + RB [Press Enter to continue]\n", opcode);
            break;
        case 0b00001101:  // JC = imm (Jump if Carry)
            if (carry) {
                PC = memory[PC];
                printf("Instruction %02X: JC = %02X (Jump to Instruction %02X) [Press Enter to continue]\n", opcode, memory[PC], PC);
            } else {
                PC++;
            }
            break;
        case 0b00001110:  // J = imm (Unconditional Jump)
            PC = memory[PC];
            printf("Instruction %02X: J = %02X (Jump to Instruction %02X) [Press Enter to continue]\n", opcode, memory[PC], PC);
            break;
        default:
            printf("Error: Unknown opcode %02X\n", opcode);
            exit(1);
    }
}

void run_simulator(int step_by_step) {
    while (PC < MEMORY_SIZE) {
        unsigned char opcode = memory[PC++];
        execute_instruction(opcode);
        if (step_by_step) {
            getchar();
        }
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
    getchar();  // Consume newline

    if (mode == 'R' || mode == 'r') {
        printf("Starting Simulator in continuous mode...\n");
        run_simulator(0);
    } else if (mode == 'S' || mode == 's') {
        printf("Starting Simulator in step-by-step mode...\n");
        run_simulator(1);
    } else {
        printf("Invalid mode selection. Exiting.\n");
    }

    return 0; }
