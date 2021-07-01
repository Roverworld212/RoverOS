#include "libstd.h"
#include "libmem.h"

void memcpy(uint8 *d, uint8 *s, uint8 l){
  const uint8 *sp = (const uint8 *)s;
  uint8 *dp = (uint8 *)d;
  for(; l != 0; l--){
    *dp++ = *sp++;
  }
}

void memset(uint8 *d, uint8 v, uint32 l){
  uint8 *t = (uint8 *)d;
  for(; l != 0; l--){
    *t++ = v;
  }
}

void swap(int *x, int *y){
  int *b;
  b = x;
  x = y;
  y = b;
}
