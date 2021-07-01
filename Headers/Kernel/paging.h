#ifndef PAGING_H
#define PAGING_H

#include "types.h"

#define PAGE_SZ 0x1000
#define FSET_SZ (PAGE_SZ*32)
#define FSET32 0x80000000
#define FDIV32 0x20000
#define FFULL 0xFFFFFFFF
#define FHALF 0xFFFF

#define PAGE_MASK 0x000ffffffffff000 //Thanks u/Mai_Lapyst
#define PG_MASK 0x0000000000000FFF
#define PT_MASK 0x00000000000FF000
#define PDE_MASK 0x000000000FF00000
#define PDPE_MASK 0x0000000FF0000000
#define PML_MASK 0x00000FF000000000
#define SIGN_MASK 0xFFFFF00000000000

#define GET_PG_BASE(p) ((p&PAGE_MASK)>>12)
#define GET_PT_BASE(d) ((d&PAGE_MASK)>>12)
#define GET_PDE_BASE(d) ((d&PAGE_MASK)>>12)
#define GET_PDPE_BASE(d) ((d&PAGE_MASK)>>12)

#define IS_ALIGNED(addr) (((uint64)addr)) //Thanks u/devjustinian
#define ALIGN(addr) ((((uint64)addr)&0xFFFFF000)+0x1000)

#define SET_PAE(x) (x|(FSET32>>5))
#define SET_PE(x) (x|FSET32)

//Paging macros

#define GET_PT(v) (v>>12)&0x1FF
#define GET_PDE(v) (v>>21)&0x1FF
#define GET_PDPE(v) (v>>30)&0x1FF
#define GET_PML(v) (v>>39)&0x1FF

struct pml4e_entry{
    bool present : 1; //Present if set
    bool rw : 1; //Write if set
    bool supervisor : 1; //User if set
    bool pwt : 1; //Uses write-through cache policy if set
    bool pcd : 1; //Cannot be cached if set
    bool accessed : 1; //Accessed if set
    bool IGN : 1; //Reserved
    bool MBZ : 2; //Reserved
    uint8 avalible : 3; //Avalible
    uint64 baseAddress : 40; //Address to pdpe
    uint64 free : 11; //Free for use
    bool nx : 1; //Not executable
}__attribute__((packed)); //pml4e

struct pdpe_entry{
    bool present : 1; //Present if set
    bool rw : 1; //Write if set
    bool supervisor : 1; //User if set
    bool pwt : 1; //Uses write-through cache policy if set
    bool pcd : 1; //Cannot be cached if set
    bool accessed : 1; //Accessed if set
    bool IGN : 1; //Reserved
    bool Z : 1; //Same as PDE
    bool IGN2 : 1; //Reserved
    uint8 avalible : 3; //Avalible
    uint64 baseAddress : 40; //Address to pde
    uint64 free : 11; //Free for use
    bool nx : 1; //Not executable
}__attribute__((packed)); //Page directory pointer

struct pde_entry{
    bool present : 1; //Present if set
    bool rw : 1; //Write if set
    bool supervisor : 1; //User if set
    bool pwt : 1; //Uses write-through cache policy if set
    bool pcd : 1; //Cannot be cached if set
    bool accessed : 1; //Accessed if set
    bool IGN : 1; //Has been written to if set
    bool F : 1; //Indicates 4K pages if cleared
    bool IGN2 : 1; //Ignored
    uint8 avalible : 3; //Avalible
    uint64 baseAddress : 40; //Address to page tables
    uint32 free : 11; //Free for use
    bool nx : 1; //Not executable
}__attribute__((packed)); //Page directory

struct pt_entry{
    bool present : 1; //Present if set
    bool rw : 1; //Write if set
    bool supervisor : 1; //User if set
    bool pwt : 1; //Uses write-through cache policy if set
    bool pcd : 1; //Cannot be cached if set
    bool accessed : 1; //Accessed if set
    bool D : 1; //Has been written to if set
    bool PAT : 1; //Page attribution table
    bool G : 1; //Ignored
    uint32 avalible : 3; //Avalible
    uint64 baseAddress : 40; //Address to 4k pages
    uint32 free : 11; //Free for use
    uint32 PKE : 4; //Depends on CR4.PKE
    bool nx : 1; //Not executable
}__attribute__((packed)); //Page table

struct pml4eS{
    struct pml4e_entry pml[512];
}__attribute__((packed));
 
struct pdpeS{
    struct pdpe_entry pdpe[512];
}__attribute__((packed));
 
struct pdeS{
    struct pde_entry pde[512];
}__attribute__((packed));
 
struct ptS{
    struct pt_entry pt[512];
}__attribute__((packed));

struct address_space{
    struct pml4eS *pml;
    struct pdpeS *pdpe;
    struct pdeS *pde;
    struct ptS *pt;
}__attribute__((packed));

extern struct ptS pageTable;
extern struct pdeS pageDir;
extern struct pdpeS pageDirPointer;
extern struct pml4eS pageMap;

#endif