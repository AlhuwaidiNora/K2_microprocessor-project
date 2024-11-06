#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Instruction encoding functions
unsigned char encode_ra_plus_rb() { return 0x00; }
unsigned char encode_rb_plus_ra() { return 0x10; }
unsigned char encode_ra_minus_rb() { return 0x04; }
unsigned char encode_rb_minus_ra() { return 0x14; }
unsigned char encode_ro_equals_ra() { return 0x20; }
unsigned char encode_ra_equals_imm(int imm) { return 0x08 | (imm & 0x07); }
unsigned char encode_rb_equals_imm(int imm) { return 0x18 | (imm & 0x07); }
unsigned char encode_jc_equals_imm(int imm) { return 0x70 | (imm & 0x07); }
unsigned char encode_j_equals_imm(int imm) { return 0xB0 | (imm & 0x07); }

// Trim whitespace from string
void trim(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file.asm>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("Error: Cannot open input file %s\n", argv[1]);
        return 1;
    }

    // Create output filename by replacing .asm with .bin
    char output_filename[256];
    strcpy(output_filename, argv[1]);
    char *dot = strrchr(output_filename, '.');
    if (dot) *dot = '\0';
    strcat(output_filename, ".bin");

    FILE *output = fopen(output_filename, "wb");
    if (!output) {
        printf("Error: Cannot create output file %s\n", output_filename);
        fclose(input);
        return 1;
    }

    printf("Starting Assembler...\n");
    printf("Reading file: %s\n", argv[1]);

    char line[256];
    int line_number = 0;

    while (fgets(line, sizeof(line), input)) {
        line_number++;
        trim(line);
        
        // Skip empty lines and comments
        if (strlen(line) == 0 || line[0] == '#' || line[0] == ';')
            continue;

        unsigned char machine_code = 0;
        char instruction[32];
        int value;

        // Parse instruction
        if (sscanf(line, "RA=RA+RB%s", instruction) == 0)
            machine_code = encode_ra_plus_rb();
        else if (sscanf(line, "RB=RA+RB%s", instruction) == 0)
            machine_code = encode_rb_plus_ra();
        else if (sscanf(line, "RA=RA-RB%s", instruction) == 0)
            machine_code = encode_ra_minus_rb();
        else if (sscanf(line, "RB=RA-RB%s", instruction) == 0)
            machine_code = encode_rb_minus_ra();
        else if (sscanf(line, "RO=RA%s", instruction) == 0)
            machine_code = encode_ro_equals_ra();
        else if (sscanf(line, "RA=%d", &value) == 1)
            machine_code = encode_ra_equals_imm(value);
        else if (sscanf(line, "RB=%d", &value) == 1)
            machine_code = encode_rb_equals_imm(value);
        else if (sscanf(line, "JC=%d", &value) == 1)
            machine_code = encode_jc_equals_imm(value);
        else if (sscanf(line, "J=%d", &value) == 1)
            machine_code = encode_j_equals_imm(value);
        else {
            printf("Error: Invalid instruction at line %d: %s\n", line_number, line);
            continue;
        }

        printf("Line %d: %s -> Machine Code: %02X\n", line_number, line, machine_code);
        fwrite(&machine_code, 1, 1, output);
    }

    printf("Successfully generated output file: %s\n", output_filename);

    fclose(input);
    fclose(output);
    return 0;
}
