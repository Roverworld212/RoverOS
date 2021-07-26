#ifndef LIBARGS_H
#define LIBARGS_H

#include <stdarg.h> //I will make my own the second I can

#define CAST_NORMAL 0
#define CAST_INT 1
#define CAST_CHAR 2
#define CAST_DOUBLE 3
#define CAST_LONG 4
#define CAST_LLONG 5
static const uint64 type_sz_com[6] = {
    sizeof(int)*2,
    sizeof(int)*2,
    sizeof(char)*2,
    sizeof(double)*2,
    sizeof(unsigned long long)*2,
    sizeof(unsigned long long)*2
};

#define GET_SZBYTES(v) (type_sz_com[v]-1)

#endif