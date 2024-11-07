#include <stdio.h>

int main() {
    // Fibonacci program opcodes
    unsigned char program[] = {
        0x00,  // RA = 0
        0x01,  // RB = 1
        0x02,  // R0 = RA
        0x03,  // RB = RA + RB
        0x04,  // RA = RA + RB
        0x05,  // RB = RA - RB
        0x06,  // RA = RA - RB
        0x07,  // J = 2 (Jump to instruction 2)
        0xFF   // End program (not reached due to jump)
    };
    
    FILE *file = fopen("fibonacci.bin", "wb");
    if (!file) {
        perror("Error creating file");
        return 1;
    }
    
    fwrite(program, sizeof(program), 1, file);
    fclose(file);
    printf("fibonacci.bin created successfully\n");
    
    return 0;
}

