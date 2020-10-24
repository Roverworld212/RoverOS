#include "kheaders.h"
#include "kmalloc.h"
#include "paging.h"

void kmalloc_err(char *r, char *fnc, uint32 a, bool al){
    printq("%EKMALLOC ERROR\n", __F__);
    printq("Caller: "); printq(fnc);
    printq("\nReason: "); printq(r); printq("\n");
    if(al){printq("Align\n");}
    printq("Msz: %x\n", a);
}

uint32 curAddr = (uint32)&end;

uint32 kmalloc_int(uint32 amt, bool a, char *fnc){
    if(amt == NULL){return NULL;}
    uint32 ptr = curAddr;
    if(a && !IS_ALIGNED(ptr)){
        ptr = ALIGN(ptr);
    }
    curAddr += (ptr-curAddr);
    return (uint32)ptr;
}