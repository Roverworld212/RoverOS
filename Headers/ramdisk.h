#ifndef RAMDISK_H
#define RAMDISK_H

#include "types.h"

#define RDMASTERMAGIC 0xFB
#define RDMAGIC 0xFA

struct masterHeader{
    uint8 magic;
    uint8 files;
    uint8 sz;
}__attribute__((packed));

struct ramHeader{
    uint8 magic;
    char name[19];
    uint8 off;
    uint32 len;
}__attribute__((packed));

//FS Stuff

#endif