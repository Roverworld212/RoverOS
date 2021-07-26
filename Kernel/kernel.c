#include "libksystem.h"
#include "multiboot.h"
#include "libstd.h"
#include "types.h"
#include "kvinf.h"
#include "cpuid.h"
#include "libui.h"
#include "acpi.h"
#include "idt.h"
#include "pmalloc.h"
#include "basefont.h"

extern void kmain(uint64 *kstruct);
extern void read_mstruct(uint64 *mp);
extern void mapUI();
extern void initKshell();
extern void readCpuid();
extern void kbInt(regs r);
extern void readRamdisk();
struct gui kui;
struct text txt;
struct task ktask;

void kmain(uint64 *kstruct){
    printq("%qEntered kmain\n", QINFO);
    kvinf.kernel.timelog = false;
    read_mstruct(kstruct);
    //Setup UI so printf will work
    kui.buffer = (uint32)kvinf.video.physical;
    kui.Xpos = 0;
    kui.Ypos = 0;
    kui.Xres = kvinf.video.width;
    kui.Yres = kvinf.video.height;
    //Txt
    txt.flags = TXT_FONT;
    txt.font = &baseFont;
    txt.lcpX = 0;
    txt.lcpY = 0;
    txt.nlPadding = 10;
    txt.padding = 8;
    txt.Xpos = 0;
    txt.Ypos = 0;
    txt.fcolor = VIOLET;
    txt.bcolor = BLACK;
    txt.Xres = kui.Xres-txt.Xpos;
    txt.Yres = kui.Yres-txt.Ypos;
    txt.Xscale = 0;
    txt.Yscale = 0;
    ld_idt();
    ld_handlers();
    ld_paging();
    mapUI();
    createTask(&kui, &txt, &ktask, 30, NULL);
    swapTask(&ktask);
    printf("UI Loaded\n");
    initPit(40);
    printf("Loaded PIT\n");
    addInterruptHandler(kbInt, IRQ1);
    readRamdisk();
    printf("Read ramdisk\n");
    readACPI();
    printf("Read ACPI\n");
    sti();
    printf("Enabled interrupts\n");
    //printq("%qEnabled interrupts\n", QINFO);
    //kvinf.kernel.timelog = true;
    //printq("%qEnabled time logging\n", QINFO);
    readCpuid();
    pmallocInit(0x100000);
    printf("Setup pmalloc\n");
    printq("%qAlloc returned 0x%llx\n", QINFO, (uint64)pmalloc(1, 0));
    //initKshell();
    printf("Idling\n");
    printq("%qKernel idling\n", QINFO);
    for(;;){hlt();}
}