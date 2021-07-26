#include "libstd.h"
#include "libksystem.h"
#include "idt.h"
#include "registers.h"

extern void fidt(void*);
extern void fake_handler(void);
extern idtp idtpnt;

idt idte[256] __attribute__((aligned(4096)));;
idtp idtpnt __attribute__((aligned(4096)));;

#define INORMAL 0x4C
#define INOTHER 0x61

void writeIdt(uint32 n, uint64 isr){
    idte[n].low = (uint16)(isr&0xFFFF);
    idte[n].mid = (uint16)((isr>>16)&0xFFFF);
    idte[n].high = (uint32)(isr>>32);
    idte[n].ist = 0x0;
    idte[n].flags = 0x8E;
    idte[n].segment = 0x8;
    idte[n].zero = 0;
}

#define FCHECK64 0x8000000000000000

void ld_idt(){
    cli();
    printq("%qLoading IDT\n", QINFO);
    printq("%qIDT-Size: 0x%X\n%qIDTP-Size: 0x%X\n", QINFO, sizeof(idt), QINFO, sizeof(idtp));
    idtpnt.limit = sizeof(idt)*256-1;
    idtpnt.base = (uint64)&idte;
    memset((uint8*)&idte, NULL, sizeof(idt)*256);
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
    printq("%qFilling IDT\n", QINFO);
    writeIdt(0, (uint64)isr0);
    writeIdt(1, (uint64)isr1);
    writeIdt(2, (uint64)isr2);
    writeIdt(3, (uint64)isr3);
    writeIdt(4, (uint64)isr4);
    writeIdt(5, (uint64)isr5);
    writeIdt(6, (uint64)isr6);
    writeIdt(7, (uint64)isr7);
    writeIdt(8, (uint64)isr8);
    writeIdt(9, (uint64)isr9);
    writeIdt(10, (uint64)isr10);
    writeIdt(11, (uint64)isr11);
    writeIdt(12, (uint64)isr12);
    writeIdt(13, (uint64)isr13);
    writeIdt(14, (uint64)isr14);
    writeIdt(15, (uint64)isr15);
    writeIdt(16, (uint64)isr16);
    writeIdt(17, (uint64)isr17);
    writeIdt(18, (uint64)isr18);
    writeIdt(19, (uint64)isr19);
    writeIdt(20, (uint64)isr20);
    writeIdt(21, (uint64)isr21);
    writeIdt(22, (uint64)isr22);
    writeIdt(23, (uint64)isr23);
    writeIdt(24, (uint64)isr24);
    writeIdt(25, (uint64)isr25);
    writeIdt(26, (uint64)isr26);
    writeIdt(27, (uint64)isr27);
    writeIdt(28, (uint64)isr28);
    writeIdt(29, (uint64)isr29);
    writeIdt(30, (uint64)isr30);
    writeIdt(31, (uint64)isr31);
    writeIdt(32, (uint64)irq0);
    writeIdt(33, (uint64)irq1);
    writeIdt(34, (uint64)irq2);
    writeIdt(35, (uint64)irq3);
    writeIdt(36, (uint64)irq4);
    writeIdt(37, (uint64)irq5);
    writeIdt(38, (uint64)irq6);
    writeIdt(39, (uint64)irq7);
    writeIdt(40, (uint64)irq8);
    writeIdt(41, (uint64)irq9);
    writeIdt(42, (uint64)irq10);
    writeIdt(43, (uint64)irq11);
    writeIdt(44, (uint64)irq12);
    writeIdt(45, (uint64)irq13);
    writeIdt(46, (uint64)irq14);
    writeIdt(47, (uint64)irq15);
    printq("%qFilled IDT\n", QINFO);
    fidt((void*)&idtpnt);
    printq("%qLoaded IDT\n", QINFO);
}