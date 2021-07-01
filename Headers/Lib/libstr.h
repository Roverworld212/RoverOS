#ifndef LIBSTR_H
#define LIBSTR_H

#include "types.h"

extern uint32 dCount(int n);
extern void reverse(char *str, uint32 len);
extern char *itoa(int num, char *str, int base);
extern int atoi(const char *str);
extern int strcmp(const char *s1, const char *s2);
extern uint32 strlen(const char *str);
extern void strcpy(char *d1, char *s1);

#endif