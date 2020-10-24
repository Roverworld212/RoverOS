#include "kheaders.h"
#include "libcom.h"
#include "setup.h"

extern void idtf(uint32);
idt idte[256];
idt_ptr idtp;

void writeIdt(uint8 n, uint32 b, uint16 s, uint8 f){
    idte[n].baseLow = b&0xFFFF;
    idte[n].baseHigh = (b>>16)&0xFFFF;
    idte[n].s = s;
    idte[n].a0 = 0;
    idte[n].flags = f;
}

void setupIDT(){
    printq("%I%gSetting up IDT\n", __F__);
    idtp.limit = sizeof(idt)*256-1;
    printq("%I%gLimit: %x\n", __F__, (uint32)(sizeof(idt)*256-1));
    idtp.base = (uint32)&idte;
    printq("%I%gBase: %x\n", __F__, (uint32)&idte);
    printq("%I%gClearing entries\n", __F__);
    memset(&idte, NULL, sizeof(idt)*256);
    printq("%I%gRemapping IDT\n", __F__);
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
    printq("%I%gSetting Gates\n", __F__);
    writeIdt(0, (uint32)isr0, 0x08, 0x8E);
    writeIdt(1, (uint32)isr1, 0x08, 0x8E);
    writeIdt(2, (uint32)isr2, 0x08, 0x8E);
    writeIdt(3, (uint32)isr3, 0x08, 0x8E);
    writeIdt(4, (uint32)isr4, 0x08, 0x8E);
    writeIdt(5, (uint32)isr5, 0x08, 0x8E);
    writeIdt(6, (uint32)isr6, 0x08, 0x8E);
    writeIdt(7, (uint32)isr7, 0x08, 0x8E);
    writeIdt(8, (uint32)isr8, 0x08, 0x8E);
    writeIdt(9, (uint32)isr9, 0x08, 0x8E);
    writeIdt(10, (uint32)isr10, 0x08, 0x8E);
    writeIdt(11, (uint32)isr11, 0x08, 0x8E);
    writeIdt(12, (uint32)isr12, 0x08, 0x8E);
    writeIdt(13, (uint32)isr13, 0x08, 0x8E);
    writeIdt(14, (uint32)isr14, 0x08, 0x8E);
    writeIdt(15, (uint32)isr15, 0x08, 0x8E);
    writeIdt(16, (uint32)isr16, 0x08, 0x8E);
    writeIdt(17, (uint32)isr17, 0x08, 0x8E);
    writeIdt(18, (uint32)isr18, 0x08, 0x8E);
    writeIdt(19, (uint32)isr19, 0x08, 0x8E);
    writeIdt(20, (uint32)isr20, 0x08, 0x8E);
    writeIdt(21, (uint32)isr21, 0x08, 0x8E);
    writeIdt(22, (uint32)isr22, 0x08, 0x8E);
    writeIdt(23, (uint32)isr23, 0x08, 0x8E);
    writeIdt(24, (uint32)isr24, 0x08, 0x8E);
    writeIdt(25, (uint32)isr25, 0x08, 0x8E);
    writeIdt(26, (uint32)isr26, 0x08, 0x8E);
    writeIdt(27, (uint32)isr27, 0x08, 0x8E);
    writeIdt(28, (uint32)isr28, 0x08, 0x8E);
    writeIdt(29, (uint32)isr29, 0x08, 0x8E);
    writeIdt(30, (uint32)isr30, 0x08, 0x8E);
    writeIdt(31, (uint32)isr31, 0x08, 0x8E);
    writeIdt(32, (uint32)irq0, 0x08, 0x8E);
    writeIdt(33, (uint32)irq1, 0x08, 0x8E);
    writeIdt(34, (uint32)irq2, 0x08, 0x8E);
    writeIdt(35, (uint32)irq3, 0x08, 0x8E);
    writeIdt(36, (uint32)irq4, 0x08, 0x8E);
    writeIdt(37, (uint32)irq5, 0x08, 0x8E);
    writeIdt(38, (uint32)irq6, 0x08, 0x8E);
    writeIdt(39, (uint32)irq7, 0x08, 0x8E);
    writeIdt(40, (uint32)irq8, 0x08, 0x8E);
    writeIdt(41, (uint32)irq9, 0x08, 0x8E);
    writeIdt(42, (uint32)irq10, 0x08, 0x8E);
    writeIdt(43, (uint32)irq11, 0x08, 0x8E);
    writeIdt(44, (uint32)irq12, 0x08, 0x8E);
    writeIdt(45, (uint32)irq13, 0x08, 0x8E);
    writeIdt(46, (uint32)irq14, 0x08, 0x8E);
    writeIdt(47, (uint32)irq15, 0x08, 0x8E);
    printq("%I%gSet Gates\n", __F__);
    idtf(&idtp);
    printq("%I%gLoaded IDT\n", __F__);
}