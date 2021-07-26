#ifndef RAMDISK_H
#define RAMDISK_H

#include "types.h"

#define RDMAGIC 0x4D41535445524D47 //MASTERMG
#define RDF_MAGIC 0x524446494C454D47 //RDFILEMG
#define RDSEG_MAGIC 0x52445345474D4D47 //RDSEGMMG
#define RDSEG_HEADERS 0xDE //File headers
#define RDSEG_DATA 0xAF //File data
#define RDSEG_NEXT 0xCA //Name and ext

//Segment headers must immediately follow this header 
struct ramdisk_header{
    uint64 magic;
    uint64 files; //Files in disk
    uint64 size; //Ramdisk size
    uint64 segments; //Segment structures present
}__attribute__((packed));

struct ramdisk_segment{
    uint64 magic;
    uint8 id; //Segment ID
    uint64 start; //Segment start
    uint64 size; //Segment size
}__attribute__((packed));

struct ramdisk_file{
    uint64 magic;
    uint64 fstart;
    uint64 flen;
    uint64 name;
    uint64 ext;
    uint16 flags;
    uint64 fmaster; //Unused for now
    uint64 checksum;
}__attribute__((packed));

//Macros

#define SEGMENT_DATA_START(rdstart, sstart, segments) (rdstart+sstart+sizeof(struct ramdisk_header)+(sizeof(struct ramdisk_segment)*segments))
#define SEGMENT_POSITION(rdstart, segment) (rdstart+sizeof(struct ramdisk_header)+(sizeof(struct ramdisk_segment)*segment))
#define FILE_DATA(rdstart, nseg, segments, data)(rdstart+nseg+data+sizeof(struct ramdisk_header)+sizeof(struct ramdisk_segment)*segments)

//Flags

#define RDF_WRITE 1<<0
#define RDF_EXEC 1<<1
#define RDF_KERNEL 1<<2

#endif