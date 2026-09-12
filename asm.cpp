#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "common.h"

extern uint16_t mem[];

struct Op { const char *name; uint16_t code; int args; };
static Op ops[] = {
    {"NOP", 0x00, 0}, {"LDA", 0x01, 1}, {"LDB", 0x02, 1},
    {"ADD", 0x03, 0}, {"SUB", 0x04, 0}, {"STA", 0x05, 1},
    {"JMP", 0x06, 1}, {"MUL", 0x07, 0}, {"PRT", 0x08, 1},  // MUL takes 0 args — uses A and B
    {"PRS", 0x09, 1}, {"HLT", 0xFF, 0}, {"INS", 0x0A, 1},
    {"INN", 0x0B, 1}, {"PRN", 0x0C, 1}, {"JZ", 0x0D, 1},
    {"JNZ", 0x0E, 1}, {"STB", 0x10, 1},
    {NULL, 0, 0}
};

static uint16_t parse_val(const char *s) {
    if (s[0] == '0' && s[1] == 'x') return (uint16_t)strtol(s, NULL, 16);
    return (uint16_t)atoi(s);
}

void assemble(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("can't open %s\n", filename); exit(1); }

    char line[256];
    int mode = 0;
    uint16_t pc = 0x00;

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == ';' || *p == '\0') continue;

        if (strcmp(p, ".data") == 0) { mode = 1; continue; }
        if (strcmp(p, ".code") == 0) { mode = 2; pc = 0x00; continue; }

        if (mode == 1) {
            char addr_s[32], rest[224];
            if (sscanf(p, "%31s %223[^\n]", addr_s, rest) < 2) continue;
            uint16_t addr = parse_val(addr_s);

            if (rest[0] == '"') {
                char *s = rest + 1;
                while (*s && *s != '"')
                    mem[addr++] = (uint16_t)*s++;
                mem[addr] = 0;
            } else {
                mem[addr] = parse_val(rest);
            }
        } else if (mode == 2) {
            char tok[32] = {0}, a1[32] = {0}, a2[32] = {0};
            int n = sscanf(p, "%31s %31s %31s", tok, a1, a2);
            if (n < 1) continue;
            if (tok[0] == ';') continue;

            Op *op = ops;
            while (op->name && strcmp(op->name, tok) != 0) op++;
            if (!op->name) { printf("unknown opcode: %s\n", tok); continue; }

            mem[pc++] = op->code;
            if (op->args >= 1) mem[pc++] = parse_val(a1);
            if (op->args >= 2) mem[pc++] = parse_val(a2);
        }
    }
    fclose(f);
}
