#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define IM_SIZE 16
unsigned char IM[IM_SIZE];
unsigned char MUX_OUT;

typedef struct {
    unsigned char RA, RB, R0;
} Registers;

struct ControlSignals {
    bool j;
    bool c;
    bool D1;
    bool D0;
    bool sreg;
    bool s;
    bool imm2, imm1, imm0;
};

unsigned char RA, RB, R0;

void load(unsigned char IM[], const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }
    
    char line[9];
    int i = 0;
    
    while (fgets(line, sizeof(line), fp) && i < IM_SIZE) {
        unsigned char instruction = 0;
        for (int j = 0; j < 8; j++) {
            if (line[j] == '1') {
                instruction |= (1 << (7-j));
            }
        }
        IM[i++] = instruction;
    }
    fclose(fp);
}

unsigned char fetch_inst(unsigned char *IM, unsigned char *pc) {
    unsigned char instruction = IM[*pc];
    *pc = (*pc + 1) % IM_SIZE;
    return instruction;
}

void instructionDecode(unsigned char instruction, struct ControlSignals *flags) {
    flags->j = (instruction & 0x80) >> 7;
    flags->c = (instruction & 0x40) >> 6;
    flags->D1 = (instruction & 0x20) >> 5;
    flags->D0 = (instruction & 0x10) >> 4;
    flags->sreg = (instruction & 0x08) >> 3;
    flags->s = (instruction & 0x04) >> 2;
    flags->imm2 = (instruction & 0x04) >> 2;
    flags->imm1 = (instruction & 0x02) >> 1;
    flags->imm0 = instruction & 0x01;
}

unsigned char MUX(unsigned char sum, unsigned char imm, bool Sreg) {
    return Sreg ? imm : sum;
}

void decoder(bool D1, bool D0, unsigned char sum, unsigned char imm, bool Sreg) {
    MUX_OUT = MUX(sum, imm, Sreg);

    if (D1 == 0 && D0 == 0) {
        RA = MUX_OUT;
    }
    else if (D1 == 0 && D0 == 1) {
        RB = MUX_OUT;
    }
    else if (D1 == 1 && D0 == 0) {
        R0 = RA;
    }
}

unsigned char ALU(unsigned char RA, unsigned char RB, struct ControlSignals *flags, bool *carry) {
    unsigned int result;
    if (flags->s == 0) {
        result = RA + RB;
    } else {
        result = RA - RB;
    }
    *carry = (result > 0x0F);
    return (result & 0x0F);
}

bool DFF(bool carry) {
    static bool stored_carry = false;
    stored_carry = carry;
    return stored_carry;
}

void execute(Registers *regs, struct ControlSignals *ctrl, unsigned char *pc) {
    unsigned char sum;
    bool carry;

    sum = ALU(regs->RA, regs->RB, ctrl, &carry);
    bool stored_carry = DFF(carry);
    unsigned char imm = (ctrl->imm2 << 2) | (ctrl->imm1 << 1) | ctrl->imm0;
    
    decoder(ctrl->D1, ctrl->D0, sum, imm, ctrl->sreg);

    regs->RA = RA;
    regs->RB = RB;
    regs->R0 = R0;

    if (ctrl->j || (ctrl->c && stored_carry)) {
        *pc = imm;
    }
}

void print_step_instruction(int inst_count, struct ControlSignals *ctrl, Registers *regs, bool carry) {
    printf("Instruction %d: ", inst_count);

    if (ctrl->D1 == 0 && ctrl->D0 == 0) {
        if (ctrl->sreg)
            printf("RA=%d", (ctrl->imm2 << 2) | (ctrl->imm1 << 1) | ctrl->imm0);
        else
            printf("RA=RA+RB");
    } else if (ctrl->D1 == 0 && ctrl->D0 == 1) {
        if (ctrl->sreg)
            printf("RB=%d", (ctrl->imm2 << 2) | (ctrl->imm1 << 1) | ctrl->imm0);
        else if (ctrl->s)
            printf("RB=RA-RB");
        else
            printf("RB=RA+RB");
    } else if (ctrl->D1 == 1 && ctrl->D0 == 0) {
        printf("RO=RA -> RO=%d", regs->R0);
    } else if (ctrl->j) {
        unsigned char imm = (ctrl->imm2 << 2) | (ctrl->imm1 << 1) | ctrl->imm0;
        printf("J=%d (Jump to Instruction %d)", imm, imm);
    } else if (ctrl->c) {
        printf("JC=%d (%s)", carry, carry ? "Jump" : "No Jump");
    }
}

void simulate(const char* filename) {
    char mode;
    Registers regs = {0};
    struct ControlSignals ctrl = {0};
    unsigned char pc = 0;
    bool carry = false;

    printf("# Simulator Prompt\n");
    printf("Select one of the following mode\n");
    printf("R - Run in continuous mode\n");
    printf("S - Run step-by-step\n");
    printf("Select mode: ");
    scanf(" %c", &mode);

    printf("Loading binary file: %s\n", filename);
    load(IM, filename);

    if (mode == 'S' || mode == 's') {
        printf("Starting Simulator in step-by-step mode...\n");
        int inst_count = 0;

        while (pc < IM_SIZE) {
            unsigned char instruction = fetch_inst(IM, &pc);
            if (instruction == 0 && pc > 1) break;
            
            instructionDecode(instruction, &ctrl);
            execute(&regs, &ctrl, &pc);
            
            print_step_instruction(inst_count, &ctrl, &regs, carry);
            printf("[Press Enter to continue]\n");
            if (ctrl.D1 == 1 && ctrl.D0 == 0) {
                printf("# Printed\nbecause RO=RA\n");
            }
            
            getchar();
            getchar();
            inst_count++;
        }
    } else if (mode == 'R' || mode == 'r') {
        printf("Starting Simulator in continuous mode...\n");
        printf("Execution (Register RO output):\n");
        
        while (pc < IM_SIZE) {
            unsigned char instruction = fetch_inst(IM, &pc);
            if (instruction == 0 && pc > 1) break;
            
            instructionDecode(instruction, &ctrl);
            execute(&regs, &ctrl, &pc);
            
            if (ctrl.D1 == 1 && ctrl.D0 == 0) {
                printf("RO=%d\n", regs.R0);
                usleep(100000);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    simulate(argv[1]);
    return 0;
}
