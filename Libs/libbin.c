#include "kheaders.h"
#include "libbin.h"

void printbit32(uint32 b){
    for(uint32 i = 0; i < 32; ++i){
        if(b&(BCHECK32>>i)){printq("1");}else{printq("0");}
    }
}

void printbit8(uint8 b){
    for(uint32 i = 0; i < 8; ++i){
        if(b&(BCHECK8>>i)){printq("1");}else{printq("0");}
    }
}