#include "libstd.h"
#include "libio.h"
#include "libstr.h"
#include "libarg.h"
#include "libtask.h"
#include "libksystem.h"

//For getting interrupts easily to other processes/functions

const int limit = 10; //Temporary for testing

uint64 hookInt(uint8 mode, uint16 type, uint16 flags, uint16 mask, hookfunc func, uint64 id, uint64 idata){
    struct task *t = currentTask();
    struct hookData data;
    switch(mode){
        case 0: //Add
        for(uint32 i = 0; i < 10; ++i){
            if(t->taskHooks[i].inUse == false){
                t->taskHooks[i].flags = flags;
                t->taskHooks[i].mask = mask;
                t->taskHooks[i].type = type;
                t->taskHooks[i].func = func;
                t->taskHooks[i].inUse = true;
                return i;
            }
        }
        printq("%qNo free hooks\n", QWARNING);
        return 0;
        break;
        case 1: //Remove
        break;
        case 2: //Change mask
        break;
        case 3: //Disable hook
        break;
        case 4: //Enable hook
        break;
        case 5: //Run hooks
        for(uint32 i = 0; i < limit; ++i){
            if(t->taskHooks[i].inUse == true && t->taskHooks[i].type == type){
                data.type = type;
                data.data = idata;
                data.flags = flags;
                if(t->taskHooks[i].flags&0x1){
                    data.type = t->taskHooks[i].mask;
                }
                t->taskHooks[i].func(&data);
                return;
            }
        }
        break;
        default:
        printq("%qUnknown mode\n", QWARNING);
        return;
        break;
    }
}