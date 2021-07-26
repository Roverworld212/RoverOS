#ifndef LIBPNG_H
#define LIBPNG_H

#include "types.h"

struct IHDR{
    uint16 width;
    uint16 height;
    uint8 depth;
    uint8 color;
    uint8 compression;
    uint8 filter;
    uint8 interlacing;
}__attribute__((packed));

struct PLTE{
    uint8 r;
    uint8 g;
    uint8 b;
}__attribute__((packed));

struct IDAT{
    uint32 magic;
}__attribute__((packed));

struct IEND{
    uint32 magic;
}__attribute__((packed))

#endif