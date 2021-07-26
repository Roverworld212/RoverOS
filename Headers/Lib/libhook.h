#ifndef LIBHOOK_H
#define LIBHOOK_H

#include "types.h"

//Data passed to the function
struct hookData{
    //Will be overriden by mask if applicable
    uint16 type;
    //Flags passed by interrupt
    uint16 flags;
    //Data from the interrupt
    uint64 data;
};

typedef (*hookfunc)(struct hookData*);

//For registering hooks internally
struct hook{
    //Hook type
    uint16 type;
    //In use
    bool inUse;
    //Flags
    uint16 flags;
    //Only active if bit 0 is set
    //Treats interrupt type as a different interrupt
    uint16 mask;
    //Function to call
    hookfunc func;
};

/*
hookData flags
Bit#|Desc
0   |If set, the data is a keyboard release code
1   |If set, the data is a character
2   |If set, the data is from a mouse interrupt
hook flags
*/


#define addHook(type,flags,mask,func) hookInt(0,type,flags,mask,func,0,0)
#define removeHook(id) hookInt(1,0,0,0,0,id,0)
#define changeMask(mask,id) hookInt(2,0,0,mask,0,id,0)
#define disableHook(id) hookInt(3,0,0,0,0,id,0)
#define enableHook(id) hookInt(4,0,0,0,0,id,0)
#define runHook(type,flags,idata) hookInt(5,type,flags,0,0,0,idata)

#define KEYBOARD_HOOK 0x1

#endif