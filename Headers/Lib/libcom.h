#ifndef LIBCOM_H
#define LIBCOM_H

#include "types.h"

#define QINFO __FUNCTION__, 'I'
#define QWARNING __FUNCTION__, 'W'
#define QERROR __FUNCTION__, 'E'

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

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

extern void printq(const char *format, ...);

#define qhexc(v) qhex_int(CAST_CHAR, v)
#define qhexi(v) qhex_int(CAST_INT, v)

#endif