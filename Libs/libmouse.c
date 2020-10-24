#include "kheaders.h"
#include "libvalloc.h"
#include "libmouse.h"
#include "setup.h"

void mouseInt(regs r){
    uint8 d = inb(0x60);
}

uint8 mRead(){
    return inb(0x60);
}

void mWrite(uint8 d){
    mWait(1);
    outb(0x64, 0xD4);
    mWait(1);
    outb(0x60, d);
}

void mWait(uint32 i){
    uint32 d = 100000*i;
    while(--d){
        (void)inb(0x64);
    }
    return;
}

bool hasMData(){
    uint8 d = inb(0x64);
    printq("Got data: %x\n", d);
    if(d == 1){
        return true;
    }
    return false;
}

void initMouse(){
    printq("======Initilizing the mouse======\n");
    printq("%gLoading Mouse\n");
    addHandler(0x2C, &mouseInt);
    uint8 s;
    mWrite(0xFF);
    mWait(1);
    printq("%gSent reset command\n");
    if(mRead() != 0xFA){printq("%e%gMouse failed to acknowledge command\n", __F__); return;}
    printq("%gMouse acknowledged command\n");
    mWrite(0xF2);
    mWait(1);
    uint32 mid = mRead();
    printq("%gGot MouseID: %x\n", mid);
    printq("%gSetting up interrupts\n");
    mWait(1);
    outb(0x64, 0x20);
    mWait(1);
    s = (inb(0x60)|2);
    mWait(1);
    outb(0x64, 0x60);
    mWait(1);
    outb(0x60, s);
    printq("%gUsing mouse default settings\n");
    mWait(1);
    mWrite(0xF6);
    mWait(1);
    mWrite(0xF4);
    mWait(1);
    printq("%gSetup mouse\n");
    printq("=================================\n");
}