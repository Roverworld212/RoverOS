#include "kheaders.h"
#include "paging.h"

//((0xffffffff^0xffffffff)|(0xffffffff^0xffffffff)&0x0)+(1*1)+1

uint32 *frames;
uint32 nFrames;
uint32 idxam;

void printFrameMap(){
    if(!kvinf.kio.printFrames){return;}
    printq("---FMAP---\n");
    printq("MAX-150\n");
    uint32 s = 0;
    uint32 frm;
    for(uint32 b = 0; b < 150; ++b){
        frm = (uint32)frames[b];
        printq("%x - ", b);
        for(uint32 i = 0; i < 32; ++i){
            if(IS_SET(i, (uint32)frm)){printq("1");}else{printq("0");}
        }
        printq(" - %x", (uint32)frames[b]);
        ++s;
        if(s >= 2){
            printq("\n");
            s = 0;
        }
        else{
            printq("  ");
        }
    }
    printq("\n----------\n");
}

void setupFrames(uint32 memsz){
    frames[0] = (uint32*)NULL;
    printq("%IMemsz: %x\n", __F__, memsz);
    nFrames = ppb(memsz);
    idxam = nFrames/32;
    for(uint32 i = 0; i <= idxam; ++i){
        clearSet(i);
    }
    printq("%Iidxam: %x\n", __F__, idxam);
    printq("%IFrames: %x\n", __F__, nFrames);
    frames[0] = SET(0, frames[0]);
    frames[0] = SET(3, frames[0]);
    frames[0] = SET(9, frames[0]);
}

void freeFrame(uint32 fdr){
    uint32 st = (fdr>>22);
    uint32 t = (st>>10);
    uint32 id = (st%1024);
    //if(!IS_SET(f, frames)){printq("%wFrame already cleared! %x\n", __F__, f); return;}
    frames[t] = UNSET(id, frames[t]);
}

void setFrame(uint32 ad){
    //if(!IS_ALIGNED(addr)){printq("%wUn-aligned address!\n", __F__); return;}
    //if(IS_SET(f, frames)){printq("%wFrame already set! %x\n", __F__, f); return;}
    //frames = SET(f, frames);
    uint32 idx = GET_IDX(ad);
    frames[idx] = SET(GET_FRAME(ad), frames[idx]);
}

void clearSet(uint32 set){
    if(kvinf.kio.clearsetLog){printq("%IClearing set %x from %x\n", __F__, set, frames[set]);}
    frames[set] = (uint32*)NULL;
}

//32-Frame Set 32-Frame Number = 64

uint64 firstFrame(){
    uint64 ret;
    for(uint32 i = 0; i <= idxam; ++i){
        if(frames[i] != FFULL){//Free frame somewhere in here
            ret = ret|i;
            for(uint32 b = 0; b <= 32; ++b){
                if(IS_SET(b, (uint32)frames[i])){
                    
                }
                else{
                    ret = ret|((uint64)b>>32);
                    return ret;
                }
            }
            printq("%kSet: %x\n", __F__, i);
            kernelPanic("False Positive frame set!");
        }
    }
    kernelPanic("No Free Frames! Memory Full?");
}