#include "ramdisk.h"
#include "types.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Required files
FILE *ramdisk = NULL;
FILE *segments = NULL;
FILE *fheaders = NULL;
FILE *filedata = NULL;
FILE *nameexts = NULL;
//Currently opened file
FILE *current = NULL;

//Counters
uint64 segcount = 0;
uint64 fheader = 0;
uint64 fldata = 0;
uint64 nameext = 0;
uint64 fcount = 0;

uint64 FileLen(FILE *f){ //All other methods seem to be unreliable, I could not get consistent numbers
    uint64 i = 0;
    while(fgetc(f) != EOF){++i;}
    fseek(f, 0, SEEK_SET);
    return i;
}

void writeSegment(uint8 id, uint64 s, uint64 sz){
    struct ramdisk_segment segment;
    memset(&segment, 0x0, sizeof(struct ramdisk_segment));
    ++segcount;
    segment.magic = RDSEG_MAGIC;
    segment.id = id;
    segment.start = s;
    segment.size = sz;
    printf("[Segment - ID: 0x%hx - Start: 0x%lx - Size: 0x%lx]\n", id, s, sz);
    fwrite(&segment,sizeof(struct ramdisk_segment),1,segments);
}

void writeFileHeader(uint8 *n, uint8 *e, uint16 f, uint64 override){
    fprintf(nameexts, "%s%c%s%c", n, '\0', e, '\0');
    struct ramdisk_file fl;
    memset(&fl, 0x0, sizeof(struct ramdisk_file)); //Clean the structure
    if(!override){
        fl.fstart = fldata;
        fl.flen = FileLen(current);
        fldata += fl.flen;
    }
    else{
        fl.fstart = fldata;
        fldata += override;
        fl.flen = override;
    }
    fl.magic = RDF_MAGIC;
    fl.flags = f;
    fl.name = nameext;
    nameext += strlen(n)+1;
    fl.ext += nameext;
    nameext += strlen(e)+1;
    ++fheader;
    fwrite(&fl,sizeof(struct ramdisk_file),1,fheaders);
    ++fcount;
    printf("[%s.%s - 0x%lx - 0x%lx - 0x%lx - 0x%lx - 0x%x]\n", n, e, fl.fstart, fl.flen, fl.name, fl.ext, fl.flags);
    return;
}

void fappend(FILE *m, FILE *append){
    fseek(append, 0, SEEK_SET);
    char ch;
    while((ch = fgetc(append)) != EOF){
        fputc(ch, m);
    }
    fseek(append, 0, SEEK_SET);
}

void writeFile(){
    fappend(filedata, current);
}

void merge(){
    //Write segment headers
    uint64 segsz = fheader*sizeof(struct ramdisk_file);
    printf("Creating segment headers\n");
    writeSegment(RDSEG_HEADERS,0,segsz);
    writeSegment(RDSEG_DATA,segsz,fldata);
    segsz+=fldata;
    writeSegment(RDSEG_NEXT,segsz,nameext);
    segsz+=nameext;
    //Write ramdisk header
    struct ramdisk_header rd;
    rd.magic = RDMAGIC;
    rd.files = fcount;
    rd.size = segsz+sizeof(struct ramdisk_header);
    rd.segments = segcount;
    fwrite(&rd,sizeof(struct ramdisk_header),1,ramdisk);
    //Append all data to main ramdisk file
    printf("Appending segment headers\n");
    fappend(ramdisk,segments);
    printf("Appending file headers\n");
    fappend(ramdisk,fheaders);
    printf("Appending file data\n");
    fappend(ramdisk,filedata);
    printf("Appending name and exts\n");
    fappend(ramdisk,nameexts);
    printf("Cleaning up\n");
    if(segments){fclose(segments);}
    if(fheaders){fclose(fheaders);}
    if(filedata){fclose(filedata);}
    if(nameext){fclose(nameexts);}
    if(current){fclose(current);}
}

int main(){
    ramdisk = fopen("./ramdisk/disk.initrd", "w+");
    segments = fopen("./ramdisk/segment.tmp", "w+");
    fheaders = fopen("./ramdisk/fheader.tmp", "w+");
    nameexts = fopen("./ramdisk/nameext.tmp", "w+");
    filedata = fopen("./ramdisk/fldata.tmp", "w+");
    current = fopen("./ramdisk/test.png", "r");
    printf("Writing files\n");
    writeFile();
    writeFileHeader("test", "png", 0x0, 0);
    fclose(current);
    current = fopen("./ramdisk/kvargs.txt", "r");
    writeFile();
    writeFileHeader("kvargs", "txt", RDF_KERNEL, 0);
    merge();
    return 0;
}

/*
+-------------+--------+--------+-----+-------+
|Master header|Segments|FHeaders|Fdata|Strings|
+-------------+--------+--------+-----+-------+
*/