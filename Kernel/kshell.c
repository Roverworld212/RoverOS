#include "kheaders.h"
#include "libargs.h"
#include "libhook.h"

vui ui;

extern void tmain();
const char reservedChars[0] = {';'};
const uint32 charLimit = 100;

bool uargs;
uint32 absoCurs;
uint32 cmdCurs;
uint32 argStructCurs;
argData args[9];
char cmd[30];

char *getChar(uint8 d){
    if((kvinf.kio.shift) && (kvinf.kio.caps)){
        return (char*)uskb[d];
    }
    if((kvinf.kio.shift) || (kvinf.kio.caps)){
        return (char*)cuskb[d];
    }
    else{
        return (char*)uskb[d];
    }
}

hook shiftHook(uint8 data){
    kvinf.kio.shift = !kvinf.kio.shift;
}

hook capsHook(uint8 data){
    kvinf.kio.caps = !kvinf.kio.caps;
}

hook charHook(uint8 data){
    if(data&0x80){return;}
    if((cmdCurs >= 29) && (!uargs)){return;}
    if(absoCurs >= charLimit){return;}
    if(uargs){
        args[argStructCurs].arg[args[argStructCurs].cursor] = (char)getChar(data);
        ++args[argStructCurs].cursor;
    }
    else{
        cmd[cmdCurs] = (char)getChar(data);
        ++cmdCurs;
    }
    printf("%c", getChar(data));
    ++absoCurs;
}

hook spaceHook(uint8 data){
    if(absoCurs >= charLimit){return;}
    ++absoCurs;
    printf(" ");
}

hook backspaceHook(uint8 data){

}

hook enterHook(uint8 data){
    kvinf.kio.disableHooks = true;
    uargs = false;
    args[argStructCurs].arg[args[argStructCurs].cursor+1] = '\0';
    cmd[cmdCurs+1] = '\0';
    absoCurs = 0;
    cmdCurs = 0;
    argStructCurs = 0;
    runCommand(&cmd, &args);
    printf("\n>");
}

command anza(commandData *args){
    printf("R.I.P anza, fuck the tards that killed you\n");
}

command uname(commandData *args){
    printf("%s", kvinf.kernelname);
}

void tmain(){
    ui = kvui;
    kvinf.kui.pfui = &ui;
    printf("Kernel Name: %s\n>", kvinf.kernelname);
    addHook(GK_HOOK, AUTO, &charHook);
    addHook(SPACE_HOOK, AUTO, &spaceHook);
    addHook(LSHIFT_HOOK, AUTO, &shiftHook);
    addHook(RSHIFT_HOOK, AUTO, &shiftHook);
    addHook(LSHIFTR_HOOK, AUTO, &shiftHook);
    addHook(RSHIFTR_HOOK, AUTO, &shiftHook);
    addHook(BACKSPACE_HOOK, AUTO, &backspaceHook);
    addHook(CAPS_HOOK, AUTO, &capsHook);
    addHook(ENTER_HOOK, AUTO, &enterHook);
    //printf("\n>");
}