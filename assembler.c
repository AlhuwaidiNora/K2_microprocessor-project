#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to convert an integer (0-15) to a binary string of fixed length
void int_to_binary(int value, char *output, int bits) {
    for (int i = bits - 1; i >= 0; --i) {
        output[i] = (value % 2) ? '1' : '0';
        value /= 2;
    }
    output[bits] = '\0';  // Null-terminate the string
}

// Function to convert assembly instruction to machine code
int convert_to_machine_code(const char *instruction, char *machine_code) {
    memset(machine_code, '0', 8);
    machine_code[8] = '\0';

    if (strcmp(instruction, "RA=0") == 0) strncpy(machine_code, "00001000", 8);
    else if (strcmp(instruction, "RB=1") == 0) strncpy(machine_code, "00011001", 8);
    else if (strcmp(instruction, "RO=RA") == 0) strncpy(machine_code, "00100000", 8);
    else if (strcmp(instruction, "RB=RA+RB") == 0) strncpy(machine_code, "00010000", 8);
    else if (strcmp(instruction, "JC=0") == 0) strncpy(machine_code, "01110000", 8);
    else if (strcmp(instruction, "RA=RA+RB") == 0) strncpy(machine_code, "00000000", 8);
    else if (strcmp(instruction, "RB=RA-RB") == 0) strncpy(machine_code, "00010100", 8);
    else if (strcmp(instruction, "RA=RA-RB") == 0) strncpy(machine_code, "00000100", 8);
    else if (strcmp(instruction, "J=2") == 0) strncpy(machine_code, "10110010", 8);
    //else if (strcmp(instruction, "JC=3") == 0) strncpy(machine_code, "01110011", 8);
   // else if (strcmp(instruction, "J=5") == 0) strncpy(machine_code, "10110101", 8);
    else return -1;  // Invalid instruction

    return 0;
}

// Function to handle errors
void handle_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}

// Helper function to trim whitespace
char *trim_whitespace(char *str) {
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        handle_error("Please provide the assembly file name as a command line argument.");
    }

    // Print starting message
    printf("Starting Assembler...\n");

    // Open the input assembly file
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        handle_error("Failed to open input file.");
    }

    printf("Reading file: %s\n", argv[1]);

    // Generate the output file name
    char output_filename[100];
    snprintf(output_filename, sizeof(output_filename), "%s.bin", strtok(argv[1], "."));

    // Open the output binary file
    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        fclose(input_file);
        handle_error("Failed to create output file.");
    }

    char line[MAX_LINE_LENGTH];
    char machine_code[9];
    int line_number = 1;

    // Read each line of the assembly file
    while (fgets(line, sizeof(line), input_file)) {
        // Strip newline and trim whitespace
        line[strcspn(line, "\n")] = '\0';
        char *instruction = trim_whitespace(line);

        // Convert the assembly instruction to machine code
        if (convert_to_machine_code(instruction, machine_code) == -1) {
            fprintf(stderr, "Warning: Invalid instruction '%s' on line %d\n", instruction, line_number);
            continue;
        }

        // Print line number, assembly instruction, and machine code
        printf("Line %d: %s -> Machine Code: %s\n", line_number, instruction, machine_code);

        // Write the machine code to the binary file
        fprintf(output_file, "%s\n", machine_code);

        line_number++;  // Increment line counter
    }

    // Close files
    fclose(input_file);
    fclose(output_file);

    // Print success message
    printf("Successfully generated output file: %s\n", output_filename);
    return 0;
}
