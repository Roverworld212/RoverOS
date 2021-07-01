#ifndef LIBFS_H
#define LIBFS_H

#include "types.h"

#define FILE_STRUCTURE_MAGIC 0xABAF
#define STRUCTURE_OFFSET_TABLE 0xFCFC
#define STRUCTURE_FILE 0x88
#define STRUCTURE_DIRECTORY 0x8F
#define STRUCTURE_TERMINATOR 0xADAD

#define FSN 0x8000

#define FILE_READ FSN>>0
#define FILE_WRITE FSN>>1
#define FILE_EXECUTE FSN>>2
#define FILE_SU FSN>>3
#define FILE_SYSTEM FSN>>4

struct fcheckS{
    uint16 magic; //Magic value
    uint16 type; //Structure type
}__attribute__((packed));

typedef struct fcheckS fcheck;

struct ramdiskOffsetTableS{
    fcheck file_check;
    uint64 headers_start; //Offset from start to dir/file structures
    uint64 content_start; //Offset from start to file content
    uint64 strings_start; //Offset from start to names and extensions
}__attribute__((packed));

typedef struct ramdiskOffsetTableS ramdiskOffsetTable;

struct ramdiskTerminatingHeaderS{
    fcheck file_check;
}__attribute__((packed));

typedef struct ramdiskTerminatingHeaderS ramdiskTerminatingHeader;

struct fileS{
    fcheck file_check; //fcheck structure
    uint64 checksum; //Checksum of file content
    uint64 ext_len; //Length of ext
    uint64 name_len; //Length of name
    uint64 ext; //File extension pointer
    uint64 name; //File name pointer
    uint64 content; //Content pointer
    uint64 parent; //Parent structure pointer
    uint64 size; //File size
    uint16 flags; //File flags
}__attribute__((packed));

struct directoryS{
    fcheck file_check; //fcheck structure
    uint64 content; //Pointer to list of pointers
    uint64 parent; //Parent structure pointer
    uint64 size; //Directory size
    uint16 flags; //File flags
}__attribute__((packed));


typedef struct fileS file;

typedef struct directoryS directory;

#endif