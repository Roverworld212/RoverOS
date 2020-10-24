#ifndef LIBUTILS_H
#define LIBUTILS_H

#include "types.h"

extern int Clamp(int in, int min, int max);
extern void outb(uint16 port, uint8 data);
extern uint8 inb(uint16 port);
extern uint32 dCount(int n);
extern void itoa(int n, char *num);
extern int atoi(char *str);
extern void memcpy(uint8 *d, uint8 *s, uint8 l);
extern void memset(uint8 *d, uint8 v, uint32 l);
extern void memshift(uint8 *s, uint32 a);
extern int strcmp(const char *s1, const char *s2);
extern uint32 strlen(const char *str);
extern void strcpy(char *d1, const char *s1);
extern uint32 rand();

#define SRAND 2.99999

#endif