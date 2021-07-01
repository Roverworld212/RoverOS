#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "types.h"

struct instructionS{
    //REX byte
    uint8 magic : 4;
    bool x64 : 1;
    bool modrm : 1;
    bool sib : 1;
    bool ext : 1;
    //Opcode
    uint8 opcode : 8;
    //modrm
    uint8 mod : 2;
    uint8 reg : 3;
    uint8 rm : 3;
    //Instructions
}__attribute__((packed));

typedef struct instructionS instruction;

extern void view_instructions(uint64 rip);
extern void read_instructions(uint8 *m);
#endif