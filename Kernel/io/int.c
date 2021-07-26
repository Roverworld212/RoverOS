#include "libksystem.h"
#include "registers.h"
#include "libstd.h"
#include "kvinf.h"
#include "libio.h"
#include "idt.h"

void tickInterrupt(){
    ++kvinf.kernel.tick;
    if(kvinf.kernel.tick%40 == 0){
        ++kvinf.kernel.second;
        kvinf.kernel.tick = 0;
    }
    if(kvinf.kernel.second%60 == 0){
        ++kvinf.kernel.minute;
        kvinf.kernel.second = 0;
    }
    if(kvinf.kernel.minute%60 == 0){
        ++kvinf.kernel.hour;
        kvinf.kernel.minute = 0;
    }
    return;
}

void initPit(uint32 freq){
    printq("%qLoading PIT\n", QINFO);
    addInterruptHandler(tickInterrupt, IRQ0);
    printq("%qAdded PIT handler\n", QINFO);
    uint32 div = 1193180/freq;
    outb(0x43, 0x36);
    uint8 l = (uint8)(div&0xFF);
    uint8 h = (uint8)((div>>8)&0xFF);
    outb(0x40, l);
    outb(0x40, h);
    printq("%qFreq: 0x%lx - Div: 0x%lx - L: 0x%hx - H: 0x%hx\n", QINFO, freq, div, l, h);
    printq("%qSetup pit\n", QINFO);
}

irh handlers[256];
uint32 errcode;

uint32 getErrCode(){
    return errcode;
}

void isrHandler(regs r, uint32 ec){
    if(handlers[ec] != NULL){
        errcode = ec;
        irh handler = handlers[ec];
        handler(r); 
    }
    return;
}

void irqHandler(int interrupt, regs r){
    if(interrupt >= 40){
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
    if(handlers[interrupt] != NULL){
        irh handler = handlers[interrupt];
        handler(r);
        return;
    }
    printq("%qUnhandled irq 0x%x\n", QWARNING, interrupt);
    return;
}

void addInterruptHandler(irh func, int n){
    handlers[n] = func;
}