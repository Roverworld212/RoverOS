#ifndef GDT_H
#define GDT_H

//For the 64 bit GDT

struct tss{
    uint32 reserved1 : 32;
    uint64 rsp0 : 64;
    uint64 rsp1 : 64;
    uint64 rsp2 : 64;
    uint64 reserved2 : 64;
    uint64 ist0 : 64;
    uint64 ist1 : 64;
    uint64 ist2 : 64;
    uint64 ist3 : 64;
    uint64 ist4 : 64;
    uint64 ist5 : 64;
    uint64 ist6 : 64;
    uint64 ist7 : 64;
    uint64 reserved3 : 64;
    uint16 reserved4 : 16;
    uint16 iopb : 16;
}__attribute__((packed));

struct gdtp{
    uint16 limit;
    uint64 base;
}__attribute__((packed));

struct gdte{
    uint16 limit_upper : 16;
    uint16 base_upper : 16;
    uint16 base_middle : 16;
    bool a : 1;
    bool r : 1;
    bool c : 1;
    uint32 a1 : 2;
    uint32 dpl : 2;
    bool present : 1;
    uint16 limit_lower : 16;
    bool avl : 1;
    bool longMode : 1;
    bool d : 1; //0 when long mode
    bool granularity : 1;
    uint32 base_lower : 32;
}__attribute__((packed));

#endif