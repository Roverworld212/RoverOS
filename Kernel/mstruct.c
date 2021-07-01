#include "libksystem.h"
#include "types.h"
#include "multiboot.h"
#include "libstd.h"

void read_mstruct(uint64 *mp){
    printq("%qReading mstruct values\n", QINFO);
    printq("%qLocated at 0x%llx\n", QINFO, (uint64)mp);
    struct mstruct *info = (struct mstruct*)mp;
    printq("===VBE===\n");
    printq("Buffer: 0x%llx\n", info->vesa.phys);
    printq("Pitch: 0x%lx\n", info->vesa.pitch);
    printq("Width: 0x%lx\n", info->vesa.width);
    printq("Height: 0x%lx\n", info->vesa.height);
    printq("Bpp: 0x%x\n", info->vesa.bpp);
    printq("Type: 0x%x\n", info->vesa.type);
}