#ifndef KMALLOC_H
#define KMALLOC_H

extern uint32 kmalloc_int(uint32 amt, bool a, char *fnc);

#define kmalloc(amt, a) kmalloc_int(amt, a, __FUNCTION__)

#endif