#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define MEMORY_SIZE 256
#define WORD_SIZE 8

typedef struct {
    uint8_t RA;
    uint8_t RB;
    uint8_t RO;
    uint8_t PC;
    uint8_t IR;
    uint8_t S;
    uint8_t Carry;
    uint8_t memory[MEMORY_SIZE];
} K2Processor;

void init_processor(K2Processor* cpu) {
    memset(cpu, 0, sizeof(K2Processor));
}

int load_program(K2Processor* cpu, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    printf("Loading binary file: %s\n", filename);
    
    int addr = 0;
    char line[256];
    
    while (fgets(line, sizeof(line), file) && addr < MEMORY_SIZE) {
        // Remove newline and any whitespace
        line[strcspn(line, "\n\r")] = 0;
        
        // Skip empty lines
        if (strlen(line) < 8) continue;
        
        // Convert binary string to integer
        char *endptr;
        long val = strtol(line, &endptr, 2);
        
        if (*endptr == '\0' || *endptr == '\n' || *endptr == '\r') {
            cpu->memory[addr++] = (uint8_t)val;
        }
    }
    
    fclose(file);
    return addr;
}

void print_instruction(K2Processor* cpu, uint8_t instruction) {
    uint8_t opcode = (instruction >> 4) & 0x0F;
    uint8_t imm = instruction & 0x0F;

    printf("Instruction %d: ", cpu->PC - 1);
    switch(opcode) {
        case 0x0:
            if ((instruction & 0x0F) == 0x0)
                printf("RA=RA+RB");
            else if ((instruction & 0x0F) == 0x4)
                printf("RA=RA-RB");
            else
                printf("RA=%d", imm);
            break;
        case 0x1:
            if ((instruction & 0x0F) == 0x0)
                printf("RB=RA+RB");
            else if ((instruction & 0x0F) == 0x4)
                printf("RB=RA-RB");
            else
                printf("RB=%d", imm);
            break;
        case 0x2:
            printf("RO=RA -> RO=%d", cpu->RA);
            break;
        case 0x7:
            printf("JC=%d (%s)", imm, cpu->Carry == 0 ? "Jump" : "No Jump");
            break;
        case 0xB:
            printf("J=%d (Jump to Instruction %d)", imm, imm);
            break;
    }
    printf(" [Press Enter to continue]\n");
}

void execute_instruction(K2Processor* cpu, uint8_t instruction) {
    uint8_t opcode = (instruction >> 4) & 0x0F;
    uint8_t imm = instruction & 0x0F;
    uint16_t result;

    switch(opcode) {
        case 0x0: // RA operations
            if ((instruction & 0x0F) == 0x0) {
                result = cpu->RA + cpu->RB;
                cpu->Carry = (result > 255) ? 1 : 0;
                cpu->RA = result & 0xFF;
            } else if ((instruction & 0x0F) == 0x4) {
                result = cpu->RA - cpu->RB;
                cpu->Carry = (cpu->RA < cpu->RB) ? 1 : 0;
                cpu->RA = result & 0xFF;
            } else {
                cpu->RA = imm;
            }
            break;
            
        case 0x1: // RB operations
            if ((instruction & 0x0F) == 0x0) {
                result = cpu->RA + cpu->RB;
                cpu->Carry = (result > 255) ? 1 : 0;
                cpu->RB = result & 0xFF;
            } else if ((instruction & 0x0F) == 0x4) {
                result = cpu->RA - cpu->RB;
                cpu->Carry = (cpu->RA < cpu->RB) ? 1 : 0;
                cpu->RB = result & 0xFF;
            } else {
                cpu->RB = imm;
            }
            break;

        case 0x2: // RO = RA
            cpu->RO = cpu->RA;
            printf("RO=%d\n", cpu->RO);
            break;

        case 0x7: // JC
            if (cpu->Carry == 0) {
                cpu->PC = imm;
            }
            break;

        case 0xB: // J
            cpu->PC = imm;
            break;
    }
}

void run_simulation(K2Processor* cpu, char mode) {
    printf("Starting Simulator in %s mode...\n", 
           mode == 'S' ? "step-by-step" : "continuous");
    
    if (mode == 'R') {
        printf("Execution (Register RO output):\n");
    }

    while (cpu->PC < MEMORY_SIZE) {
        uint8_t instruction = cpu->memory[cpu->PC++];
        
        // Skip if instruction is 0 (empty)
        if (instruction == 0) continue;
        
        if (mode == 'S') {
            print_instruction(cpu, instruction);
            getchar(); // Wait for Enter key
        }
        
        execute_instruction(cpu, instruction);
        
        if (mode == 'R') {
            usleep(100000); // Small delay in continuous mode
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <binary_file>\n", argv[0]);
        return 1;
    }

    K2Processor cpu;
    init_processor(&cpu);
    
    int num_instructions = load_program(&cpu, argv[1]);
    if (num_instructions > 0) {
        char mode;
        printf("Select one of the following mode\n");
        printf("R - Run in continuous mode\n");
        printf("S - Run step-by-step\n");
        printf("Select mode:");
        scanf(" %c", &mode);
        getchar(); // Consume newline

        if (mode == 'R' || mode == 'S') {
            run_simulation(&cpu, mode);
        } else {
            printf("Invalid mode selected\n");
        }
    }

    return 0;
}
