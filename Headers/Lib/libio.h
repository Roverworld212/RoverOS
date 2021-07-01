#ifndef LIBIO_H
#define LIBIO_H

#include "types.h"

extern void outb(uint16 port, uint8 data);
extern uint8 inb(uint16 port);

#endif