#include "kheaders.h"
#include "tss.h"

extern void switchTSS(tss*);
tss ktss;

void initTSS(){
    printq("%ILoading TSS\n", __F__);
    
}