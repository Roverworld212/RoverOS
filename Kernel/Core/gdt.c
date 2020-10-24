#include "kheaders.h"
#include "libcom.h"
#include "setup.h"

gdt gdte[5];
gdt_ptr gptr;
extern void gdtf(uint32);

void writeGate(char n, uint32 b, uint32 l, uint8 a, uint8 g){
    gdte[n].baseLow = (b&0xFFFF);
    gdte[n].baseMid = (b>>16)&0xFF;
    gdte[n].baseHigh = (b>>24)&0xFF;
    gdte[n].limLow = (l&0xFFFF);
    gdte[n].gran = (l>>16)&0x0F;
    gdte[n].gran = g&0xF0;
    gdte[n].acc = a;
}

void setupGDT(){
    printq("%I%gSetting up the GDT\n", __F__);
    gptr.limit = ((sizeof(gdt)*5)-1);
    printq("%I%gLimit: %x\n", __F__, ((sizeof(gdt)*5)-1));
    gptr.base = &gdte;
    printq("%I%gBase: %x\n", __F__, (uint32)&gdte);
    printq("%I%gSetting Gates\n", __F__);
    writeGate(0, 0, 0, 0, 0);
    printq("%I%gSet NULL segment\n", __F__);
    writeGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    printq("%I%gSet Code segment\n", __F__);
    writeGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    printq("%I%gSet Data segment\n", __F__);
    writeGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    printq("%I%gSet User segment\n", __F__);
    gdtf(&gptr);
    printq("%I%gLoaded GDT\n", __F__);
}