#ifndef LIBSTD_H
#define LIBSTD_H

#include "types.h"
#include "libmem.h"
#include "libcom.h"
#include "libstr.h"

#define malloc(s) malloc_int(__FUNCTION__, s)

#endif