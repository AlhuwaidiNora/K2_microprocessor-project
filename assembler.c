#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function prototypes
void parseInstruction(const char* line, FILE* outputFile);
void handleError(const char* errorMessage);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <assembly_file>\n", argv[0]);
        return 1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("Error opening file");
        return 1;
    }

    // Create output file name
    char outputFileName[256];
    strcpy(outputFileName, argv[1]);
    char* dot = strrchr(outputFileName, '.');
    if (dot) *dot = '\0';
    strcat(outputFileName, ".bin");

    FILE* outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        perror("Error creating output file");
        fclose(inputFile);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        parseInstruction(line, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
    printf("Successfully generated output file: %s\n", outputFileName);
    return 0;
}

// Function to parse each assembly instruction and write to output file
void parseInstruction(const char* line, FILE* outputFile) {
    // Trim whitespace and handle empty lines
    while (*line == ' ' || *line == '\t') line++;
    if (*line == '\0' || *line == '\n') return;

    // Example parsing: "RA=0" -> binary code "00001000"
    if (strncmp(line, "RA=0", 4) == 0) {
        fputc(0x08, outputFile); // Example binary code for RA=0
    } else if (strncmp(line, "RB=1", 4) == 0) {
        fputc(0x19, outputFile); // Example binary code for RB=1
    } else if (strncmp(line, "RO=RA", 5) == 0) {
        fputc(0x20, outputFile); // Example binary code for RO=RA
    } else {
        handleError("Unknown instruction");
    }
}

// Function to handle errors
void handleError(const char* errorMessage) {
    fprintf(stderr, "Error: %s\n", errorMessage);
    exit(1);
}


