#include "ramdisk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 parsed = 0;

void error(char *str, uint32 l, uint32 c, FILE *f, FILE *r){
    printf("%s - Line: %li Char: %li\n", str, l, c);
    fclose(f);
    fclose(r);
    exit(EXIT_FAILURE);
}

void preparse(FILE *f, FILE *r){ //Pre-check kvargs before compiling the kernel/ramdisk
    char ch;
    uint32 comOpen = 0;
    bool comment = false;
    bool args = false;
    uint32 line = 1;
    uint32 chl = 0;
    uint32 subComm = 0;
    uint32 end;
    bool noParse = false;
    while((ch = fgetc(f)) != EOF){
        ++chl;
        ++parsed;
        switch(ch){
            case '\n':
            chl = 0;
            if(!comment){fputc(ch, r);}
            ++line;
            break;
            case ';':
            if(comment){break;}
            if(args){error("Closed statment before closing args", line, chl, f, r);}
            fputc(ch, r);
            break;
            case '(':
            if(comment){break;}
            if(args){error("Opened new args before closing", line, chl, f, r);}
            fputc(ch, r);
            args = true;
            break;
            case ')':
            if(comment){break;}
            if(!args){error("Closed non open args", line, chl, f, r);}
            fputc(ch, r);
            args = false;
            break;
            case '<':
            if(!comment){error("No parse statment outside of comment", line, chl, f, r);}
            noParse = true;
            break;
            case '>':
            if(!noParse){error("Closed non-existant no parse statment", line, chl, f, r);}
            noParse = false;
            break;
            case '{':
            if(comment){++subComm; break;}
            if(!comment){comOpen = line;}
            comment = true;
            break;
            case '}':
            if(!comment){error("Closed non-existant comment", line, chl, f, r);}
            if(subComm == 0){comOpen = 0; if(noParse){error("Closed comment before closing no parse statment", line, chl, f, r);} comment = false;}else{--subComm;}
            break;
            case '#':
            if(noParse){break;}
            if(comment){error("Reached terminator before closing comment", line, chl, f, r);}
            fputc(ch, r);
            return;
            break;
            case ' ':
            break;
            default:
            if(!comment){
                fputc(ch, r);
            }
            break;
        }
    }
    if(!end){error("No closing terminator", line, chl, f, r);}
}

int main(){
    FILE *ram;
    FILE *k;
    FILE *f;
    struct masterHeader master;
    struct ramHeader header;
    ram = fopen("./ramdisk/disk.initrd", "w+");
    k = fopen("./ramdisk/Files/kvargs.txt", "r");
    if(k == NULL){printf("kvargs.txt file missing\n"); exit(EXIT_FAILURE);}
    if(ram == NULL){printf("Failed to open ramdisk\n"); exit(EXIT_FAILURE);}
    master.magic = RDMASTERMAGIC;
    master.files = 1;
    fwrite(&master, 1, sizeof(struct masterHeader), ram);
    header.magic = RDMAGIC;
    header.len = parsed;
    header.name[0] = '\0';
    header.off = sizeof(struct ramHeader);
    fwrite(&header, 1, sizeof(struct ramHeader), ram);
    preparse(k, ram);
    //Insert loop for other files here
    fclose(ram);
    fclose(k);
    return 0;
}