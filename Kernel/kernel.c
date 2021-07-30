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

extern void kmain(uint64 mbstruct);
extern void mapUI();
extern void initKshell();
extern void readCpuid();
extern void kbInt(regs r);
extern void readRamdisk();
struct gui kui;
struct text txt;
struct task ktask;
struct object curs;

void kmain(uint64 mbstruct){
    kvinf.kernel.timelog = false;
    kvinf.io.caps = false;
    kvinf.io.shift = false;
    printq("%qEntered kmain\n", QINFO);
    readMultiboot(mbstruct);
    //Setup UI so printf will work
    createGUI(&kui,kvinf.video.width,kvinf.video.height,0,0,0,NULL);
    //Txt
    createText(&txt, kui.Xres, kui.Yres, 0, 0, 0, 0, 8, 10, 0, 0, VIOLET, BLACK, TXT_FONT, &baseFont);
    //Cursor
    createObject(&curs, 30, 30, 16, 16, OBJ_BIT_DRAW, 16, &cursor);
    createTask(&kui, &txt, &ktask, 30, NULL);
    swapTask(&ktask);
    ld_idt();
    ld_handlers();
    ld_paging();
    mapUI();
    //fill(&kui, ORANGE);
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
    initKshell();
    printq("%qKernel idling\n", QINFO);
    for(;;){hlt();}
}