#include "libstd.h"
#include "idt.h"
#include "registers.h"
#include "libcom.h"
#include "libksystem.h"

//extern void view_instructions(uint64 rip);

void dumpRegisters(regs r){
    printq("===REGISTERS===\n");
    printq("r15: 0x%lX\n", r.r15);
    printq("r14: 0x%lX\n", r.r14);
    printq("r13: 0x%lX\n", r.r13);
    printq("r12: 0x%lX\n", r.r12);
    printq("r11: 0x%lX\n", r.r11);
    printq("r10: 0x%lX\n", r.r10);
    printq("r9:  0x%lX\n", r.r9);
    printq("r8:  0x%lX\n", r.r8);
    printq("rbp: 0x%lX\n", r.rbp);
    printq("rdi: 0x%lX\n", r.rdi);
    printq("rsi: 0x%lX\n", r.rsi);
    printq("rdx: 0x%lX\n", r.rdx);
    printq("rcx: 0x%lX\n", r.rcx);
    printq("rbx: 0x%lX\n", r.rbx);
    printq("rax: 0x%lX\n", r.rax);
    printq("rip: 0x%lX\n", r.rip);
    printq("cs:  0x%lX\n", r.cs);
    printq("rfl: 0x%lX\n", r.rflags);
    printq("rsp: 0x%lX\n", r.rsp);
    printq("ss:  0x%lX\n", r.ss);
    uint64 fault;
    asm volatile("mov %%cr2, %0" : "=r" (fault));
    printq("cr2: 0x%lX\n", fault);
    printq("--INSTRUCTIONS--\n");
    //read_instructions(r.rip);
    printq("===============\n");
}

void gpf(regs r){ //General Protection Fault
    printq("General Protection Fault\n");
    dumpRegisters(r);
    const uint64 c = ((uint64)1);
    uint64 error = (r.ss<<56);
    printq("External: ");
    if(error&c){printq("True\n");}else{printq("False\n");}
    printq("Table: ");
    uint16 tbl = (error&(c<<1))|(error&(c<<2));
    if(tbl == (uint16)0x0){printq("GDT\n");}
    if(tbl == (uint16)0x01){printq("IDT\n");}
    if(tbl == (uint16)0x10){printq("LDT\n");}
    if(tbl == (uint16)0x11){printq("IDT\n");}
    tbl = (uint16)0x0;
    for(uint64 i = 0; i < 13; ++i){tbl = tbl|(c<<(3+i));}
    printq("Index: 0x%X\n", error&(uint64)0x1FFF);
    printq("Instruction: ");
    uint8 *m = (uint8*)r.rip;
    char ist[4];
    //read_instructions(r.rip);
    ist[3] = '\0';
    printq((char*)&ist);
    kernelPanic();
}

void divz(regs r){
    printq("0/0\n");
    dumpRegisters(r);
    kernelPanic();
}

void nmi(regs r){
    printq("NMI\n");
    dumpRegisters(r);
    kernelPanic();
}

void brk(regs r){
    printq("Breakpoint\n");
    dumpRegisters(r);
    kernelPanic();
}

void ovfl(regs r){
    printq("Overflow exception\n");
    dumpRegisters(r);
    kernelPanic();
}

void bound(regs r){
    printq("Exceeded bounds\n");
    dumpRegisters(r);
    kernelPanic();
}

void ivop(regs r){
    printq("Invalid operand\n");
    dumpRegisters(r);
    kernelPanic();
}

void dnav(regs r){
    printq("Device not avalible\n");
    dumpRegisters(r);
    kernelPanic();
}

void dbflt(regs r){
    printq("Double Fault\n");
    dumpRegisters(r);
    kernelPanic();
}

void invtss(regs r){
    printq("Invalid TSS\n");
    dumpRegisters(r);
    kernelPanic();
}

void npseg(regs r){
    printq("Segment not present\n");
    dumpRegisters(r);
    kernelPanic();
}

void stckf(regs r){
    printq("Stack segment fault\n");
    dumpRegisters(r);
    kernelPanic();
}

void fpue(regs r){
    printq("FPU Error\n");
    dumpRegisters(r);
    kernelPanic();
}

void alchk(regs r){
    printq("Alignment check\n");
    dumpRegisters(r);
    kernelPanic();
}

void mchk(regs r){
    printq("Machine check\n");
    dumpRegisters(r);
    kernelPanic();
}

void simdfp(regs r){
    printq("SIMD execption");
    dumpRegisters(r);
    kernelPanic();
}

void ld_handlers(){
    addInterruptHandler(gpf, 13);
    addInterruptHandler(divz, 0x0);
    addInterruptHandler(nmi, 0x2);
    addInterruptHandler(brk, 0x3);
    addInterruptHandler(ovfl, 0x4);
    addInterruptHandler(bound, 0x5);
    addInterruptHandler(ivop, 0x6);
    addInterruptHandler(dnav, 0x7);
    addInterruptHandler(dbflt, 0x8);
    addInterruptHandler(invtss, 0xA);
    addInterruptHandler(npseg, 0xB);
    addInterruptHandler(stckf, 0xC);
    addInterruptHandler(fpue, 0x10);
    addInterruptHandler(alchk, 0x11);
    addInterruptHandler(mchk, 0x12);
    addInterruptHandler(simdfp, 0x13);
}