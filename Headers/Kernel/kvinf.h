#ifndef KVINF_H
#define KVINF_H

#include "types.h"
#include "libtask.h"
#include "multiboot.h"

struct kvcpuid{
    char brand[12];
};

struct kvvideo{
    uint64 physical;
    uint32 width;
    uint32 height;
    uint32 bpp;
    uint32 pitch;
};

struct kvio{
    bool debug;
    bool caps;
    bool shift;
};

struct kvfs{
    uint64 *ramdisk;
    uint64 *ramdiskEnd;
    uint64 *vfs;
};

struct kvacpi{
    bool new;
    uint64 *rdsp;
    bool switched;
    struct{
        bool avalible;
        bool IRQCapable;
    }hpet;
};

struct kvmem{
    uint64 total;
};

struct kvkernel{
    bool timelog;
    uint16 tick; //Reset after each new second
    uint32 second;
    uint32 minute;
    uint32 hour;
};

struct kvtask{
    struct task *currentTask;
};

struct kvaudio{
    //PC Speaker
    bool state; //0-out 1-in
    //SB16
    //Intel HD
};

struct kvinfS{
    struct kvcpuid cpuid; //For CPUID related info
    struct kvvideo video; //For VESA/VBE related info
    struct kvio io; //For IO info
    struct kvfs fs; //For Filesystem info
    struct kvacpi acpi; //For ACPI related info
    struct kvmem memory; //For memory related info
    struct kvtask task;
    struct kvkernel kernel; //For kernel info
};

struct kvinfS kvinf;

#endif