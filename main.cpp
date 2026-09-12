#include <stdio.h>
#include <stdint.h>

#define MEM_SIZE 1024
#define FB_START (MEM_SIZE - 256)

uint16_t mem[MEM_SIZE] = {0};

// CPU opcodes
#define NOP  0x00
#define LDA  0x01
#define LDB  0x02
#define ADD  0x03
#define SUB  0x04
#define STA  0x05
#define JMP  0x06
#define MUL  0x07
#define PRT  0x08  // print mem[arg] as number
#define PRS  0x09
#define HLT  0xFF

uint16_t A = 0, B = 0, PC = 0;

void run() {
    while (1) {
        uint8_t op = mem[PC++];
        switch (op) {
            case NOP: break;
            case LDA: A = mem[mem[PC++]]; break;
            case LDB: B = mem[mem[PC++]]; break;
            case ADD: A = A + B;          break;
            case SUB: A = A - B;          break;
            case STA: mem[mem[PC++]] = A; break;
            case JMP: PC = mem[PC];       break;
            case MUL: A = A * B;          break;
            case PRT: printf("%d\n", mem[mem[PC++]]); break;
            case PRS: {
                uint16_t addr = mem[PC++];
                while (mem[addr]) putchar(mem[addr++]);
                putchar('\n');
                break;
                }
            case HLT: return;
            default:
                printf("Unknown opcode 0x%02X at PC=%d\n", op, PC-1);
                return;
        }
        //printf("PC=%03d  A=%03d  B=%03d\n", PC, A, B); Not needed for normal use; only for debugging
    }
}

int main() {
    // CPU program
    mem[0x80] = 152;
    mem[0x81] = 3;

    mem[0x90] = 'I';
    mem[0x91] = ' ';
    mem[0x92] = 'M';
    mem[0x93] = 'a';
    mem[0x94] = 'd';
    mem[0x95] = 'e';
    mem[0x96] = ' ';
    mem[0x97] = 'M';
    mem[0x98] = 'y';
    mem[0x99] = ' ';
    mem[0x9A] = 'O';
    mem[0x9B] = 'w';
    mem[0x9C] = 'n';
    mem[0x9D] = ' ';
    mem[0x9E] = 'C';
    mem[0x9F] = 'P';
    mem[0xA0] = 'U';
    mem[0xA1] = '\0';

    mem[0x00] = LDA; mem[0x01] = 0x80;
    mem[0x02] = LDB; mem[0x03] = 0x81;
    mem[0x04] = ADD;
    mem[0x05] = STA; mem[0x06] = 0x82;
    mem[0x07] = LDA; mem[0x08] = 0x80;
    mem[0x09] = LDB; mem[0x0A] = 0x81;
    mem[0x0B] = MUL;
    mem[0x0C] = STA; mem[0x0D] = 0x83;
    mem[0x0E] = PRS; mem[0x0F] = 0x90;
    mem[0x10] = PRT; mem[0x11] = 0x82;
    mem[0x12] = PRT; mem[0x13] = 0x83;
    mem[0x14] = HLT;

    run();
    int has_data = 0;
    for (int i = 0; i < 256; i++)
        if (mem[FB_START + i]) { has_data = 1; break; }

    if (has_data) {
            printf("VRAM:\n");
            for (int i = 0; i < 16; i++) {
                for (int j = 0; j < 16; j++)
                    printf("%3d ", mem[FB_START + i*16 + j]);
                printf("\n");
            }
        }
    //printf("0x82: %d\n", mem[0x82]);
    //printf("0x83: %d\n", mem[0x83]);


    return 0;
}
