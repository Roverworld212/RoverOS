#ifndef LIBHOOK_H
#define LIBHOOK_H

#include "types.h"

//For registering hooks internally
struct hook{
    //Hook type
    uint16 type;
    //Flags
    uint16 flags;
    //Only active if bit 0 is set
    //Treats interrupt type as a different interrupt
    uint16 mask;
    //Array position+processid+random = uid
    uint64 uid;
    //Function to call
    uint64 *func;
};

//Data passed to the function
struct hookData{
    //Will be overriden by mask if applicable
    uint16 type;
    //Flags currently set
    uint16 flags;
    //Data from the interrupt
    uint64 data;
};

#define addHook(type,task,flags,mask,func) hookInt(0,type,task,flags,mask,func,0)
#define removeHook(task,id) hookInt(1,0,task,0,0,0,id)
#define changeMask(task,mask,id) hookInt(2,0,task,0,mask,0,id)
#define disableHook(task,id) hookInt(3,0,task,0,0,0,id)
#define enableHook(task,id) hookInt(4,0,task,0,0,0,id)

#endif