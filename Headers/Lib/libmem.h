#ifndef LIBMEM_H
#define LIBMEM_H

#include "types.h"

extern void memcpy(uint8 *d, uint8 *s, uint8 l);
extern void memset(uint8 *d, uint8 v, uint32 l);
extern void swap(int *x, int *y);

#endif