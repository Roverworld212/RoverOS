#include "libcom.h"
#include "libstd.h"
#include "libio.h"
#include "libmem.h"
#include "paging.h"
#include "memory.h"
#include "pmalloc.h"
#include "types.h"

//This code is 100% terrible

struct kblock fblock;
uint64 blocks; //Blocks present

void *pmalloc(uint32 size, uint16 flags){
    printq("%qAlloc request size 0x%lx flags 0x%lx\n", QINFO, size, (uint32)flags);
    //Space actually needed
    uint64 truesz = size+sizeof(struct kblock);
    struct kblock *block = &fblock;
    uint64 *pblock;
    for(uint64 i = 0; i < blocks; ++i){
        //Add a new block
        if(block->next == NULL){
            printq("%qAdding new block\n", QINFO);
            pblock = (uint64*)block;
            block->next = (uint64*)block->memory+block->size;
            block = (struct kblock*)block->next;
            block->flags = flags&KBF_INUSE;
            block->size = size;
            block->memory = (uint64*)block+sizeof(struct kblock);
            block->previous = pblock;
            block->next = (uint64*)NULL;
            return (void*)block->memory;
        }
        //Use a free block that matches the size
        if(!(block->flags&KBF_INUSE) && block->size >= size){
            block->flags = flags&KBF_INUSE;
            if(block->size > size){
                block->flags &= KBF_SPARE;
            }
            return (void*)block->memory;
        }
        //Spilt a block
        if(block->flags&KBF_SPARE){
            printq("%qSpare memory\n", QINFO);
        }
    }
}

void pfree(void *memory){

}

//Initilize pmalloc and stuff
void pmallocInit(uint64 mstart){
    printq("%qInitilizing pmalloc\n", QINFO);
    printq("%qMemory spent per alloc 0x%llx\n", QINFO, (uint64)sizeof(struct kblock));
    printq("%qAllocating at 0x%llx\n", QINFO, mstart);
    fblock.flags = KBF_INUSE;
    fblock.size = 0;
    fblock.memory = (uint64*)mstart;
    fblock.next = (uint64*)NULL;
    fblock.previous = (uint64*)NULL;
    blocks = 1;
    printq("%qSetup starting block\n", QINFO);
    printq("%qFinished initilizing pmalloc\n", QINFO);
}