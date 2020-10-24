#ifndef KSETUP_H
#define KSETUP_H

#include "types.h"

typedef struct stackS{
  
}stack;

struct gdtS{
  uint16 limLow;
  uint16 baseLow;
  uint8 baseMid;
  uint8 acc;
  uint8 gran;
  uint8 baseHigh;
}__attribute__((packed));

struct gdtpS{
  uint16 limit;
  uint32 base;
}__attribute__((packed));

typedef struct gdtS gdt;
typedef struct gdtpS gdt_ptr;

struct idteS{
  uint16 baseLow;
  uint16 s;
  uint8 a0;
  uint8 flags;
  uint16 baseHigh;
}__attribute__((packed));

struct idtpS{
  uint16 limit;
  uint32 base;
}__attribute__((packed));

typedef struct idteS idt;
typedef struct idtpS idt_ptr;

typedef struct registers{
  uint32 ds;
  uint32 edi, esi, ebp, unused_esp, ebx, edx, ecx, eax;
  uint32 int_no, err_code;
  uint32 eip, cs, eflags, esp, ss;
}regs;

extern void setupGDT();
extern void setupIDT();
extern void setupINT(uint32 freq);

extern void isrHandler(regs r);
extern void irqHandler(regs r);
typedef void (*irh)(regs);
extern void addHandler(uint8 n, irh h);

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif