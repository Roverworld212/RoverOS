#include "libstd.h"
#include "cpuid.h"

const char *proctype[4] = {"OEM", "IOP", "Dual", "Reserved"};
const char *chrdx[32] = {
    "x87 FPU",
    "VME Virtual 8086",
    "Debug Extensions",
    "Page Size Extensions",
    "Time Stamp Counter",
    "Model Specific Registers",
    "Physical Address Extensions 64 Bit",
    "Machine Check Exception",
    "CX8",
    "APIC",
    "Reserved",
    "SYSENTER & SYSEXIT",
    "MTRR",
    "Page Global Enable Bit CR4",
    "Machine Check Architecture",
    "CMOV & FCMOV",
    "Page Attribute Table",
    "36 Bit Page Size Extensions",
    "Processor Serial Number",
    "CFLUSH",
    "Reserved",
    "Debug Store",
    "Thermal ACPI",
    "MMX",
    "FXSR",
    "SSE",
    "SSE2",
    "SS",
    "Hyper-Threading",
    "Thermal Monitor",
    "IA64 x86",
    "PBE"
};

const char *chrcx[32] = {
    "SSE3",
    "pclmulqdq",
    "64 Bit Debug Store",
    "MONITOR & MWAIT",
    "CPL Qualified Debug Store",
    "Virtual Machine Extensions",
    "Safer Mode Extensions",
    "Enhanced SpeedStep",
    "Thermal Monitor 2",
    "Supplemental SSE3",
    "L1 Context ID",
    "Silicon Debug Interface",
    "Fused Multiply-add",
    "CX16",
    "XTPR",
    "Perfmon & Debug Capability",
    "Reserved",
    "PCID",
    "Direct Cache Access",
    "SSE 4.1",
    "SSE 4.2",
    "x2 APIC",
    "MOVBE Instructions",
    "POPCNT Instructions",
    "TSC-Deadline",
    "AES",
    "XSAVE",
    "OSXSAVE",
    "Advanced Vector Extensions",
    "F16C",
    "On-Chip Random Number Generator",
    "Hypervisor"
};

const char *chrcx7[32] = {
    "FSGSBASE",
    "IA32_TSC_ADJUST",
    "Software Guard Extensions",
    "Bit Manipulation Instruction Set 1",
    "TSX Hardware Lock Elision",
    "Advanced Vector Extensions 2",
    "FDP_EXCPTN_ONLY",
    "Supervisor Mode Execution Prevention",
    "Bit Manipulation Instruction Set 2",
    "Enhanced REP MOVSB/STOSB",
    "INVPCID Instruction",
    "TSX Restricted Transactional Memory",
    "Platform Quality of Service Monitoring",
    "FPU CS and FPU DS deprecated",
    "Intel Memory Protection Extensions",
    "Platform Qualtiy of Service Enforcement",
    "AVX-512 Foundation",
    "AVX-512 Doubleword and Quadword Instructions",
    "RDSEED Instruction",
    "Intel ADX",
    "Supervisor Mode Access Prevenetion",
    "AVX-512 Integer Fused Multiply-Add Instructions",
    "PCOMMIT Instruction",
    "CLFLUSHOPT Instruction",
    "CLWB Instruction",
    "Intel Processor Trace",
    "AVX-512 Prefetch Instructions",
    "AVX-512 Exponential and Reciprocal Instructions",
    "AVX-512 Conflict Detection Instructions",
    "Intel SHA Extensions",
    "AVX-512 Byte and Word Instructions",
    "AVX-512 Vector Length Extensions"
};

const char *chrdx7[32] = {
    "Reserved",
    "Reserved",
    "AVX-512 4 Register Network Instructions",
    "AVX-512 4 Register Multiply Accumulation Single Precision",
    "Fast Short REP MOVSB",
    "Reserved",
    "Reserved",
    "Reserved",
    "AVX-512 VP2INTERSECT Doubleword and Quadword Instructions",
    "Special Register Buffer Data Sampling Mitigations",
    "VERW Instruction clears CPU buffers",
    "Reserved",
    "Reserved",
    "TSX_FORCE_ABORT",
    "Serialized Instruction Execution",
    "Hybrid",
    "TSX Suspend Load Address Tracking",
    "Reserved",
    "Memory Encryption Instructions",
    "Architectural Last Brach Records",
    "Control Flow Enforcment",
    "Reserved",
    "Tile Computation on bfloat16",
    "Reserved",
    "Tile Architecture",
    "Tile Computation on 8Bit integers",
    "Speculation Control",
    "Single Thread Indirect Branch",
    "IA32_FLUSH_CMD MSR",
    "Side Channel Mitigations",
    "MSR Specific Core Capabilities",
    "Speculative Store Bypass"
};

const char *chrdxAMD[32] = {
    "x87 FPU",
    "VME Virtual 8086",
    "Debug Extensions",
    "Page Size Extensions",
    "Time Stamp Counter",
    "Model Specific Registers",
    "Physical Address Extensions 64 Bit",
    "Machine Check Exception",
    "CX8",
    "APIC",
    "Reserved",
    "SYSCALL & SYSRET",
    "MTRR",
    "Page Global Bit CR4",
    "Machine Check Architecture",
    "CMOV & FCMOV",
    "Page Attribute Table",
    "36 Bit Page Size Extensions",
    "Reserved",
    "Multiprocessor Capable",
    "Reserved",
    "Reserved",
    "Extended MMX",
    "MMX",
    "FXSR",
    "SSE",
    "Gigibyte pages",
    "RDTSCP",
    "Long Mode 64 Bit",
    "Extended 3DNow",
    "3DNow",
    "Reserved"
};

const char *chrcxAMD[32] = {
    "LAHF",
    "CMP Legacy",
    "Secure Virtual Machine",
    "Extended APIC",
    "CR8 Legacy",
    "Advanced Bit Manipulation",
    "SSE4A",
    "Misaligned SSE",
    "3DNow Prefetch",
    "OS Visible Workaround",
    "Instruction Based Sampling",
    "XOP Instruction Set",
    "SKINIT Instruction Set",
    "Watchdog Timer",
    "Reserved",
    "Light Weight Profiling",
    "4 Operands Fused Multiply",
    "Translation Cache Extension",
    "Reserved",
    "Trailing Bit Manipulation",
    "Topology Extensions",
    "Core Preformance Counter Extensions",
    "NB Preformance Counter Extensions",
    "Reserved",
    "Data Breakpoint Extensions",
    "Preformance TSC",
    "L2I Perf Counter Extensions",
    "Reserved",
    "Reserved",
    "Reserved"
};

void readCpuid(){
    printq("====CPUID====\n");
    cpuidInt(0);
    cpuidInt(1);
    cpuidInt(7);
    printq("=============\n");
}

void cpuidRet(uint64 rax2, uint64 rbx2, uint64 rdx2, uint64 rcx2, uint64 id2){
    //Save them before the registers change
    uint64 rax, rbx, rdx, rcx, id;
    rax = rax2; rbx = rbx2; rdx = rdx2; rcx = rcx2; id = id2;
    switch(id){
        case 0:
        printq("Vendor: ");
        for(uint32 i = 0; i < 32; i+=8){
            printq("%c", (uint8)(rbx>>i));
        }
        for(uint32 i = 0; i < 32; i+=8){
            printq("%c", (uint8)(rdx>>i));
        }
        for(uint32 i = 0; i < 32; i+=8){
            printq("%c", (uint8)(rcx>>i));
        }
        printq("\n");
        break;
        case 1:
        printq("Processor type: OEM\n", proctype[(rax2&((1<<13)&(1<<12)))]);
        break;
        case 7:
        printq("CPU Features:\n");
        for(uint32 i = 0; i < 32; ++i){
            if(rcx&(0x8000000000000000>>i)){printq("%s\n", chrcx7[i]);}
        }
        for(uint32 i = 0; i < 32; ++i){
            if(rdx&(8000000000000000>>i)){printq("%s\n", chrdx7[i]);}
        }
        break;
        default:
        printq("Unknown rax value\n");
        break;
    }
    return;
}