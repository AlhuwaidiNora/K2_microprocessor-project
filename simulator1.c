#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Function prototypes
void runContinuousMode(unsigned char* instructions, int length);
void runStepByStepMode(unsigned char* instructions, int length);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <binary_file>\n", argv[0]);
        return 1;
    }

    FILE* binaryFile = fopen(argv[1], "rb");
    if (!binaryFile) {
        perror("Error opening binary file");
        return 1;
    }

    // Load the binary file into memory
    fseek(binaryFile, 0, SEEK_END);
    int length = ftell(binaryFile);
    rewind(binaryFile);

    unsigned char* instructions = (unsigned char*)malloc(length);
    if (!instructions) {
        perror("Memory allocation failed");
        fclose(binaryFile);
        return 1;
    }
    fread(instructions, 1, length, binaryFile);
    fclose(binaryFile);

    printf("Loading binary file: %s\n", argv[1]);
    printf("Select one of the following modes:\n");
    printf("R - Run in continuous mode\n");
    printf("S - Run step-by-step\n");
    char mode;
    scanf(" %c", &mode);

    if (mode == 'R' || mode == 'r') {
        runContinuousMode(instructions, length);
    } else if (mode == 'S' || mode == 's') {
        runStepByStepMode(instructions, length);
    } else {
        printf("Invalid mode selected.\n");
    }

    free(instructions);
    return 0;
}

// Function to run the simulator in continuous mode
void runContinuousMode(unsigned char* instructions, int length) {
    printf("Running in continuous mode...\n");
    for (int i = 0; i < length; ++i) {
        // Execute each instruction (example: print instruction value)
        printf("Executing instruction: %02X\n", instructions[i]);
    }
}

// Function to run the simulator step-by-step
void runStepByStepMode(unsigned char* instructions, int length) {
    printf("Running in step-by-step mode...\n");
    for (int i = 0; i < length; ++i) {
        printf("Executing instruction: %02X [Press Enter to continue]\n", instructions[i]);
        getchar(); // Wait for user input to continue
    }
}
