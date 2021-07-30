#include "libstd.h"
#include "kvinf.h"
#include "ramdisk.h"
#include "paging.h"

void readFile(struct ramdisk_segment *data, struct ramdisk_segment *hd, struct ramdisk_segment *ns, uint64 file){
    struct ramdisk_file *fl = (struct ramdisk_file*)(hd->start+(sizeof(struct ramdisk_file)*file));
    if(fl->magic != RDF_MAGIC){printq("[Invalid File Magic 0x%llx]\n", fl->magic); return;}
    //Get file name and extension 
    uint64 gread = ns->start;
    uint8 *name = (uint8*)gread+fl->name;
    uint8 *ext = (uint8*)gread+fl->ext;
    printq("[%s.%s - ", name, ext);
    if(fl->flags&RDF_WRITE){printq("W");}else{printq("R");}
    if(fl->flags&RDF_KERNEL){printq("K");}else{printq("-");}
    if(fl->flags&RDF_EXEC){printq("X");}else{printq("-");}
    printq("]\n");
}

void readRamdisk(){
    uint64 rdstart = (uint64)kvinf.fs.ramdisk;
    uint64 rdend = (uint64)kvinf.fs.ramdiskEnd;
    //Read master header for valid magic
    struct ramdisk_header *master = (struct ramdisk_header*)rdstart;
    if(master->magic != RDMAGIC){printq("%qInvalid master magic 0x%llx\n", QERROR, master->magic); return;}
    printq("%qRamdisk located at 0x%llx\n", QINFO, rdstart);
    printq("%qReading segments\n", QINFO);
    //Segments to save for later use
    struct ramdisk_segment *fdata_segment;
    struct ramdisk_segment *header_segment;
    struct ramdisk_segment *next_segment;
    for(uint64 i = 0; i < master->segments; ++i){
        struct ramdisk_segment *segment = (struct ramdisk_segment*)SEGMENT_POSITION(rdstart, i);
        if(segment->magic != RDSEG_MAGIC){printq("%q[0x%llx]Invalid segment magic 0x%llx\n", QWARNING, i, segment->magic); continue;}
        printq("[ID: 0x%hx - Start: 0x%llx - Size: 0x%llx]\n", segment->id, segment->start, segment->size);
        //Change start to the actual memory location
        segment->start = SEGMENT_DATA_START(rdstart, segment->start, master->segments);
        switch(segment->id){
            case RDSEG_DATA:
            fdata_segment = segment;
            break;
            case RDSEG_HEADERS:
            header_segment = segment;
            break;
            case RDSEG_NEXT:
            next_segment = segment;
            break;
            default:
            printq("%qUnknown segment ID 0x%hx\n", QWARNING, segment->id);
            break;
        }
    }
    readFile(fdata_segment, header_segment, next_segment, 0);
}