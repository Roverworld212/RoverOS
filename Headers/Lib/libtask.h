#ifndef LIBTASK_H
#define LIBTASK_H

#include "types.h"
#include "libui.h"
#include "libhook.h"

struct task{
    struct gui *taskUI; //UI for the task
    struct text *stdio; //Where to draw stdio text
    uint16 maxTicks; //Max ticks before task switch
    uint16 currentTick; //Current tick
    struct hook *taskHooks;
};

extern struct task *currentTask();
extern void swapTask(struct task *t);
extern void createTask(struct gui *ui, struct text *stdio, struct task *t, uint16 maxTicks, uint64 *hooks);

#endif