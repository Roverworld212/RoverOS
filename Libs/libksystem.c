#include "libstd.h"
#include "libksystem.h"

void sti(){
    asm volatile("sti");
}

void cli(){
    asm volatile("cli");
}

void hlt(){
    asm volatile("hlt");
}

void kernelPanic_int(){
    cli();
    for(;;){hlt();}
}