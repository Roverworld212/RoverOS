#include "kheaders.h"
#include "kutil.h"
#include "libui.h"
#include "rcom.h"
#include "libbin.h"
#include "tss.h"

void kernelPanic_int(char *caller, char *file, uint32 line, char *reason){
    asm volatile("cli");
    //cls();
    //for(uint32 i = kvargs.bufferVirt; i <= ((PAGE_SZ*600)+kvargs.bufferPhys); i+=PAGE_SZ){//Make sure the buffer is mapped so we don't page fault
    //    mapPage(i, i, true, true, false);
    //} 
    /*kvui.x = kvargs.xres;
    kvui.y = 200;
    kvui.sz = kvui.x*kvui.y;
    kvui.csx = kvui.xpadding;
    kvui.csy = kvui.ypadding;
    //Doesn't matter how it's mapped, we already had a panic and the kernel couldn't recover
    kvui.textPadding = 15;
    kvui.nlPadding = 20;*/
    printq("==================================Kernel Panic==================================\n");
    printq("Caller: "); printq(caller); //printf(caller);
    printq("\nFile: "); printq(file); //printf(file);
    printq("\nLine: %f\n", line);
    printq("Reason: "); printq(reason); //printf(reason);
    //printf("\nCheck the Out.log for more information");
    printq("\n================================================================================\n");
    for(;;){asm("hlt");}
}

//Kvargs utils

void setArg(){
    
}