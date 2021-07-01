#include "types.h"
#include "libmutex.h"

//For future multi-threading tasks

void get_lock(mutex *mtx){//Aquire mutex lock
    while(mtx->locked){/*Sleep/give up time slice*/}
    mtx->locked = true;
    return;
}

void free_lock(mutex *mtx){
    mtx->locked = false;
}

bool is_locked(mutex *mtx){
    return mtx->locked;
}

int get_mutex_owner(mutex *mtx){
    return mtx->owner_id;
}

void set_lock_owner(mutex *mtx, int trid){//Set mutex owner
    if(mtx->owner_id){return;} //Mutex already has an owner
    mtx->owner_id = trid;
}