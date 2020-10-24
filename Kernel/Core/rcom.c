#include "kheaders.h"
#include "rcom.h"
#include "setup.h"

//Basic utilites

void sendData(uint32 c, uint8 d){
    if(c == COM1 && !kvinf.kio.log){return;}
    outb(c, d);
}

bool canSend(uint32 c){
    return inb(c+5)&0x20;
}

int serialIn(uint32 c){
    return inb((c+5)&1);
}

char readData(uint32 c){
    while(!serialIn(c)){}
    return (char)inb(c);
}

void startCom(uint32 c){
    outb(c+1, 0x0); //cli
    outb(c+3, 0x80); //DLAB
    outb(c, 0x3); //divisor
    outb(c+1, 0x0); //Hi-bit
    outb(c+3, 0x3); //8 bits
    outb(c+2, 0xC7); //FIFO
    outb(c+4, 0x0B); //sti
}

//Kernel comms

irh comRecv(regs *r){
    printq("%iGot com recv\n", __F__);
}

void setupComms(){
    printq("%ILoading Kernel comms %x\n", __F__, COM3);
    addHandler(IRQ4, &comRecv);
    startCom(COM3);
}

bool sendKReq(){
    return true;
}

void sendResp(){

}