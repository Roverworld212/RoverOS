#ifndef ACPI_H
#define ACPI_H

#include "types.h"

extern void readACPI();

struct rdsp{
    char signature[8];
    uint8 check;
    char OEM[6];
    uint8 revision;
    uint32 address;
}__attribute__((packed));

struct rdsp2{
    struct rdsp old;
    uint32 len;
    uint64 xsdt;
    uint8 extendedCheck;
    uint8 reserved[3];
}__attribute__((packed));

struct rsdt{
    char signature[4];
    uint32 len;
    uint8 rev;
    uint8 check;
    char OEM[6];
    char OEMT[8];
    uint32 OEMRev;
    uint32 CreatorID;
    uint32 CreatorRev;
}__attribute__((packed));

//APIC

struct apic{
    char signature[4];
    uint32 len;
    uint8 rev;
    uint8 check;
    char OEMID[6];
    char OEMT[8];
    uint32 OEMRev;
    uint32 CreatorID;
    uint32 CreatorRev;
    uint32 lapic;
    uint32 flags;
}__attribute__((packed));

//FACP

struct address{
    uint8 AddressSpace;
    uint8 BitWidth;
    uint8 BitOffset;
    uint8 AccessSize;
    uint64 Addr;
}__attribute__((packed));

struct facp{
    char Signature[4];
    uint32 len;
    uint8 rev;
    uint8 check;
    char OEMID[6];
    char OEMT[8];
    uint32 OEMRev;
    uint32 CreatorID;
    uint32 CreatorRev;
    uint32 FirmwareCtrl;
    uint32 DSDT;
    uint8 Reserved0; //ACPI 1.0 Only
    uint8 PPMP;
    uint16 SCI_Interrupt;
    uint32 SMI_CommandPort;
    uint8 ACPIEnable;
    uint8 ACPIDisable;
    uint8 S4BIOS_REQ;
    uint8 PSTATEControl;
    uint32 PM1AEvent;
    uint32 PM1BEvent;
    uint32 PM1AControl;
    uint32 PM1BControl;
    uint32 PM2Control;
    uint32 PMTimer;
    uint32 GPE0;
    uint32 GPE1;
    uint8 PM1ELen;
    uint8 PM1CLen;
    uint8 PMTimerLen;
    uint8 GPE0L;
    uint8 GPE1L;
    uint8 GPE1Base;
    uint8 CStateControl;
    uint16 WC2L;
    uint16 WC3L;
    uint16 FSize;
    uint16 FStride;
    uint8 DutyOffset;
    uint8 DayAlarm;
    uint8 MonthAlarm;
    uint8 Century;
    uint16 BootArchFlags; //Reserved in ACPI 1.0
    uint8 Reserved;
    uint32 Flags;
    struct address ResetReg;
    uint8 ResetValue;
    uint8 Reserved2[3];
    uint64 XFirmwareCtrl; //ACPI 2.0
    uint64 XDSDT; //ACPI 2.0
    struct address XPM1E;
    struct address XPM2E;
    struct address XPM1C;
    struct address XPM2C;
    struct address XPMT;
    struct address XGPE0;
    struct address XGPE1;
}__attribute__((packed));

//HPET

struct hpet{
    char Signature[4];
    uint32 len;
    uint8 rev;
    uint8 check;
    char OEMID[6];
    char OEMTable[8];
    uint32 CreatorID;
    uint32 CreatorRev;
    uint8 HardwareRevID;
    uint8 Comparators : 5;
    uint8 size : 1;
    uint8 reserved : 1;
    uint8 legacy : 1;
    uint16 PCI;
    struct address addr;
    uint8 hpetNum;
    uint16 minTick;
    uint8 pageProtection;
}__attribute__((packed));

#endif