#include "kheaders.h"
#include "libvalloc.h"
#include "libui.h"

uint32 valloc_int(uint32 xsz, uint32 ysz, uint32 x, uint32 y, uint32 par, uint32 layer, bool draw, bool vsync, uint32 buffm, uint32 nlp, uint32 txtp, uint32 xp, uint32 yp, uint32 mode){
    switch(mode){
        case NEW_CHILD:
        if(par == NULL){printq("%WNULL Parent!\n", __F__);}
        vui *parent = (vui*)par;
        break;
        case SELF_MODE:
        for(uint32 i = 0; i < (CHILD_MAX+1); ++i){
            if(kvui.children[i] == NULL){
                
            }
        }
        printq("%WKVUI at CHILD_MAX!\n", __F__);
        return NULL;
        break;
        default:
        printq("%WNo Mode specified!\n", __F__);
        return NULL;
        break;
    };
}