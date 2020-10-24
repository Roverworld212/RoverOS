#include "kheaders.h"
#include "libacpi.h"
#include "libbin.h"
#include "rcom.h"

struct RSDPStruct *acpiold;
struct RSDP2Struct *acpi;
struct RSDTStruct *rsdt;
struct XSDTStruct *xsdt;

bool checkSig(uint32 l, char *str, char *cmp){
    for(uint32 i = 0; i < l; ++i){if(str[i] != cmp[i]){return false;}}
    return true;
}

void setFlag(uint32 n){
    kvinf.kACPI.flags = kvinf.kACPI.flags | (BCHECK32>>n);
}

void parseFACP(uint32 *p){
    struct FACPStruct *facp = (struct FACPStruct*)p;
    printq("==FACP Table==\n");
    if(checkSig(4, facp->Signature, "FACP ") == false){printq("Invalid Signature\n"); return;}
    printq("Len: %x\nRevision: %x\nCheck: %x\n", facp->len, facp->rev, facp->check);
    setFlag(0);
}

void parseAPIC(uint32 *p){
    printq("==APIC Table==\n");
    struct MADTStruct *apic = (struct MADTStruct*)p;
    if(checkSig(4, apic->Signature, "APIC ") == false){printq("Invalid Signature\n"); return;}
    printq("Len: %x\nRevision: %x\nCheck: %x\nOEM ID: ", apic->len, apic->rev, apic->check);
    //for(uint32 i = 0; i < 6; ++i){sendData(COM1, apic->OEMID[i]);}
    printq("\nOEM Table: \n");
    printq("OEM Revision: %x\nCreator ID: %x\nCreator Revision: %x\n", apic->OEMRev, apic->CreatorID, apic->CreatorRev);
    printq("Lapic Addr: %x\nFlags: ", apic->lapicAddr);
    for(uint32 i = 0; i < 32; ++i){
        if(BCHECK32>>i & apic->Flags){printq("1");}else{printq("0");}
    }
    printq("\n");
    setFlag(1);
}

void parseHPET(uint32 *p){
    printq("==HPET Table==\n");
    struct HPETStruct *hpet = (struct HPETStruct*)p;
    if(checkSig(4, hpet->Signature, "HPET ") == false){printq("Invalid Signature\n"); return;}
    printq("Len: %x\nRevision: %x\nCheck: %x\nOEM ID:", hpet->len, hpet->rev, hpet->check);
    //for(uint32 i = 0; i < 6; ++i){sendData(COM1, hpet->OEMID[i]);}
    printq("\nOEM Table: ");
    //for(uint32 i = 0; i < 8; ++i){sendData(COM1, hpet->OEMTable[i]);}
    printq("\nCreator ID: %x\nCreator Revision: %x\n", hpet->CreatorID, hpet->CreatorRev);
    printq("PCI ID: %x\nLegacy Replacment IRQ Routing Capable: %b\n", hpet->PCI_ID, hpet->IRQRouteCapable);
    printq("Count Cap: %b\n", hpet->CountCap);
    printq("Number of comparators: %x\n", hpet->CompNum);
    printq("Hardware Revision ID: %x\n", hpet->HardwareRevID);
    printq("Base: %x\nHPET Number: %x\n", hpet->BaseAddress, hpet->HPETNumber);
    printq("Main Counter Minimum: %x\n", hpet->MinClockTick);
    printq("Page Protection Flags: "); for(uint32 i = 0; i < 8; ++i){if((BCHECK8>>i) & hpet->PageProtection){printq("1");}else{printq("0");}}
    printq("\n");
    for(uint32 i = 0; i < 8; ++i){
        if((BCHECK8>>i) & hpet->PageProtection){
            switch(i){
                case 0:
                printq("No Garunteed Page Protection\n");
                break;
                case 1:
                printq("4KB Page Protected Access\n");
                break;
                case 2:
                printq("64KB Page Protected Access\n");
                break;
            }
        }
    }
    setFlag(3);
}

void parseACPI(bool new, uint8 *rdsp){
    if(new){
        //Add ACPI 2.0 support here
        kernelPanic("ACPI 2.0 is not supported in this version of RoverOS");
    }
    acpiold = (struct RSDPStruct*)rdsp;
    if(checkSig(8, acpiold->Signature, "RSD PTR ") == false){kernelPanic("Invalid ACPI signature");}
    printq("Check: %x\nOEM: ", acpiold->check);
    for(uint32 i = 0; i < 6; ++i){
        sendData(COM1, acpiold->OEM[i]);
    }
    printq("\nRevision: %x\n", acpiold->rev);
    printq("Address: %x\n", acpiold->addr);
    kvinf.kACPI.version = 0;
    rsdt = (struct RSDTStruct*)acpiold->addr;
    printq("-Descriptor Table-\n");
    if(checkSig(4, rsdt->Signature, "RSDT ") == false){kernelPanic("Invalid RSDT signature");}
    printq("Len: %x\nRevision: %x\nCheck: %x\nOEM: ", rsdt->len, rsdt->rev, rsdt->check);
    for(uint32 i = 0; i < 6; ++i){sendData(COM1, rsdt->OEM[i]);}
    printq("\nOEM TableID: ");
    for(uint32 i = 0; i < 8; ++i){sendData(COM1, rsdt->OEMTable[i]);}
    printq("\nOEM Revision: %x\nCreatorID: %x\nCreator Revision: %x\n", rsdt->OEMRev, rsdt->CreatorID, rsdt->CreatorRev);
    uint32 tables = (rsdt->len-sizeof(struct RSDTStruct))/4;
    printq("Other tables: %x\n", tables);
    uint32 *ts = (uint32*)(acpiold->addr+sizeof(struct RSDTStruct));
    struct RSDTStruct *table;
    uint32 c = 0;
    for(uint32 i = 0; i < tables; ++i){
        table = (struct RSDTStruct*)ts[i];
        for(uint32 b = 0; b < 4; ++b){c += (uint8)table->Signature[b];}
        switch(c){
            case 0x11a: //FACP
            parseFACP(ts[i]);
            break;
            case 0x11d: //APIC
            parseAPIC(ts[i]);
            break;
            case 0x131: //HPET
            parseHPET(ts[i]);
            break;
            default:
            printq("%wUnkown Table - %x\n", __F__, c);
            printq("Signature: ");
            for(uint32 g = 0; g < 4; ++g){sendData(COM1, table->Signature[g]);}
            printq("\n");
            break;
        }
        c = 0;
    }
}