#ifndef LIBMUTEX_H
#define LIBMUTEX_H

#include "types.h"

typedef struct mutexS{
    bool locked; //Is locked?
    int owner_id; //Owner pid/thread id
}mutex;

extern void get_lock(mutex *mtx);
extern void free_lock(mutex *mtx);
extern void set_lock_owner(mutex *mtx, int trid);

#endif