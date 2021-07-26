#ifndef ACPI_H
#define ACPI_H

#include "types.h"

extern void readACPI();

struct rdsp{
    char signature[8];
    uint8 check;
    char OEM[6];
    uint16 revision;
    uint32 address;
}__attribute__((packed));

struct rdsp2{
    struct rdsp old;
    uint32 len;
    uint64 xsdt;
    uint8 extendedCheck;
    uint8 reserved[3];
}__attribute__((packed));

#endif