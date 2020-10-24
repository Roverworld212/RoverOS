#ifndef KUTIL_H
#define KUTIL_H

#include "types.h"
#include "libvalloc.h"

extern void getCPUI();

extern void kernelPanic_int(char *caller, char *file, uint32 line, char *reason);

#define kernelPanic(reason) kernelPanic_int(__FUNCTION__, __FILE__, __LINE__, reason)

//Runtime args

typedef struct kvcpuidS{
    uint8 vendor[11]; //Vendor name
    uint32 vendorid; //Unique Vendor ID calculated using Vendor name ebx+ecx+edx
}kvcpuidStruct;

typedef struct kvuiS{
    uint32 mode; //Video Mode
    uint32 xsz; //X-Res
    uint32 ysz; //Y-Res
    uint32 bpp; //Bits per pixel
    uint32 pitch; //pitch
    uint32 buffer;
    vui *pfui; //Current UI to be used
}kvuiStruct;

typedef struct kvioS{
    //Qemu
    bool log;
    bool debugOut; //Write debug statments
    bool infoOut; //Write info statments
    bool warnOut; //Write warn statments
    bool errorOut; //Write error statments
    bool panicOut; //Write kernelpanic statment
    //Alloc
    bool kmallocLog;
    bool mallocLog;
    //Paging
    bool clearsetLog;
    bool printFrames;
    //Keys
    bool caps;
    bool shift;
    bool fn;
    //Libhook
    bool disableHooks;
}kvioStruct;

typedef struct kvfsS{
    uint32 rdStart; //Ramdisk load addr
}kvfsStruct;

typedef struct kvACPIS{
    bool version : 1;
    bool maskAPIC : 1;
    //FACP
    bool FACPSigValid : 1;
    //APIC
    bool APICSigValid : 1;
    //HPET
    bool HPETSigValid : 1;
    //ACPI
    uint32 flags; //What tables were found
}kACPIStruct;

typedef struct kvMemS{
    uint64 free;
    uint64 reserved;
    uint64 acpi;
    uint64 nvs;
    uint64 badram;
    uint64 unknown;
}kvMem;

/*flags
Bit 0: FACP
Bit 1: APIC
Bit 2: HPET
*/

typedef struct kvinfS{
    kvioStruct kio;
    kvuiStruct kui;
    kvcpuidStruct kcpuid;
    kvfsStruct kvfs;
    kACPIStruct kACPI;
    kvMem kmem;
    char *kernelname;
}kvinfStruct;

kvinfStruct kvinf;

vui kvui;

#endif