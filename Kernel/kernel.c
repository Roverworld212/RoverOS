#include "types.h"
#include "multiboot.h"
#include "libstd.h"
#include "libksystem.h"

extern void kmain(uint64 *kstruct);
extern void read_mstruct(uint64 *mp);

void kmain(uint64 *kstruct){
    printq("%qEntered kmain\n", QINFO);
    read_mstruct(kstruct);
    hlt();
}