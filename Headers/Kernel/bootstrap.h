#ifndef KBOOTSTRAP_H
#define KBOOTSTRAP_H

#include "types.h"

struct bstinfoS{ //Informatiob passed to kmain by bootstrap code
    uint64 mmagic; //Multiboot magic
    uint64 mstart; //Multiboot tag start
    uint16 segment; //Sys segment for IDT
}__attribute__((packed));

typedef struct bstinfoS bstinfo;

#endif