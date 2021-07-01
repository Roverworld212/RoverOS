#ifndef KMEMORY_H
#define KMEMORY_H

#include "types.h"

typedef struct mregionS{
    uint64 blocks; //bitmap of free/used blocks
    uintptr *next; //Next region block
}mregion;

typedef struct mblockS{
    uint64 size; //Total size
    uint64 used; //Used size
    uintptr *next; //Next block in the chain
    uint8 flags; //Reserved for future use
}mblock;

#endif