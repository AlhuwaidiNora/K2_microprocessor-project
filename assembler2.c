#include <stdio.h>
#include <string.h>

void generate_machine_code(const char *line, FILE *output_file) {
    if (strcmp(line, "RA = RA + RB") == 0) {
        fprintf(output_file, "00000000\n");
    } else if (strcmp(line, "RB = RB + RB") == 0) {
        fprintf(output_file, "00001000\n");
    } else if (strcmp(line, "RA = RA - RB") == 0) {
        fprintf(output_file, "00000001\n");
    } else if (strcmp(line, "RB = RA - RB") == 0) {
        fprintf(output_file, "00001001\n");
    } else if (strcmp(line, "RO = RA") == 0) {
        fprintf(output_file, "00100000\n");
    } else {
        fprintf(stderr, "Error: Unknown instruction '%s'\n", line);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_asm_file>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("Error: Unable to open input file.\n");
        return 1;
    }

    FILE *output_file = fopen("output.bin", "w");
    if (!output_file) {
        printf("Error: Unable to create output file.\n");
        fclose(input_file);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), input_file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        generate_machine_code(line, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    printf("Machine code generated successfully in output.bin\n");

    return 0;
}
