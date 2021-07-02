#include "libstd.h"
#include "kvinf.h"
#include "cpuid.h"

void readCpuid(){
    printq("======CPUID======\n");
    //Get Brand string
    CPUIDint(0);
    return;
}

void cpuidRet(uint64 rax, uint64 rdi){
    switch(rax){
        case 0: //Brand
        break;
        default:
        printq("%wCPUID rax returned unkown value of 0x%llx\n", rax);
        break;
    }
    return;
}