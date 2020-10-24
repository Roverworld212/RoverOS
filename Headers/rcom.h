#ifndef RCOM_H
#define RCOM_H

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

typedef struct rpckS{
    bool h : 1;
}rpck __attribute__((packed));

#endif