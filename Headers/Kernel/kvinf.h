#ifndef KVINF_H
#define KVINF_H

#include "types.h"

struct kvcpuid{
    char brand[12];
};

struct kvvideo{
    uint64 physical;
    uint64 width;
    uint64 height;
    uint64 bpp;
    uint64 pitch;
};

struct kvio{

};

struct kvfs{

};

struct kvacpi{

};

struct kvmem{

};

struct kvkernel{

};

struct kvmultiboot{

};

struct kvinfS{
    struct kvcpuid cpuid; //For CPUID related info
    struct kvvideo video; //For VESA/VBE related info
    struct kvio io; //For IO info
    struct kvfs fs; //For Filesystem info
    struct kvacpi acpi; //For ACPI related info
    struct kvmem memory; //For memory related info
    struct kvmultiboot multiboot; //For multiboot related info
    struct kvkernel kernel; //For kernel info
};

struct kvinfS kvinf;

#endif