#include "kheaders.h"
#include "kutil.h"
#include "libui.h"
#include "rcom.h"
#include "libbin.h"
#include "cpuid.h"

extern void getCPUINF(uint32);

void getCPUI(){ //We don't support legacy devices around here
    //printf("Getting CPUID Information\n");
    printq("---CPUID---\n");
    getCPUINF(0);
    if((kvinf.kcpuid.vendorid != VENDOR_INTEL) && (kvinf.kcpuid.vendorid != VENDOR_AMD) && (kvinf.kcpuid.vendorid != VENDOR_VM)){gcfend(); return;}//Unkown Vendor
    getCPUINF(1);
    getCPUINF(7);
    if(kvinf.kcpuid.vendorid == VENDOR_AMD){getCPUINF(0x80000001);}
    gcfend();
}

void gcfend(){
    printq("-----------\n");
}

void cpur(uint32 eax, uint32 ebx, uint32 ecx, uint32 edx){
    switch (eax){
        case 0: //Vendor Name
        printq("VENDOR: ");
        printf("Vendor: ");
        uint32 b = 0;
        for(uint32 i = 0; i < 32; i+=8){
            sendData(COM1, ((uint8)(ebx>>i)));
            printf("%c", ((uint8)(ebx>>i)));
            kvinf.kcpuid.vendor[b] = ((uint8)(ebx>>i));
            ++b;
        }
        for(uint32 i = 0; i < 32; i+=8){
            sendData(COM1, ((uint8)(edx>>i)));
            printf("%c", ((uint8)(edx>>i)));
            kvinf.kcpuid.vendor[b] = ((uint8)(edx>>i));
            ++b;
        }
        for(uint32 i = 0; i < 32; i+=8){
            sendData(COM1, ((uint8)(ecx>>i))); //printq mangles the data somehow
            printf("%c", ((uint8)(ecx>>i)));
            kvinf.kcpuid.vendor[b] = ((uint8)(ecx>>i));
            ++b;
        }
        printf("\n");
        printq(" - HID: %x\n", (ebx+ecx+edx));
        kvinf.kcpuid.vendorid = (ebx+ecx+edx);
        break;
        case 1: //Supported Features
        printq("--Features--\n");
        if(kvinf.kcpuid.vendorid == VENDOR_INTEL){
            for(uint32 i = 0; i < 32; ++i){
                if(edx&(BCHECK32>>i)){printq(chedx[i]); printq("\n");}
            }
            for(uint32 i = 0; i < 32; ++i){
                if(ecx&(BCHECK32>>i)){printq(checx[i]); printq("\n");}
            }
        }
        break;
        case 7:
        for(uint32 i = 0; i < 32; ++i){
            if(ecx&(BCHECK32>>i)){printq(checx7[i]); printq("\n");}
        }
        for(uint32 i = 0; i < 32; ++i){
            if(edx&(BCHECK32>>i)){printq(chedx7[i]); printq("\n");}
        }
        break;
        case 0x80000001:
        for(uint32 i = 0; i < 32; ++i){
            if(edx&(BCHECK32>>i)){printq(chedxAMD[i]); printq("\n");}
        }
        for(uint32 i = 0; i < 32; ++i){
            if(ecx&(BCHECK32>>i)){printq(chedxAMD[i]); printq("\n");}
        }
        break;
        default:
        printq("EAX: %x\n", eax);
        kernelPanic("Bad CPUID eax value! Is it known?");
        break;
    };
    return;
}