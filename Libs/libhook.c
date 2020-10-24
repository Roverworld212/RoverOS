#include "libhook.h"
#include "libargs.h"
#include "kheaders.h"

const uint32 hsz = 29;
hookData hooks[29];
commandData commands[29];

void hook_int(uint32 type, uint8 ftype, int s, hook fc, uint8 data){
    if(kvinf.kio.disableHooks){return;}
    hook call;
    switch(type){
        case 0://Add
        if(fc == NULL){return;}
        if(s < 0){
            for(uint32 i = 0; i<=hsz; ++i){
                if(!hooks[i].isUsed){
                    hooks[i].isUsed = true;
                    hooks[i].type = ftype;
                    hooks[i].func = fc;
                    return;
                }
            }
        }
        else{
            hooks[s].isUsed = true;
            hooks[s].type = ftype;
            hooks[s].func = fc;
            return;
        }
        break;
        case 1://Remove
        hooks[s].isUsed = false;
        hooks[s].type = NULL;
        hooks[s].func = NULL;
        break;
        case 2://Run
        if(data == NULL || ftype == NULL){return;}
        for(uint32 i = 0; i<=hsz; ++i){
            if(hooks[i].type == ftype){
                if(hooks[i].func != NULL){
                    call = hooks[i].func;
                    call(data);
                }
            }
        }
        break;
        case 3://Clear
        for(uint32 i = 0; i<=hsz; ++i){
            hooks[i].isUsed = false;
            hooks[i].type = NULL;
            hooks[i].func = NULL;
        }
        break;
        default:
        printq("%wBad Type\n", __F__);
    }
}

void command_int(uint32 type, char *cmd, argData *args, command cmdfc, char *ah, uint32 acgrp, uint32 acright){
    switch(type){
        case 0: //Add
        for(uint32 i = 0; i<=hsz; ++i){
            if(commands[i].isUsed){
                commands[i].isUsed = false;
                commands[i].cmd = cmd;
                commands[i].func = cmdfc;
                if(ah == NULL){commands[i].man == (char*)"No Args";}else{
                    commands[i].man = ah;
                }
                return;
            }
        }
        printq("%wAll spaces used\n", __F__);
        break;
        case 1: //Remove
        for(uint32 i = 0; i<=hsz; ++i){
            if(commands[i].cmd == cmd){
                commands[i].isUsed = true;
                commands[i].cmd = NULL;
                commands[i].func = NULL;
                commands[i].man = NULL;
                return;
            }
        } 
        break;
        case 2: //Clear
        for(uint32 i = 0; i <= hsz; ++i){
            commands[i].isUsed = true;
            commands[i].cmd = NULL;
            commands[i].func = NULL;
            commands[i].man = NULL;
        }
        break;
        case 3: //List
        for(uint32 i = 0; i<=hsz; ++i){
            if(!commands[i].isUsed){
                printf("%s\n", commands[i].cmd);
            }
        }
        break;
        case 4: //Run
        for(uint32 i = 0; i <= hsz; ++i){
            if((!commands[i].isUsed) && (strcmp(commands[i].cmd, cmd))){
                commands[i].func(args);
                return;
            }
        }
        default:
        printq('%iBad Type\n', __F__);
        break;
    }
}