#include "libksystem.h"
#include "types.h"
#include "multiboot.h"
#include "libstd.h"
#include "kvinf.h"

void cmdline(struct multibootTag *tag){
    printq("======CMD======\n");
    struct multibootTagString *s = (struct multibootTagString*)tag;
    printq("%s\n", (uint8*)&s->str);
}

void bootloader(struct multibootTag *tag){
    printq("======BLD======\n");
    struct multibootTagString *s = (struct multibootTagString*)tag;
    printq("%s\n", (uint8*)&s->str);
}

void module(struct multibootTag *tag){
    printq("======MOD======\n");
    struct multibootTagMod *mod = (struct multibootTagMod*)tag;
    if(strcmp(&mod->cmdline, "ramdisk") == 0){
        kvinf.fs.ramdisk = (uint64*)mod->start;
        kvinf.fs.ramdiskEnd = (uint64*)mod->start+mod->end;
    }
    printq("Module: %s\n", (uint8*)&mod->cmdline);
    printq("Located at 0x%lx\n", mod->start);
    printq("End: 0x%lx\n", mod->end);
    printq("Size: 0x%lx\n", mod->end-mod->start);
}

void meminfo(struct multibootTag *tag){
    printq("======MEM======\n");
    struct multibootBasicMem *mem = (struct multibootBasicMem*)tag;
    printq("Upper: 0x%lx\nLower: 0x%lx\n", mem->memUpper, mem->memLower);
}

void bootdevice(struct multibootTag *tag){
    printq("======BDEV=====\n");
    struct multibootBootdev *dev = (struct multibootBootDev*)tag;
    printq("Bios dev: 0x%lx\n", dev->biosdev);
    printq("Slice: 0x%lx\n", dev->slice);
    printq("Part: 0x%lx\n", dev->part);
}

void mmap(struct multibootTag *tag){
    printq("======MMAP=====\n");
    struct multibootMmap *mmap = (struct multibootMmap*)tag;
    printq("Mmap: 0x%lx-0x%lx\n", mmap->entrysz, mmap->entryversion);
}

void vbe(struct multibootTag *tag){
    printq("======VBE======\n");
    struct multibootVBE *vbet = (struct multibootVBE*)tag;
    printq("Mode: 0xlx\n", (uint32)vbet->mode);
    printq("Seg: 0x%lx\n", (uint32)vbet->seg);
    printq("Off: 0x%lx\n", (uint32)vbet->off);
    printq("Len: 0x%lx\n", (uint32)vbet->len);
}

void framebuffer(struct multibootTag *tag){
    printq("=====FBUFF=====\n");
    struct multibootFramebufferReal *fb = (struct multibootFramebufferReal*)tag;
    printq("Buffer: 0x%llx\n", fb->common.phys);
    printq("Pitch: 0x%lx\n", fb->common.pitch);
    printq("Width: 0x%lx\n", fb->common.width);
    printq("Height: 0x%lx\n", fb->common.height);
    printq("Bpp: 0x%hx\n", fb->common.bpp);
    printq("Type: 0x%hx\n", fb->common.ftype);
    printq("Palette 0x%lx\n", fb->palette);
    printq("R: 0x%hx G: 0x%hx B: 0x%hx\n", fb->fbPalette.red, fb->fbPalette.green, fb->fbPalette.blue);
    printq("RedPos: 0x%hx Mask: 0x%hx\n", fb->redPos, fb->redMask);
    printq("GreenPos: 0x%hx Mask: 0x%hx\n", fb->greenPos, fb->greenMask);
    printq("BluePos: 0x%hx Mask: 0x%hx\n", fb->bluePos, fb->blueMask);
    kvinf.video.physical = fb->common.phys;
    kvinf.video.width = fb->common.width;
    kvinf.video.height = fb->common.height;
    kvinf.video.bpp = fb->common.bpp;
}

void apm(struct multibootTag *tag){
    printq("======APM======\n");
    struct multibootApm *apmt = (struct multibootApm*)tag;
    printq("Version: 0x%lx\n", (uint32)apmt->version);
}

void efi(struct multibootTag *tag, bool t){
    printq("======EFI======\n");
}

void smbios(struct multibootTag *tag){
    printq("======SMB======\n");
}

bool b = false;

void acpi(struct multibootTag *tag, bool t){
    if(!t && b){return;}
    b = t;
    printq("======ACPI=====\n");
    struct multibootAcpi *acpi = (struct multibootAcpi*)tag;
    printq("Version: ");
    if(t){printq("New\n");}else{printq("Old\n");}
    printq("RSDP: 0x%llx\n", (uint64)acpi->rsdp);
    kvinf.acpi.new = t;
    kvinf.acpi.rdsp = (uint64)&acpi->rsdp;
}

void network(struct multibootTag *tag){
    printq("======NET======\n");
    struct multibootNetwork *net = (struct multibootNetwork*)tag;
    printq("DHC: 0x%llx\n", (uint64)net->dhcpack);
}

void lba(struct multibootTag *tag){
    printq("======LBA======\n");
    struct multibootBaseLoad *lbat = (struct multibootBaseLoad*)tag;
    printq("Base: 0x%lx\n", lbat->baseAddr);
}

void readMultiboot(uint64 mbs){
    printq("===============MULTIBOOT===============\n");
    uint64 mbst = mbs+8;
    struct multibootTag *tag;
    while(true){
        tag = (struct mutlibootTag*)mbst;
        switch(tag->type){
            case MBOOT_TAG_END:
            printq("===============\n");
            printq("=======================================\n");
            return;
            break;
            case MBOOT_CMDLINE:
            cmdline(tag);
            break;
            case MBOOT_BOOTLOADER:
            bootloader(tag);
            break;
            case MBOOT_MOD:
            module(tag);
            break;
            case MBOOT_MEMINFO:
            meminfo(tag);
            break;
            case MBOOT_BOOTDEV:
            bootdevice(tag);
            break;
            case MBOOT_MMAP:
            mmap(tag);
            break;
            case MBOOT_VBE:
            vbe(tag);
            break;
            case MBOOT_FBUFFER:
            framebuffer(tag);
            break;
            case MBOOT_APM:
            apm(tag);
            break;
            case MBOOT_EFI32:
            efi(tag, 0);
            break;
            case MBOOT_EFI64:
            efi(tag, 1);
            break;
            case MBOOT_SMBIOS:
            smbios(tag);
            break;
            case MBOOT_ACPI_OLD:
            acpi(tag, 0);
            break;
            case MBOOT_ACPI_NEW:
            acpi(tag, 1);
            break;
            case MBOOT_NETWORK:
            network(tag);
            break;
            case MBOOT_LOAD_BASE_ADDR:
            lba(tag);
            break;
        }
        mbst += tag->sz+7;
        mbst &= ~7;
    }
}