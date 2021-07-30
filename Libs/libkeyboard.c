#include "libstd.h"
#include "libio.h"
#include "idt.h"
#include "libkeyboard.h"
#include "libhook.h"

extern void kbInt(regs r);

void kbInt(regs r){
    uint8 kc = inb(0x60);
    uint16 flags = 0x0;
    if(kc == 0x0){
        printq("%qScancode error\n", QERROR);
        return;
    }
    if((kc >= 0x02 && kc <= 0x0D) || (kc >= 0x10 && kc <= 0x1B) || (kc >= 0x1E && kc <= 0x28) || (kc >= 0x2C && kc <= 0x35)){
        flags|=KBFLAG_CHAR;
    }
    if(kc&0x80){
        flags|=KBFLAG_RELEASE;
    }
    if(kc==0x2A||kc==0x36||kc==0x3A){
        flags|=KBFLAG_TOGGLE;
    }
    runHook(KEYBOARD_HOOK,flags,(uint64)kc);
}