#include "libstd.h"
#include "paging.h"
#include "kvinf.h"
#include "idt.h"
#include "multiboot.h"
#include "libcom.h"
#include "libksystem.h"

void pageFault(regs r){
    cli();
    printq("Page fault\n");
    uint32 ec = getErrCode();
    dumpRegisters(r);
    uint64 cr2;
    asm volatile("movq %%cr2, %0" : "=r"(cr2));
    printq("Addr: 0x%X\n", cr2);
    printq("|present|rw|user|reserved-write|decode|\n");
    printq("Flags: ");
    if(ec&0x1){printq("p");}else{printq("-");}
    if(ec&0x2){printq("w");}else{printq("r");}
    if(ec&0x4){printq("u");}else{printq("s");}
    if(ec&0x8){printq("z");}else{printq("-");}
    if(ec&0x10){printq("d");}else{printq("-");}
    printq("\n");
    printq("rip: 0x%X\n", r.rip);
    printq("pml4e[0x%X]->", GET_PML(cr2));
    printq("pdpe[0x%X]->", GET_PDPE(cr2));
    printq("pde[0x%X]->", GET_PDE(cr2));
    printq("pt[0x%X]->", GET_PT(cr2));
    printq("0x%X\n", GET_PG_BASE(cr2));
    kernelPanic();
}

struct addressSpaceS kspace;
extern uint64 linker_end;

void mapPage(struct addressSpaceS *space, uint64 phys, uint64 virt, bool write, bool user, bool nx){
    //Set flags
    uint64 set = PRESENT;
    if(write){set |= RW;}
    if(user){set |= USR;}
    //if(nx){set |= (uint64)NX;}
    //PML4E
    space->pml[GET_PML(virt)] = set|((uint64)GET_PDPE_BASE((uint64)space->pdpe)<<12);
    //PDPE
    space->pdpe[GET_PDPE(virt)] = set|(uint64)GET_PDE_BASE((uint64)&space->pde)<<12;
    //PDE
    space->pde[GET_PDE(virt)] = set|(uint64)GET_PT_BASE((uint64)&space->pt)<<12;
    //PT
    space->pt[GET_PT(virt)] = set|(uint64)GET_PG_BASE(phys)<<12;
}

void setCR3(uint64 a){
    asm volatile("movq %%rax, %%cr3;" ::"a"(a));
    return;
}

void ld_paging(){
    printq("%qLoading new pml4e\n", QINFO);
    addInterruptHandler(pageFault, 14);
    printq("%qAdded page fault handler\n", QINFO);
    printq("%qEnd 0x%llx\n", QINFO, (uint64)&linker_end);
    for(uint64 i = 0; i <= 0x200000; i+=PAGE_SZ){
        mapPage(&kspace, i, i, true, false, true);
    }
    printq("%qMapped kernel\n", QINFO);
    setCR3((uint64)&kspace.pml);
    printq("%qSet CR3\n", QINFO);
    return;
}

extern void mapUI();

void mapUI(){
    uint64 pages = ((kvinf.video.width*kvinf.video.height)*(kvinf.video.bpp/8))/PAGE_SZ;
    uint64 p = kvinf.video.physical;
    for(uint64 i = 0; i <= pages; ++i){
        mapPage(&kspace, p+(i*PAGE_SZ), p+(i*PAGE_SZ), true, false, true);
    }
    return;
}