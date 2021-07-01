#include "libstd.h"
#include "libcom.h"
#include "libarg.h"

void start_com(uint64 com){
    outb(com+1, 0x0); //cli
    outb(com+3, 0x80); //dlab
    outb(com, 0x3); //divisor
    outb(com+1, 0x0); //High bits
    outb(com+3, 0x3); //8 bits
    outb(com+2, 0xC7); //fifo
    outb(com+4, 0x0B); //sti
}

bool empty(uint64 com){
    return (bool)inb(com+5)&0x20;
}

bool send(uint8 data, uint64 com, bool caps){
    while(!empty(com)){}
    if(caps){data = data&0xDF;}
    outb(com, data);
    return true;
}

uint64 read(uint64 com){
    while(!empty(com)){}
    return (uint64)inb(com);
}

void qwrite(const char *s){
    for(uint64 i = 0; s[i] != '\0'; ++i){
        send((uint8)s[i], COM1, false);
    }
}

void qint_int(int t, const uintptr *v){
    char str[500]; //Fixed sized for now
    switch(t){
        case CAST_INT:
        case CAST_NORMAL:
        itoa((uint8)v, str, 10);
        qwrite(str);
        break;
    }
}

void qhex_int(int t, bool caps, const uintptr *v){
    uint64 n = (uint64)v;
    int comp = GET_SZBYTES(t);
    int p;
    for(; comp >= 0; --comp){
        p = (n>>(4*comp)&0xF);
        if(p >= 10){
            send(p-0xA+'a', COM1, caps);
        }
        else{
            send(p+'0', COM1, false);
        }
    }
}

void printq(const char *format, ...){
    va_list args;
    va_start(args, format);
    uint16 cast_type = CAST_NORMAL;
    bool loop = false;
    bool caps = false;
    for(uint64 ch = 0; format[ch] != '\0';){
        if(format[ch] == '%' || loop == true){
            if(!loop){++ch;}
            loop = false;
            switch(format[ch]){
                case 'X':
                caps = true;
                case 'x':
                qhex_int(cast_type, caps, va_arg(args, uintptr*));
                caps = false;
                break;
                case 'l':
                if(format[ch+1] == 'l'){
                    loop = true;
                    cast_type = CAST_LLONG;
                    ++ch;
                    break;
                }
                cast_type = CAST_LONG;
                loop = true;
                break;
                case 'c':
                send((uint8)va_arg(args, int), COM1, caps);
                break;
                case 's':
                qwrite(va_arg(args, char*));
                default:
                case 'i':
                break;
                case 'h':
                cast_type = CAST_CHAR;
                loop = true;
                break;
                case 'q':
                qwrite(va_arg(args, char*));
                qwrite("[");
                send((uint8)va_arg(args, int), COM1, caps);
                qwrite("]: ");
                break;
            }
            ++ch;
        }
        else{
            send(format[ch], COM1, caps);
            ++ch;
        }
    }
}