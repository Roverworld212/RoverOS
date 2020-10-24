#include "kheaders.h"
#include "ramdisk.h"
#include "rcom.h"

uint8 *afile;

void setupRamdisk(){
    if(kvinf.kvfs.rdStart == NULL){kernelPanic("No ramdisk loaded");}
    struct masterHeader *master = (struct masterHeader*)kvinf.kvfs.rdStart;
    struct ramHeader *header;
    printq("==========RAMDISK==========\n");
    printq("Ramdisk loaded at: %x\n", kvinf.kvfs.rdStart);
    if(master->magic != RDMASTERMAGIC){printq("%eInvalid master magic %x\n", __F__, master->magic);}
    uint32 next = kvinf.kvfs.rdStart+sizeof(struct masterHeader);
    header = (struct ramHeader*)next;
    if(header->magic != RDMAGIC){printq("%eInvalid header magic %x\n", __F__, header->magic);}
    afile = (uint8*)next+sizeof(struct ramHeader);
    printq("===========================\n");
}

void readKvargs(){//Should always be the 1st file
    uint8 statment[29];
    uint8 arg[29];
    uint8 *data = (uint8*)afile;
    uint32 i = 0;
    uint32 b = 0;
    uint32 c = 0;
    bool uarg = false;
    //We can ignore a lot of checks as the pre-parser will have checked already
    while(data[i] != '#'){
        switch(data[i]){
            case NULL: //Currently a bug where NULL data is somehow in the file
            break;
            case '\n':
            break;
            case '(':
            uarg = true;
            break;
            case ')': 
            uarg = false;
            break;
            case ';':
            arg[b] = '\0';
            statment[c] = '\0';
            setKvarg(&statment, &arg);
            b = 0;
            c = 0;
            break;
            default:
            if(b >= 29 || c >= 29){printf("%eStatment || Arg passed 29 max chars\n", __F__); return;}
            if(uarg){
                arg[b] = (uint8)data[i];
                ++b;
            }
            else{
                statment[c] = (uint8)data[i];
                ++c;
            }
            break;
        }
        ++i;
    }
    afile = (uint8*)NULL;
}

//Proper arg syntax: -<arg>(<data>);
//; Defines end of arg
//# Defines the end of args all else data will be ignored
// (<val>) Defines value to be set

uint32 ctr = 0;

void setKvarg(uint8 *data, uint8 *arg){
    bool set;
    uint32 num = NULL;
    ++ctr;
    if(arg[0] == '1' || arg[0] == 't' || arg[0] == 'T'){
        set = true;
    }
    else if(arg[0] == '0' || arg[0] == 'f' || arg[0] == 'F'){
        set = false;
    }
    num = atoi(arg);
    uint32 v = 0;
    for(uint32 i = 0; data[i] != '\0'; ++i){
        v += (uint32)data[i];
    }
    uint32 b = 0;
    switch(v){
        case 0x5f5: //-set-kernel-name
        for(; arg[b] != '\0'; ++b){
            kvinf.kernelname[b] = (char*)arg[b];
        }
        kvinf.kernelname[b] = '\0';
        break;
        case 0x2e8: //-set-log
        kvinf.kio.log = set;
        break;
        case 0x51c: //-set-debug-log
        kvinf.kio.debugOut = set;
        break;
        case 0x4c1: //-set-info-log
        kvinf.kio.infoOut = set;
        break;
        case 0x4cd: //-set-warn-log
        kvinf.kio.warnOut = set;
        break;
        case 0x53f: //-set-error-log
        kvinf.kio.errorOut = set;
        break;
        case 0x520: //-set-panic-log
        kvinf.kio.panicOut = set;
        break;
        case 0x5f8: //-set-kmalloc-log
        kvinf.kio.kmallocLog = set;
        break;
        case 0x58d: //-set-malloc-log
        kvinf.kio.mallocLog = set;
        break;
        case 0x695: //-set-clear-set-log
        kvinf.kio.clearsetLog = set;
        break;
        case 0x67e: //-set-print-frames
        kvinf.kio.printFrames = set;
        break;
        case 0x354: //-set-draw
        kvinf.kui.pfui->draw = set;
        break;
        case 0x3D9: //-set-vsync
        kvinf.kui.pfui->vsync = set;
        break;
        case 0x5F2: //-set-buffer-mode
        kvinf.kui.pfui->buffMode = num;
        break;
        case 0x4D2: //-set-x-offset
        kvinf.kui.pfui->xOffset = num;
        break;
        case 0x2FA: //-set-lpx
        kvinf.kui.pfui->lpx = num;
        break;
        case 0x2FB: //-set-lpy
        kvinf.kui.pfui->lpy = num;
        break;
        case 0x66F: //-set-text-padding
        kvinf.kui.pfui->textPadding = num;
        break;
        case 0x79C: //-set-newline-padding
        kvinf.kui.pfui->newlinePadding = num;
        break;
        case 0x5EA: //-set-font-x-size
        kvinf.kui.pfui->fontPXXSZ = num;
        break;
        case 0x5EB: //-set-font-y-size
        kvinf.kui.pfui->fontPXYSZ = num;
        break;
        case 0x3AE: //-set-scale
        kvinf.kui.pfui->scale = set;
        break;
        case 0x454: //-set-y-scale
        kvinf.kui.pfui->yScale = num;
        break;
        case 0x453: //-set-x-scale
        kvinf.kui.pfui->xScale = num;
        break;
        case 0x75E: //-set-draw-back-color
        kvinf.kui.pfui->drawBackColor = set;
        break;
        default:
        printq("%wUndefined statment: %x arg: %x\n", __F__, v, ctr);
        printq("ARG: %s\n", data);
        break;
    }
}