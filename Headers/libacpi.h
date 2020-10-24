#ifndef LIBACPI_H
#define LIBACPI_H

#include "types.h"

//ACPI

struct RSDPStruct{ //ACPI 1.0 Pointer Struct
    char Signature[8];
    uint8 check;
    char OEM[6];
    uint8 rev;
    uint32 addr;
}__attribute__((packed));

struct RSDP2Struct{ //ACPI 2.0 Pointer struct
    struct RSDPStruct base;
    uint32 len;
    uint64 XSDT;
    uint8 reserved[3]; 
}__attribute__((packed));

struct RSDTStruct{
    char Signature[4];
    uint32 len;
    uint8 rev;
    uint8 check;
    char OEM[6];
    char OEMTable[8];
    uint32 OEMRev;
    uint32 CreatorID;
    uint32 CreatorRev;
}__attribute__((packed));

struct XSDTStruct{ //Extended ACPI Struct
    char Signature[4];
    uint32 len;
    uint8 rev;
    uint8 check;
    char OEM[6];
    char OEMTable[8];
    uint32 OEMRev;
    uint32 CID;
    uint32 CRev;
}__attribute__((packed));

//APIC

struct MADTStruct{
    char Signature[3];
    uint32 len;
    uint8 rev;
    uint8 check;
    char OEMID[6];
    char OEMTable[8];
    uint32 OEMRev;
    uint32 CreatorID;
    uint32 CreatorRev;
    uint32 lapicAddr;
    uint32 Flags;
}__attribute__((packed));

//FACP (FADT)

struct AddressStruct{
    uint8 AddressSpace;
    uint8 BitWidth;
    uint8 BitOffset;
    uint8 AccessSize;
    uint64 Addr;
}__attribute__((packed));

struct FACPStruct{
    char Signature[3];
    uint32 len;
    uint8 rev;
    uint8 check;
    char OEMID[6];
    char OEMTable[8];
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
    struct AddressStruct ResetReg;
    uint8 ResetValue;
    uint8 Reserved2[3];
    uint64 XFirmwareCtrl; //ACPI 2.0
    uint64 XDSDT; //ACPI 2.0
    struct AddressStruct XPM1E;
    struct AddressStruct XPM2E;
    struct AddressStruct XPM1C;
    struct AddressStruct XPM2C;
    struct AddressStruct XPMT;
    struct AddressStruct XGPE0;
    struct AddressStruct XGPE1;
}__attribute__((packed));

//HPET

struct HPETStruct{
    char Signature[4];
    uint32 len;
    uint8 rev;
    uint8 check;
    char OEMID[6];
    char OEMTable[8];
    uint32 CreatorID;
    uint32 CreatorRev;
    uint32 PCI_ID : 16;
    bool IRQRouteCapable : 1;
    bool Reserved : 1;
    bool CountCap : 1;
    uint8 CompNum : 5;
    uint8 HardwareRevID : 8;
    uint32 BaseAddress;
    uint8 HPETNumber;
    uint16 MinClockTick;
    uint8 PageProtection;
}__attribute__((packed));

#endif