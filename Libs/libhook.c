#include "libstd.h"
#include "libio.h"
#include "libstr.h"
#include "libarg.h"
#include "libksystem.h"

//For getting interrupts easily to other processes/functions

uint64 hookInt(uint8 mode, uint64 *task, uint16 type, uint16 flags, uint16 mask, uintptr *func, uint64 id){

}