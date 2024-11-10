#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define MEMORY_SIZE 256  // memory size
#define WORD_SIZE 8      // in bits

//Define the K2Processor structure
typedef struct {
    uint8_t RA;          
    uint8_t RB;       
    uint8_t RO;         
    uint8_t PC;          
    uint8_t IR;          
    uint8_t S;           
    uint8_t Carry;       
    uint8_t DFF;         
    uint8_t custom_input; 
    uint8_t memory[MEMORY_SIZE]; //Memory array
} K2Processor;

//Initialize the processor
void init_processor(K2Processor* cpu) {
    memset(cpu, 0, sizeof(K2Processor)); // Set all fields to zero
}

//Load binary file into memory
int load_program(K2Processor* cpu, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0; 
    }

    printf("Loading binary file: %s\n", filename);
    
    int addr = 0; //Memory address
    char line[256]; //Buffer for reading lines
    
    //Read instructions from the file
    while (fgets(line, sizeof(line), file) && addr < MEMORY_SIZE) {
        line[strcspn(line, "\n\r")] = 0; //Remove newline
        
        //Skip lines that are too short
        if (strlen(line) < 8) continue; 
        
        //Convert binary string to integer
        char *endptr;
        long val = strtol(line, &endptr, 2);
        
        if (*endptr == '\0') {
            if (val < 0 || val > 255) {
                printf("Warning: Value out of range (0-255) for address %d\n", addr);
                continue; 
            }
            cpu->memory[addr++] = (uint8_t)val; // Store value in memory
        }
    }
    
    fclose(file); // Close the file
    printf("Loaded %d instructions into memory.\n", addr);
    return addr; // Return the number of instructions loaded
}

// Print the current instruction being executed
void print_instruction(K2Processor* cpu, uint8_t instruction) {
    uint8_t opcode = (instruction >> 4) & 0x0F; // Extract opcode
    uint8_t imm = instruction & 0x0F; // Extract immediate value

    printf("Instruction %d: ", cpu->PC - 1);
    switch(opcode) {
        case 0x0:
            printf((instruction & 0x0F) == 0x0 ? "RA=RA+RB" : "RA=RA-RB");
            break;
        case 0x1:
            printf((instruction & 0x0F) == 0x0 ? "RB=RA+RB" : "RB=RA-RB");
            break;
        case 0x2:
            printf("RO=RA -> RO=%d", cpu->RA);
            break;
        case 0x3:
            printf("S=%d (Status Register)", cpu->S);
            break;
        case 0x4:
            printf("DFF=%d (Delay Flip-Flop)", cpu->DFF);
            break;
        case 0x5:
            printf("Custom Input=%d", cpu->custom_input);
            break;
        case 0x7:
            printf("JC=%d (%s)", imm, cpu->Carry == 0 ? "Jump" : "No Jump");
            break;
        case 0xB:
            printf("J=%d (Jump to Instruction %d)", imm, imm);
            break;
        default:
            printf("Unknown instruction");
            break;
    }
    printf(" [Press Enter to continue]\n");
}

// Execute the instruction based on the opcode
void execute_instruction(K2Processor* cpu, uint8_t instruction) {
    uint8_t opcode = (instruction >> 4) & 0x0F; // Extract opcode
    uint8_t imm = instruction & 0x0F; // Extract immediate value
    uint16_t result; // Variable to hold calculation results

    switch(opcode) {
        case 0x0: // Operations on RA
            if ((instruction & 0x0F) == 0x0) {
                result = cpu->RA + cpu->RB;
                cpu->Carry = (result > 255) ? 1 : 0; // Update carry flag
                cpu->RA = result & 0xFF; // Update RA
            } else if ((instruction & 0x0F) == 0x4) {
                result = cpu->RA - cpu->RB;
                cpu->Carry = (cpu->RA < cpu->RB) ? 1 : 0; 
                cpu->RA = result & 0xFF; 
            } else {
                cpu->RA = imm; //immediate value
            }
            break;
        case 0x1: // Operations on RB
            if ((instruction & 0x0F) == 0x0) {
                result = cpu->RA + cpu->RB;
                cpu->Carry = (result > 255) ? 1 : 0;
                cpu->RB = result & 0xFF; // Update RB
            } else if ((instruction & 0x0F) == 0x4) {
                result = cpu->RA - cpu->RB;
                cpu->Carry = (cpu->RA < cpu->RB) ? 1 : 0; 
                cpu->RB = result & 0xFF; 
            } else {
                cpu->RB = imm; //load immediate value
            }
            break;
        case 0x2: // Copy RA to RO
            cpu->RO = cpu->RA;
            printf("RO=%d\n", cpu->RO);
            break;
        case 0x3: // Set S Register
            cpu->S = imm;
            break;
        case 0x4: // Set Delay Flip-Flop
            cpu->DFF = imm;
            break;
        case 0x5: 
            cpu->custom_input = imm;
            break;
        case 0x7: // Conditional Jump
            if (cpu->Carry == 0) {
                cpu->PC = imm; 
            }
            break;
        case 0xB: // Unconditional Jump
            cpu->PC = imm; // Jump to the address
            break;
        default:
            printf("Error: Unknown opcode %02X\n", opcode);
            break;
    }
}


// Run the simulation based on the selected mode
void run_simulation(K2Processor* cpu, char mode) {
    printf("Starting Simulator in %s mode...\n", 
           mode == 'S' ? "step-by-step" : "continuous");
    
    while (cpu->PC < MEMORY_SIZE) {
        uint8_t instruction = cpu->memory[cpu->PC++];
        
        if (instruction == 0) continue; //Skip

        printf("PC=%d, Instruction=%02X\n", cpu->PC - 1, instruction);
        
        if (mode == 'S') {
            print_instruction(cpu, instruction); // Print instruction details
            getchar(); //for Enter key
        }
        
        execute_instruction(cpu, instruction); //  call Execute 
        
        if (mode == 'R') {
            usleep(100000); // delay 100 mlsec
        }
    }
    printf("Simulation ended.\n");
}

// Main function to handle program execution
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <binary_file>\n", argv[0]);
        fprintf(stderr, "Example: %s example.bin\n", argv[0]);
        return 1; // Exit if no file provided
    }

    K2Processor cpu; // Create a K2Processor instance
    init_processor(&cpu); // Initialize the processor
    
    // Load the program into memory
    int num_instructions = load_program(&cpu, argv[1]);
    if (num_instructions > 0) {
        char mode;
        printf("Select one of the following modes:\n");
        printf("R - Run in continuous mode\n");
        printf("S - Run step-by-step\n");
        printf("Select mode: ");
        scanf(" %c", &mode);
        getchar(); //user press to the next

        if (mode == 'R' || mode == 'S') {
            run_simulation(&cpu, mode); // call the simulation
        } else {
            printf("Invalid mode selected\n");
        }
    } else {
        printf("No instructions loaded. Exiting...\n");
    }

    return 0;
}
