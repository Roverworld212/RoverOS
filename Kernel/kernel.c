#include "types.h"
#include "multiboot.h"
#include "libstd.h"
#include "libksystem.h"
#include "idt.h"
#include "kvinf.h"
#include "cpuid.h"

extern void kmain(uint64 *kstruct);
extern void read_mstruct(uint64 *mp);
extern void mapUI();

void kmain(uint64 *kstruct){
    printq("%qEntered kmain\n", QINFO);
    read_mstruct(kstruct);
    ld_idt();
    ld_handlers();
    ld_paging();
    mapUI();
    readCpuid();
    for(;;){hlt();}
}