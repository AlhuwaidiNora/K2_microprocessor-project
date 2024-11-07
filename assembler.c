#include <stdio.h>
<<<<<<< HEAD
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_INSTRUCTIONS 256

// Instruction structure
typedef struct {
    char opcode[32];
    char operand[32];
} Instruction;

// Function to trim whitespace
void trim(char *str) {
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((unsigned char) str[begin]))
        begin++;

    while ((end >= begin) && isspace((unsigned char) str[end]))
        end--;

    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0';
}

// Function to parse assembly instruction
Instruction parse_instruction(char *line) {
    Instruction inst = {"", ""};
    char *equals = strchr(line, '=');
    
    if (equals) {
        // Split at equals sign
        *equals = '\0';
        strcpy(inst.opcode, line);
        strcpy(inst.operand, equals + 1);
    } else {
        // No equals sign, just copy the whole line as opcode
        strcpy(inst.opcode, line);
    }
    
    trim(inst.opcode);
    trim(inst.operand);
    
    return inst;
}

// Function to convert instruction to machine code
unsigned char generate_machine_code(Instruction inst) {
    unsigned char code = 0;
    
    // Handle each instruction type
    if (strcmp(inst.opcode, "RA") == 0) {
        if (strcmp(inst.operand, "0") == 0) {
            code = 0b00001000; // RA = 0
        } else if (strstr(inst.operand, "RA+RB") != NULL) {
            code = 0b00000000; // RA = RA + RB
        } else if (strstr(inst.operand, "RA-RB") != NULL) {
            code = 0b00000100; // RA = RA - RB
        }
    } else if (strcmp(inst.opcode, "RB") == 0) {
        if (strcmp(inst.operand, "1") == 0) {
            code = 0b00011001; // RB = 1
        } else if (strstr(inst.operand, "RA+RB") != NULL) {
            code = 0b00010000; // RB = RA + RB
        } else if (strstr(inst.operand, "RA-RB") != NULL) {
            code = 0b00010100; // RB = RA - RB
        }
    } else if (strcmp(inst.opcode, "RO") == 0) {
        if (strcmp(inst.operand, "RA") == 0) {
            code = 0b00100000; // RO = RA
        }
    } else if (strcmp(inst.opcode, "JC") == 0) {
        int imm = atoi(inst.operand);
        code = 0b01110000 | (imm & 0x07); // JC = imm
    } else if (strcmp(inst.opcode, "J") == 0) {
        int imm = atoi(inst.operand);
        code = 0b10110000 | (imm & 0x07); // J = imm
    }
    
    return code;
=======
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void trim_whitespace(char *str) {
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) len--;
    str[len] = '\0';

    int start = 0;
    while (isspace(str[start])) start++;
    memmove(str, str + start, len - start + 1);
}

unsigned char translate_instruction(char *instruction) {
    trim_whitespace(instruction);
    printf("Instruction read: '%s'\n", instruction); // طباعة التعليمة للتحقق منها

    if (strcmp(instruction, "RA = RA + RB") == 0) {
        return 0b00000000;
    } else if (strcmp(instruction, "RB = RA + RB") == 0) {
        return 0b00000001;
    } else if (strcmp(instruction, "RA = RA - RB") == 0) {
        return 0b00000010;
    } else if (strcmp(instruction, "RB = RA - RB") == 0) {
        return 0b00000011;
    } else if (strcmp(instruction, "R0 = RA") == 0) {
        return 0b00000100;
    } else if (strcmp(instruction, "RA = imm") == 0) {
        return 0b00000110;
    } else if (strcmp(instruction, "RB = imm") == 0) {
        return 0b00000111;
    } else if (strcmp(instruction, "JC = imm") == 0) {
        return 0b00001101;
    } else if (strcmp(instruction, "J = imm") == 0) {
        return 0b00001110;
    } else {
        printf("Error: Unrecognized instruction: '%s'\n", instruction);
        return 0;
    }
}

FILE* open_file(const char *filename, const char *mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }
    return file;
}

void handle_error(const char *message) {
    printf("Error: %s\n", message);
>>>>>>> main
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
<<<<<<< HEAD
        printf("Usage: %s <assembly_file>\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    char output_file[256];
    strcpy(output_file, input_file);
    char *dot = strrchr(output_file, '.');
    if (dot) *dot = '\0';
    strcat(output_file, ".bin");

    FILE *fin = fopen(input_file, "r");
    if (!fin) {
        printf("Error: Cannot open input file %s\n", input_file);
        return 1;
    }

    FILE *fout = fopen(output_file, "wb");
    if (!fout) {
        printf("Error: Cannot create output file %s\n", output_file);
        fclose(fin);
        return 1;
    }

    printf("Starting Assembler...\n");
    printf("Reading file: %s\n", input_file);

    char line[MAX_LINE_LENGTH];
    int line_num = 0;

    while (fgets(line, sizeof(line), fin)) {
        // Skip empty lines and comments
        trim(line);
        if (strlen(line) == 0 || line[0] == '#')
            continue;

        line_num++;
        Instruction inst = parse_instruction(line);
        unsigned char machine_code = generate_machine_code(inst);

        printf("Line %d: %s -> Machine Code: %08b\n", 
               line_num, line, machine_code);
        
        fwrite(&machine_code, sizeof(machine_code), 1, fout);
    }

    printf("Successfully generated output file: %s\n", output_file);

    fclose(fin);
    fclose(fout);
=======
        printf("Usage: %s <input_file.asm>\n", argv[0]);
        return 1;
    }

    FILE *input_file = open_file(argv[1], "r");
    FILE *output_file = open_file("output.bin", "wb");

    char line[100];
    while (fgets(line, sizeof(line), input_file)) {
        trim_whitespace(line);
        unsigned char machine_code = translate_instruction(line);
        if (machine_code == 0) {
            handle_error("Unrecognized instruction.");
            continue;
        }
        fwrite(&machine_code, sizeof(machine_code), 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);
>>>>>>> main
    return 0;
}
