#include "libstd.h"
#include "libstdio.h"
#include "libio.h"
#include "libksystem.h"
#include "kvinf.h"
#include "acpi.h"
#include "paging.h"

bool checksig(uint32 l, char *str, char *cmp){
    for(uint32 i = 0; i < l; ++i){if(str[i] != cmp[i]){return false;}}
    return true;
}

void printsig(uint8 *sig, uint16 len){
    for(uint16 i = 0; i < len; ++i){
        printq("%c", sig[i]);
    }
    printq("\n");
}

void readFACP(uint32 *p){
    printq("==========FACP==========\n");
    struct facp *f = (struct facp*)p;
    printq("Signature: "); printsig(f->Signature, 4);
    printq("Len: 0x%lx\nRevision: 0x%hx\nCheck: 0x%hx\n", f->len, f->rev, f->check);
    printq("OEM: "); printsig(f->OEMID, 6);
    printq("OEMTable: "); printsig(f->OEMT, 8);
    printq("OEM revision: 0x%lx\n", f->OEMRev);
    printq("CreatorID: 0x%lx\n", f->CreatorID);
    printq("Creator revision: 0x%lx\n", f->CreatorRev);
    printq("Firmware control: 0x%lx\n", f->FirmwareCtrl);
    printq("DSDT: 0x%lx\n", f->DSDT);
    printq("PPMP: 0x%hx\n", f->PPMP);
    printq("SCI Interrupt: 0x%lx\n", (uint32)f->SCI_Interrupt);
    printq("SMI Command port: 0x%lx\n", f->SMI_CommandPort);
    printq("ACPI Enable: 0x%hx\n", f->ACPIEnable);
    printq("ACPI Disable: 0x%hx\n", f->ACPIDisable);
    printq("S4BIOS: 0x%hx\n", f->S4BIOS_REQ);
    printq("PSTATE: 0x%hx\n", f->PSTATEControl);
    printq("PM1AE: 0x%lx\n", f->PM1AEvent);
    printq("PM1BE: 0x%lx\n", f->PM1BEvent);
    printq("PM1AC: 0x%lx\n", f->PM1AControl);
    printq("PM1BC: 0x%lx\n", f->PM1BControl);
    printq("PM2C: 0x%lx\n", f->PM2Control);
    printq("PM Timer: 0x%lx\n", f->PMTimer);
    printq("GPE0: 0x%lx\n", f->GPE0);
    printq("GPE1: 0x%lx\n", f->GPE1);
    printq("PM1EL: 0x%hx\n", f->PM1ELen);
    printq("PM1CL: 0x%hx\n", f->PM1CLen);
    printq("PM Timer len: 0x%hx\n", f->PMTimerLen);
    printq("GPE0L: 0x%hx\n", f->GPE0L);
    printq("GPE1L: 0x%hx\n", f->GPE1L);
    printq("GPE1B: 0x%hx\n", f->GPE1Base);
    printq("CState: 0x%hx\n", f->CStateControl);
    printq("WC2L: 0x%lx\n", (uint32)f->WC2L);
    printq("WC3L: 0x%lx\n", (uint32)f->WC3L);
    printq("FSize: 0x%lx\n", (uint32)f->FSize);
    printq("FStride: 0x%lx\n", (uint32)f->FStride);
    printq("Duty offset: 0x%hx\n", f->DutyOffset);
    printq("Day alarm: 0x%hx\n", f->DayAlarm);
    printq("Month alarm: 0x%hx\n", f->MonthAlarm);
    printq("Century: 0x%hx\n", f->Century);
    printq("Flags: 0x%lx\n", f->Flags);
}

void readAPIC(uint32 *p){
    printq("==========APIC==========\n");
    struct apic *a = (struct apic*)p;
    printq("Signature: "); printsig(a->signature, 4);
    printq("Len: 0x%lx\nRevision: 0x%hx\nCheck: 0x%hx\n", a->len, a->rev, a->check);
    printq("OEM: "); printsig(a->OEMID, 6);
    printq("OEMTable: "); printsig(a->OEMT, 8);
    printq("OEM revision: 0x%lx\n", a->OEMRev);
    printq("CreatorID: 0x%lx\n", a->CreatorID);
    printq("Creator revision: 0x%lx\n", a->CreatorRev);
    printq("Lapic: 0x%lx\n", a->lapic);
    printq("Flags: 0x%lx\n", a->flags);
}

void readHPET(uint32 *p){
    printq("==========HPET==========\n");
    struct hpet *h = (struct hpet*)p;
    printq("Signature: "); printsig(h->Signature, 4);
    printq("Len: 0x%lx\nRevision: 0x%hx\nCheck: 0x%hx\n", h->len, h->rev, h->check);
    printq("OEM: "); printsig(h->OEMID, 6);
    printq("OEMTable: "); printsig(h->OEMTable, 8);
    printq("CreatorID: 0x%lx\nCreator revision: 0x%lx\n", h->CreatorID, h->CreatorRev);
    printq("Hardware revision: 0x%hx\n", h->HardwareRevID);
    printq("Comparators: 0x%hx\n", (uint8)h->Comparators);
    printq("Size: 0x%hx\n", (uint8)h->size);
    printq("Legacy: 0x%hx\n", (uint8)h->legacy);
    printq("PCI: 0x%lx\n", (uint16)h->PCI);
    printq("Address space: 0x%hx\n", h->addr.AddressSpace);
    printq("Bit width: 0x%hx\n", h->addr.BitWidth);
    printq("Bit offset: 0x%hx\n", h->addr.BitOffset);
    printq("Access size: 0x%hx\n", h->addr.AccessSize);
    printq("Address: 0x%llx\n", h->addr.Addr);
    printq("HPET Number: 0x%hx\n", h->hpetNum);
    printq("Minimum tick: 0x%hx\n", h->minTick);
    printq("Page protection: 0x%hx\n", h->pageProtection);
}

void readDSDT(uint32 *p){
    return;
}

void readACPI(){
    printq("==========ACPI==========\n");
    printq("%qReading ACPI\n", QINFO);
    if(kvinf.acpi.new == true){
        printq("%qACPI 2 is not supported\n", QWARNING);
        return;
    }
    struct rdsp *root = (struct rdsp*)kvinf.acpi.rdsp;
    printq("Signature: ");
    for(uint16 i = 0; i < 8; ++i){
        printq("%c", root->signature[i]);
    }
    printq("\nCheck 0x%hx\nOEM: ", root->check);
    for(uint16 i = 0; i < 6; ++i){
        printq("%c", root->OEM[i]);
    }
    printq("\nRevision: 0x%lx\n", (uint32)root->revision);
    printq("Address: 0x%lx\n", root->address);
    mapRegion(root->address, root->address+(PAGE_SZ*2), false, false, true);
    struct rsdt *r = (struct rsdt*)root->address;
    if(!checksig(4, r->signature, "RSDT")){printq("Invalid RSDT\n"); return;}
    uint32 tables = (r->len-sizeof(struct rsdt))/4;
    uint32 *ts = (uint32*)(root->address+sizeof(struct rsdt));
    struct rsdt *tbl;
    uint32 c = 0;
    for(uint32 i = 0; i < tables; ++i){
        tbl = (struct rsdt*)ts[i];
        c = 0;
        for(uint32 b = 0; b < 4; ++b){c+=(uint8)tbl->signature[b];}
        switch(c){
            case 0x11A: //FACP
            readFACP(ts[i]);
            break;
            case 0x11D: //APIC
            readAPIC(ts[i]);
            break;
            case 0x131: //HPET
            readHPET(ts[i]);
            break;
            default:
            printq("Unkown signature ");
            printsig(&r->signature, 4);
        }
    }
    printq("========================\n");
}