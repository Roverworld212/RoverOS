#ifndef KQEMU_H
#define KQEMU_H

#define CFG_SEL 0x510
#define CFG_DATA 0x511
#define CFG_DMA 0x514
#define CFG_SIG 0x0000
#define CFG_ID 0x0001
#define CFG_FILE 0x0019

const char tcgSig[12] = {'T', 'C', 'G', 'T', 'C', 'G', 'T', 'C', 'G', 'T', 'C', 'G'};

typedef struct CFGFileS{
    uint32 sz;
    uint16 sel;
    uint16 res;
    char name[56];
}CFGFile;

#endif