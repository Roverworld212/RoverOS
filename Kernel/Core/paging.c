#include "kheaders.h"
#include "libcom.h"
#include "setup.h"
#include "paging.h"
#include "kmalloc.h"

uint32 tablesPhys[1024];

pageDir *kdir = NULL;

void pageFault(regs r){
  asm volatile("cli");
  printq("%ePAGE FAULT\n", __F__);
  uint32 fault;
  asm volatile("mov %%cr2, %0" : "=r" (fault));
  printq("Fault at %x\n", fault);
  bool pres = (r.err_code&0x1);
  bool write = (r.err_code&0x2);
  bool user = (r.err_code&0x4);
  bool re = (r.err_code&0x8);
  bool decode = (r.err_code&0x16);
  printq("Present: %b\n", pres);
  printq("Write: %b\n", write);
  printq("User: %b\n", user);
  printq("RE: %b\n", re);
  printq("Decode: %b\n", decode);
  asm volatile("hlt");
  if(!user){kernelPanic("Kernel Faulted!");}
  kernelPanic("Page Fault");
}

void setCR3(uint32 a){
    asm volatile("movl %%eax, %%cr3" :: "a" (a));
}

uint32 getCR0(){
    uint32 c;
    asm volatile("movl %%cr0, %%eax" : "=a" (c));
}

void setCR0(uint32 v){
    asm volatile("movl %%eax, %%cr0" :: "a" (v));
}

void invlpg(uint32 a){
    asm volatile("invlpg (%0)" :: "r"(a) : "memory");
}

void mapPage(uint32 phys, uint32 virt, bool m, bool w, bool u){
  uint32 pgid = (virt>>12);
  uint32 tbl = (pgid>>10);
  uint32 pg = (pgid%1024);
  setFrame(virt);
  if(m && tablesPhys[tbl] == NULL){
    uint32 ad = kmalloc(sizeof(pageTable), true);
    tablesPhys[tbl] = ad;
    kdir->tables[tbl].present = true;
    kdir->tables[tbl].size = 1;
    kdir->tables[tbl].tableAddr = (ad>>12);
    memset((uint32*)ad, NULL, sizeof(pageTable));
  }
  if(tablesPhys[tbl] == NULL){return;}
  pageTable *table = (pageTable*)tablesPhys[tbl];
  table->pages[pg].present = true;
  table->pages[pg].write = w;
  table->pages[pg].user = u;
  table->pages[pg].frame = (phys>>12);
  invlpg(virt);
}

void mapTable(uint32 phys, uint32 virt, bool m, bool w, bool u){
  uint32 tbl = GET_TBL(virt);
  if(m && tablesPhys[tbl] == NULL){
    uint32 ad = kmalloc(sizeof(pageTable), true);
    tablesPhys[tbl] = ad;
    kdir->tables[tbl].present = true;
    kdir->tables[tbl].size = 1;
    kdir->tables[tbl].tableAddr = (ad>>12);
    memset((uint32*)ad, NULL, sizeof(pageTable));
  }
  if(tablesPhys[tbl] == NULL){return;}
  for(uint32 i = 0; i < 1024; ++i){
    mapPage(phys, virt, m, w, u);
    phys += PAGE_SZ;
    virt += PAGE_SZ;
  }
}

uint32 ppb(uint32 mem){
    return (mem/PAGE_SZ);
}

void ldPaging(){
    printq("%I%gEnabling Paging (Thanks u/Mai_Lapyst &  u/devjustinian)\n", __F__);
    addHandler(14, pageFault);
    kdir = (pageDir*)kmalloc(sizeof(pageDir), true);
    setupFrames(0x0fffc0000);
    memset(kdir, NULL, sizeof(pageDir));
    printq("%I%gKdir at: %x\n", __F__, (uint32)kdir);
    printq("%I%gLoaded at %x\n", __F__, (uint32)&load);
    printq("%I%gEnd: %x\n", __F__, (uint32)&end);
    printq("%I%gMapping with offset: %x\n", __F__, (uint32)&hhalf);
    for(uint32 i = (uint32)&load; i < (uint32)&end; i += PAGE_SZ){
        mapTable(i, i, true, false, false);
    }
    printq("%I%gMapped kernel\n", __F__);
    setCR3(kdir);
    setCR0(getCR0() | 0x80000000);
    asm volatile("sti");
}

//VESA

void mapBuffer(uint32 s, uint32 virt){
  uint32 len = (((kvinf.kui.xsz+kvinf.kui.ysz)*(kvinf.kui.bpp/8))+s);
  for(uint32 i = s; i <= len; i+=PAGE_SZ){
    mapTable(i, i, true, true, false);
  }
}