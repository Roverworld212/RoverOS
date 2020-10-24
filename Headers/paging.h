#ifndef PAGING_H
#define PAGING_H

#define IS_ALIGNED(addr) (((uint32)addr)&0xFF == 0) //Thanks u/devjustinian!
#define ALIGN(addr) ((((uint32)addr)&0xFFFFF000)+0x1000)
#define GET_TBL(addr) ((addr>>12)&0x3FF)
#define GET_PG(vaddr) (((vaddr>>12)>>10)%1024)
#define PAGE_SZ 0x1000
#define FSET_SZ (PAGE_SZ*32)

#define FSET32 0x80000000
#define FDIV32 0x20000
#define FFULL 0xFFFFFFFF
#define FHALF 0xFFFF

#define IS_SET(fn, f) ((uint32*)((uint32)f&(FSET32>>fn)))
#define SET(fn, f) ((uint32*)((FSET32>>fn)|(uint32)f))
#define UNSET(fn, f) ((uint32*)(((uint32)f)^((uint32)(FSET32>>fn))))

#define GET_IDX(ad) ((ad/PAGE_SZ)/32)
#define GET_FRAME(ad) ((ad-(((ad/PAGE_SZ)/32)*FDIV32))/PAGE_SZ)
#define GET_GLFRAME(ad) (ad/PAGE_SZ)

typedef struct pageS{
  bool present : 1; //Is in mem
  bool write : 1; //r - 0 w - 1
  bool user : 1; //su - 0 user - 1
  bool writeT : 1; //Write through
  bool dcache : 1; //Disable caching
  bool acc : 1; //Was accessed
  bool dirty : 1; //Has been modified
  bool a0 : 1; //Must stay 0
  bool global : 1; //Doesn't get changed on cr3 reset
  uint32 unused : 3; //Free for my use
  uint32 frame : 20; //higher 20 bits of pointer to physical aligned memory
}page;

typedef struct pageTableS{
  page pages[1024];
}pageTable;

typedef struct pageEntryS{
  bool present : 1;
  bool write : 1;
  bool user : 1;
  bool writeT : 1;
  bool dcache : 1;
  bool acc : 1;
  bool a0 : 1;
  bool size : 1; //0 - 4MB 1 - 4KB
  bool global : 1; //No use in this struct
  uint32 unused : 3;
  uint32 tableAddr : 20; //Same as page frames (but for pageTables)
}pageEntry;

typedef struct pageDir{
  pageEntry tables[1024];
}pageDir;

extern uint32 load; //Load address
extern uint32 end; //EOK
extern uint32 hhalf; //Higher-Half
extern uint32 curAddr;

#endif