#include "kheaders.h"
#include "multiboot.h"
#include "setup.h"
#include "paging.h"
#include "rcom.h"

void main(uint32 magic, uint64 mbaddr){
    kvinf.kernelname = (char*)"Anza";
    kvinf.kio.log = true;
    kvinf.kio.debugOut = true;
    kvinf.kio.infoOut = true;
    kvinf.kio.warnOut = true;
    kvinf.kio.errorOut = true;
    kvinf.kio.panicOut = true;
    startCom(COM1);
    readStruct(mbaddr);
    setupRamdisk();
    kvinf.kui.pfui = &kvui;
    readKvargs();
    //Setup kvui
    kvui.xsz = kvinf.kui.xsz;
    kvui.ysz = kvinf.kui.ysz;
    kvui.layer = KLAYER;
    kvui.parent = (uint32*)NULL;
    kvui.CColor = PURPLE;
    kvui.backColor = BLACK;
    kvui.font = (uint8*)&StolenFont;
    kvui.buffer = kvinf.kui.buffer;
    kvui.autoTop = true;
    initMouse();
    getCPUI();
    setupGDT();
    setupIDT();
    setupInt(40);
    ldPaging();
    printFrameMap();
    setupComms();
    //Map the buffer
    mapBuffer(kvui.buffer, kvui.buffer);
    printf("Memory Sections\n");
    printf("Free: %x\nReserved: %x\nACPI: %x\nNVS: %x\nbad ram: %x\nunknown: %x\n", (uint32)kvinf.kmem.free, (uint32)kvinf.kmem.reserved, (uint32)kvinf.kmem.acpi, (uint32)kvinf.kmem.nvs, (uint32)kvinf.kmem.badram, (uint32)kvinf.kmem.unknown);
    printf("RoverOS Loaded\n");
    tmain();
    for(;;){asm("hlt");}
}