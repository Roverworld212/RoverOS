#ifndef PMALLOC_H
#define PMALLOC_H

#include "types.h"

struct kblock{ //Block of memory
    uint32 size; //Size of allocated memory
    uint16 flags; //Flags
    uint64 *memory; //Pointer to memory
    uint64 *next; //Next structure
    uint64 *previous; //Previous structure
};

#define KBF_INUSE 1<<0
#define KBF_SPARE 1<<1
#define KBF_DESTROY 1<<2

extern void *pmalloc(uint32 size, uint16 flags);
extern void pmallocInit(uint64 mstart);

#endif