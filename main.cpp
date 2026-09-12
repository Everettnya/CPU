#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "common.h"
void assemble(const char *filename);

uint16_t mem[MEM_SIZE] = {0};

// CPU opcodes
#define NOP  0x00  // no operation
#define LDA  0x01  // load mem[arg] into A
#define LDB  0x02  // load mem[arg] into B
#define ADD  0x03  // A = A + B
#define SUB  0x04  // A = A - B
#define STA  0x05  // store A into mem[arg]
#define JMP  0x06  // unconditional jump to arg
#define MUL  0x07  // A = A * B
#define PRT  0x08  // print mem[arg] as number
#define PRS  0x09  // print string at mem[arg], with newline
#define INS  0x0A  // read string from stdin into mem[arg]
#define INN  0x0B  // read number from stdin into mem[arg]
#define PRN  0x0C  // print string at mem[arg], no newline
#define JZ   0x0D  // jump to arg if A == 0
#define JNZ  0x0E  // jump to arg if A != 0
#define STB  0x10  // store B into mem[arg]
#define HLT  0xFF  // halt

// CPU Registers and counters
uint16_t A = 0, B = 0, PC = 0;

void run() {
    while (1) {
        uint8_t op = mem[PC++];
        switch (op) {
            case NOP: break;

            // indirect — operand is a pointer to the data
            case LDA: A = mem[mem[PC++]];              break;
            case LDB: B = mem[mem[PC++]];              break;
            case STA: mem[mem[PC++]] = A;              break;
            case STB: mem[mem[PC++]] = B;              break;
            case PRT: printf("%d\n", mem[mem[PC++]]);  break;
            case INN: {
                uint16_t addr = mem[PC++];
                int val;
                scanf("%d", &val);
                mem[addr] = (uint16_t)val;
                break;
            }

            // direct — operand is the address itself
            case PRS: {
                uint16_t addr = mem[PC++];
                while (mem[addr]) putchar(mem[addr++]);
                putchar('\n');
                break;
            }
            case PRN: {
                uint16_t addr = mem[PC++];
                while (mem[addr]) putchar(mem[addr++]);
                break;
            }
            case INS: {
                uint16_t addr = mem[PC++];
                char buf[256];
                fgets(buf, sizeof(buf), stdin);
                buf[strcspn(buf, "\n")] = 0;
                for (int i = 0; buf[i]; i++)
                    mem[addr + i] = (uint16_t)buf[i];
                mem[addr + strlen(buf)] = 0;
                break;
            }

            // direct — operand is a code address
            case JMP: PC = mem[PC];                        break;
            case JZ:  PC = (A == 0) ? mem[PC] : PC + 1;   break;
            case JNZ: PC = (A != 0) ? mem[PC] : PC + 1;   break;

            case ADD: A = A + B; break;
            case SUB: A = A - B; break;
            case MUL: A = A * B; break;

            case HLT: return;
            default:
                printf("Unknown opcode 0x%02X at PC=%d\n", op, PC-1);
                return;
        }
    }
}

int main() {
    // CPU program
    /*
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
    mem[0x12] = PRT; mem[0x13] = 0x81;
    mem[0x14] = HLT;
    */

    assemble("program.asm");
    printf("mem[0x13]=%d  mem[0x81]=%d\n", mem[0x13], mem[0x81]);
    //^ only for debugging istg if i have to uncomment that piece of shit one more fucking time
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
