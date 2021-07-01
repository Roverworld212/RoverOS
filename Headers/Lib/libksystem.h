#ifndef LIBKSYSTEM_H
#define LIBKSYSTEM_H

extern void sti();
extern void cli();
extern void hlt();

#define kernelPanic() kernelPanic_int()

#endif