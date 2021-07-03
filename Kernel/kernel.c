#include "types.h"
#include "multiboot.h"
#include "libstd.h"
#include "libksystem.h"
#include "idt.h"
#include "kvinf.h"
#include "cpuid.h"
#include "libui.h"

extern void kmain(uint64 *kstruct);
extern void read_mstruct(uint64 *mp);
extern void mapUI();

void kmain(uint64 *kstruct){
    printq("%qEntered kmain\n", QINFO);
    kvinf.kernel.timelog = false;
    read_mstruct(kstruct);
    ld_idt();
    ld_handlers();
    ld_paging();
    mapUI();
    struct guiObject obj;
    obj.buffer = (uint32*)kvinf.video.physical;
    obj.Xpos = 0;
    obj.Ypos = 0;
    obj.Xres = kvinf.video.width;
    obj.Yres = kvinf.video.height;
    fill(&obj, PURPLE);
    plotPixel(&obj, 0, 0, WHITE);
    for(;;){hlt();}
}