#include "libksystem.h"
#include "types.h"
#include "multiboot.h"
#include "libstd.h"
#include "kvinf.h"

void read_mstruct(uint64 *mp){
    printq("%qReading mstruct values\n", QINFO);
    printq("%qLocated at 0x%llx\n", QINFO, (uint64)mp);
    struct mstruct *info = (struct mstruct*)mp;
    printq("===VBE===\n");
    printq("Buffer: 0x%llx\n", info->vesa.phys);
    printq("Pitch: 0x%lx\n", info->vesa.pitch);
    printq("Width: 0x%lx\n", info->vesa.width);
    printq("Height: 0x%lx\n", info->vesa.height);
    printq("Bpp: 0x%hx\n", info->vesa.bpp);
    printq("Type: 0x%hx\n", info->vesa.type);
    printq("===LBA===\n");
    printq("Base: 0x%lx\n", info->lba.base);
    printq("===SMB===\n");
    printq("Major: 0x%hx\n", info->smbios.major);
    printq("Minor: 0x%hx\n", info->smbios.minor);
    printq("===EFI===\n");
    printq("Pointer: 0x%lx\n", info->efi32.pointer);
    printq("===APM===\n");
    printq("Version: 0x%x\n", info->apm.version);
    printq("Flags: 0x%x\n", info->apm.flags);
    printq("===NET===\n");
    printq("Dhc: 0x%hx\n", info->network.dhc);
    printq("=========\n");
    kvinf.video.physical = info->vesa.phys;
    kvinf.video.width = info->vesa.width;
    kvinf.video.height = info->vesa.height;
    kvinf.video.bpp = info->vesa.bpp;
    kvinf.video.pitch = info->vesa.pitch;
}