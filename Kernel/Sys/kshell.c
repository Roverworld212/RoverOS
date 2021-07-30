#include "libstd.h"
#include "libio.h"
#include "libui.h"
#include "libhook.h"
#include "libtask.h"
#include "kvinf.h"
#include "basefont.h"
#include "scancodes.h"

struct gui *ui;
struct text *txt;
struct task *kshellTask;
struct hook shellHooks[20];

char scanToChar(uint8 d){
    if(kvinf.io.caps && kvinf.io.shift){
        return (char)uskb[d];
    }
    if(kvinf.io.caps || kvinf.io.shift){
        return (char)cuskb[d];
    }
    return (char)uskb[d];
}

hookfunc *keyboardhook(struct hookData *data){
    if(data->flags&KBFLAG_CHAR){
        if(data->flags&KBFLAG_RELEASE){return;}
        printf("%c", scanToChar((uint8)data->data));
    }
    if(data->flags&KBFLAG_TOGGLE){
        if(data->data == 0x3A){
            if(kvinf.io.caps){kvinf.io.caps=false;}
            else{kvinf.io.caps = true;}
        }
        if(data->data >= 0x2A && data->data <= 0xB6){
            if(data->data==0x2A||data->data==0x36){kvinf.io.shift=true;}
            if(data->data==0xAA||data->data==0xB6){kvinf.io.shift=false;}
        }
    }
    if(data->data==0x1C){
        txt->lcpX = 0;
        printf("\n>");
    }
}

void initKshell(){
    printq("%qLoading kshell\n", QINFO);
    printf("Loading Kshell\n");
    struct task *t = currentTask();
    ui = t->taskUI;
    txt = t->stdio;
    createTask(ui,txt,kshellTask,20,&shellHooks);
    cli();
    addHook(KEYBOARD_HOOK,0,0x0,keyboardhook);
    printf(">");
    sti();
}