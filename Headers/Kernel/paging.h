#ifndef PAGING_H
#define PAGING_H

#include "types.h"

#define PAGE_SZ 0x1000

#define PAGE_MASK2 0x000ffffffffff000 //Thanks u/Mai_Lapyst
#define PAGE_MASK 0xFFFFFFFFFFFFF000

#define GET_PG_BASE(p) ((p&PAGE_MASK)>>12)
#define GET_PT_BASE(d) ((d&PAGE_MASK)>>12)
#define GET_PDE_BASE(d) ((d&PAGE_MASK)>>12)
#define GET_PDPE_BASE(d) ((d&PAGE_MASK)>>12)

#define ALIGN(addr) ((((uint64)addr)&0xFFFFF000)+0x1000) //Thanks u/devjustinian

#define SET_PAE(x) (x|(FSET32>>5))
#define SET_PE(x) (x|FSET32)

//Paging macros

#define GET_PT(v) (v>>12)&0x1FF
#define GET_PDE(v) (v>>21)&0x1FF
#define GET_PDPE(v) (v>>30)&0x1FF
#define GET_PML(v) (v>>39)&0x1FF

#define PRESENT 0x1
#define RW 0x1<<1
#define USR 0x1<<2
#define PWT 0x8
#define PCT 0x4
#define ACC 0x2
#define PSK 0x0
#define NX 1<<63

struct pml4eS{
    uint8 flags : 8;
    bool MBZ : 1;
    uint8 free : 3;
    uint64 baseAddress : 41;
    uint16 avl : 10;
    bool nx : 1;
}__attribute__((packed));

struct pdpeS{
    uint8 flags : 8;
    bool IGN : 1;
    uint8 free : 3;
    uint64 baseAddress : 41;
    uint16 avl : 10;
    bool nx : 1;
}__attribute__((packed));

struct pdeS{
    uint8 flags : 8;
    bool IGN : 1;
    uint8 free : 3;
    uint64 baseAddress : 41;
    uint16 avl : 10;
    bool nx : 1;
}__attribute__((packed));

struct ptS{
    uint8 flags : 8;
    bool G : 1;
    uint8 free : 3;
    uint64 baseAddress : 41;
    uint16 avl : 6;
    uint16 PKE : 4;
    bool nx : 1;
}__attribute__((packed));

struct addressSpaceS{
    uint64 pml[512] __attribute__((aligned (PAGE_SZ)));
    uint64 pdpe[512] __attribute__((aligned (PAGE_SZ)));
    uint64 pde[512] __attribute__((aligned (PAGE_SZ)));
    uint64 pt[512] __attribute__((aligned (PAGE_SZ)));
}__attribute__((packed));

#endif