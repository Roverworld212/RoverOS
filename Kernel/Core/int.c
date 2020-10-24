#include "kheaders.h"
#include "libcom.h"
#include "setup.h"
#include "libhook.h"
#include "libargs.h"

void printRegs(regs *r){
  printq("-----REGISTERS-----\n");
  printq("DS: %x\n", r->ds);
  printq("EDI: %x\n", r->edi);
  printq("ESI: %x\n", r->esi);
  printq("EBP: %X\n", r->ebp);
  printq("UN_ESP: %x\n", r->unused_esp);
  printq("EBX: %x\n", r->ebx);
  printq("EDX: %x\n", r->edx);
  printq("ECX: %x\n", r->ecx);
  printq("EAX: %x\n", r->eax);
  printq("INT: %x\n", r->int_no);
  printq("ERR: %x\n", r->err_code);
  printq("EIP: %x\n", r->eip);
  printq("CS: %x\n", r->cs);
  printq("EFLAGS: %x\n", r->eflags);
  for(uint32 i = 0; i < 32; ++i){
    if((0x40000000>>i)&r->eflags){
      printq("1");
    }
    else{
      printq("0");
    }
  }
  printq("\n");
  printq("ESP: %x\n", r->esp);
  printq("SS: %x\n", r->ss);
  printq("-------------------\n");
}

void timerInterrupt(regs r){

}

void kbInterrupt(regs r){
  uint32 kcode = inb(0x60);
  switch(kcode){
    case SPACE_HOOK:
    case LSHIFT_HOOK:
    case LSHIFTR_HOOK:
    case RSHIFT_HOOK:
    case RSHIFTR_HOOK:
    case LCTRL_HOOK:
    case RCTRL_HOOK:
    case TAB_HOOK:
    case CAPS_HOOK:
    case CAPSR_HOOK:
    case ESC_HOOK:
    case BACKSPACE_HOOK:
    case BACKSPACER_HOOK:
    case FN_HOOK:
    case WIN_HOOK:
    case LALT_HOOK:
    case RALT_HOOK:
    case ENTER_HOOK:
    case ENTERR_HOOK:
    case UPA_HOOK:
    case DOWNA_HOOK:
    case LEFTA_HOOK:
    case RIGHTA_HOOK:
    runHook(kcode, kcode);
    break;
    default:
    runHook(GK_HOOK, kcode);
    break;
  }
}

//Fault Handlers

void gpf(regs r){ //General Protection Fault
  printRegs(&r);
  kernelPanic("General Protection Fault");
}

void divz(regs r){ //Divide by zero
  printRegs(&r);
  kernelPanic("Divide By Zero Exception");
}

void nmii(regs r){ //NMI Interrupt
  printRegs(&r);
  printq("NMI\n");
}

void brk(regs r){ //Breakpoint
  printRegs(&r);
  printq("Breakpoint\n");
}

void ovfl(regs r){ //Overflow Exception
  printRegs(&r);
  kernelPanic("Overflow Exception");
}

void bound(regs r){ //Bounds range exceeded
  printRegs(&r);
  kernelPanic("Bounds range exceeded");
}

void ivop(regs r){ //Invalid Operand
  printRegs(&r);
  kernelPanic("Invalid Opcode");
}

void dnav(regs r){ //Device not avalible
  printRegs(&r);
  kernelPanic("Device not avalible");
}

void dbflt(regs r){ //Double fault
  printRegs(&r);
  kernelPanic("Double Fault");
}

void invtss(regs r){ //Invalid TSS
  printRegs(&r);
  kernelPanic("Invalid TSS");
}

void npseg(regs r){ //Segmnent not present
  printRegs(&r);
  kernelPanic("Segment not present");
}

void stckf(regs r){ //Stack-segment fault
  printRegs(&r);
  kernelPanic("Stack fault");
}

void fpue(regs r){ //FPU error
  printRegs(&r);
  kernelPanic("x87 FPU error");
}

void alchk(regs r){ //Alignment check
  printRegs(&r);
  kernelPanic("Alignment Check");
}

void mchk(regs r){ //Machine Check
  printRegs(&r);
  kernelPanic("Machine Check");
}

void simdfp(regs r){//SIMD FP Exception
  printRegs(&r);
  kernelPanic("SIMD Floating-Point Exception");
}

void setupInt(uint32 freq){
    printq("%ISetting up Interrupts\n", __F__);
    printq("%ILoading fault handlers\n", __F__);
    addHandler(13, &gpf);
    addHandler(0x0, &divz);
    addHandler(0x2, &nmii);
    addHandler(0x3, &brk);
    addHandler(0x4, &ovfl);
    addHandler(0x5, &bound);
    addHandler(0x6, &ivop);
    addHandler(0x7, &dnav);
    addHandler(0x8, &dbflt);
    addHandler(0xA, &invtss);
    addHandler(0xB, &npseg);
    addHandler(0xC, &stckf);
    addHandler(0x10, &fpue);
    addHandler(0x11, &alchk);
    addHandler(0x12, &mchk);
    addHandler(0x13, &simdfp);
    printq("%ILoading other handlers\n", __F__);
    addHandler(IRQ0, &timerInterrupt);
    uint32 div = 1193180/freq;
    outb(0x43, 0x36);
    uint8 l = (uint8)(div&0xFF);
    uint8 h = (uint8)((div>>8)&0xFF);
    outb(0x40, l);
    outb(0x40, h);
    printq("%I%gPIT - Freq: %x Div: %x L: %x H: %x\n", __F__, freq, div, l, h);
    printq("%I%gLoaded PIT\n", __F__);
    addHandler(IRQ1, &kbInterrupt);
    printq("%I%gFinished Setting up Interrupts\n", __F__);
}

irh handlers[256];

void addHandler(uint8 n, irh h){
  handlers[n] = h;
}

void isrHandler(regs r){
  if(handlers[r.int_no] != NULL){
    //printq("%IInt!\n", __F__);
    irh handler = handlers[r.int_no];
    handler(r);
    return;
  }
  asm volatile("cli");
  printq("%I%gUnhandled interrupt! %x\n", __F__, r.int_no);
  for(;;);
}

void irqHandler(regs r){
  if(r.int_no >= 40){
    outb(0xA0, 0x20);
  }
  outb(0x20, 0x20);
  if(handlers[r.int_no] != NULL){
    irh handler = handlers[r.int_no];
    handler(r);
    return;
  }
  asm volatile("cli");
  printq("%I%gUnhandled irq! %x\n", __F__, r.int_no);
  for(;;);
}