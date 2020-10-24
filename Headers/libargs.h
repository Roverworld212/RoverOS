#ifndef LIBARGS_H
#define LIBARGS_H

//Hook and argument structs

#include "types.h"

typedef (*hook)(uint8);
typedef(*command)(char*);

typedef struct hookS{
    uint8 type;
    bool isUsed;
    hook func;
}hookData;

typedef struct commandS{
    char *cmd;
    char *man;
    command func;
    bool isUsed;
}commandData;

typedef struct argS{
    char arg[59];
    uint32 cursor;
    bool end;
}argData;

#endif