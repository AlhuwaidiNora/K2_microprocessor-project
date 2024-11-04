#include <stdio.h>
#include <stdlib.h>

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
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <program.bin>\n", argv[0]);
        return 1;
    }
    load_program(argv[1]);
    run_simulator();
    return 0;
}

