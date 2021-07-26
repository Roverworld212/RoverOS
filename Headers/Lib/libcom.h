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

extern void printq(const char *format, ...);

#define qhexc(v) qhex_int(CAST_CHAR, v)
#define qhexi(v) qhex_int(CAST_INT, v)

#endif