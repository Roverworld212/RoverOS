#include "libstd.h"
#include "libio.h"

void outb(uint16 port, uint8 data){
  asm volatile("outb %1, %0" : : "dN" (port), "a" (data));
}

uint8 inb(uint16 port){
  uint8 r;
  asm volatile("inb %1, %0" : "=a" (r) : "dN" (port));
  return r;
}