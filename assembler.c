#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100

// Function to convert an integer (0-15) to a binary string of fixed length
void int_to_binary(int value, char *output, int bits) {
    for (int i = bits - 1; i >= 0; --i) {
        output[i] = (value % 2) ? '1' : '0';
        value /= 2;
    }
    output[bits] = '\0';
}

// Function to handle errors
void handle_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}

// Function to convert assembly instruction to machine code
int convert_to_machine_code(const char *instruction, char *machine_code) {
    memset(machine_code, '0', 8);
    machine_code[8] = '\0';

    // Remove whitespace for easier parsing
    char clean_instruction[MAX_LINE_LENGTH];
    int i = 0, j = 0;
    while (instruction[i] != '\0') {
        if (!isspace(instruction[i])) {
            clean_instruction[j++] = instruction[i];
        }
        i++;
    }
    clean_instruction[j] = '\0';

    // Match specific instruction patterns
    if (strcmp(clean_instruction, "RA=0") == 0) {
        strncpy(machine_code, "00001000", 8);
    } else if (strcmp(clean_instruction, "RB=1") == 0) {
        strncpy(machine_code, "00011001", 8);
    } else if (strcmp(clean_instruction, "R0=RA") == 0) {
        strncpy(machine_code, "00100000", 8); // Example code for R0=RA
    } else if (strcmp(clean_instruction, "RB=RA+RB") == 0) {
        strncpy(machine_code, "00010000", 8);
    } else if (strcmp(clean_instruction, "JC=0") == 0) {
        strncpy(machine_code, "01110000", 8);
    } else if (strcmp(clean_instruction, "RA=RA+RB") == 0) {
        strncpy(machine_code, "00000000", 8);
    } else if (strcmp(clean_instruction, "RB=RA-RB") == 0) {
        strncpy(machine_code, "00010100", 8);
    } else if (strcmp(clean_instruction, "RA=RA-RB") == 0) {
        strncpy(machine_code, "00000100", 8);
    } else if (strcmp(clean_instruction, "J=2") == 0) {
        strncpy(machine_code, "10110010", 8);
    }
    // Generic immediate value instructions
    else if (strncmp(clean_instruction, "RA=", 3) == 0) {
        int imm = atoi(&clean_instruction[3]);
        if (imm < 0 || imm > 15) return -1;
        char imm_bin[5];
        int_to_binary(imm, imm_bin, 4);
        snprintf(machine_code, 9, "0000%s", imm_bin);
    } else if (strncmp(clean_instruction, "RB=", 3) == 0) {
        int imm = atoi(&clean_instruction[3]);
        if (imm < 0 || imm > 15) return -1;
        char imm_bin[5];
        int_to_binary(imm, imm_bin, 4);
        snprintf(machine_code, 9, "0001%s", imm_bin);
    } else if (strncmp(clean_instruction, "JC=", 3) == 0) {
        int imm = atoi(&clean_instruction[3]);
        if (imm < 0 || imm > 15) return -1;
        char imm_bin[5];
        int_to_binary(imm, imm_bin, 4);
        snprintf(machine_code, 9, "0111%s", imm_bin);
    } else if (strncmp(clean_instruction, "J=", 2) == 0) {
        int imm = atoi(&clean_instruction[2]);
        if (imm < 0 || imm > 15) return -1;
        char imm_bin[5];
        int_to_binary(imm, imm_bin, 4);
        snprintf(machine_code, 9, "1011%s", imm_bin);
    } else {
        return -1; // Invalid instruction
    }

    return 0;
}

// Function to trim whitespace from a string
void trim(char *str) {
    char *start = str;
    char *end = str + strlen(str) - 1;

    while(isspace(*start)) start++;
    while(end > start && isspace(*end)) end--;

    int len = end - start + 1;
    memmove(str, start, len);
    str[len] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        handle_error("Please provide the assembly file name as a command line argument.");
    }

    printf("Starting Assembler...\n");

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        handle_error("Failed to open input file.");
    }

    printf("Reading file: %s\n", argv[1]);

    // Generate output filename
    char output_filename[100];
    char *base_name = strdup(argv[1]);
    char *dot_pos = strrchr(base_name, '.');
    if (dot_pos != NULL) {
        *dot_pos = '\0';
    }
    snprintf(output_filename, sizeof(output_filename), "%s.bin", base_name);
    free(base_name);

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        fclose(input_file);
        handle_error("Failed to create output file.");
    }

    char line[MAX_LINE_LENGTH];
    char machine_code[9];
    int line_number = 1;

    // Read and process each line
    while (fgets(line, sizeof(line), input_file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        // Trim whitespace
        trim(line);

        // Skip empty lines
        if (strlen(line) == 0) {
            continue;
        }

        // Convert and print the instruction and its machine code
        if (convert_to_machine_code(line, machine_code) == -1) {
            fprintf(stderr, "Warning: Invalid instruction '%s' on line %d\n", line, line_number);
        } else {
            printf("Line %d: %s -> Machine Code: %s\n", line_number, line, machine_code);
            fprintf(output_file, "%s\n", machine_code);
        }

        line_number++;
    }

    fclose(input_file);
    fclose(output_file);

    printf("Successfully generated output file: %s\n", output_filename);
    return 0;
}
