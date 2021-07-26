#include "libstd.h"
#include "libio.h"
#include "libksystem.h"
#include "libcom.h"
#include "libtask.h"
#include "libui.h"
#include "libhook.h"
#include "kvinf.h"

struct task *currentTask(){
    return kvinf.task.currentTask;
}

void swapTask(struct task *t){
    printq("%qSwitching task\n", QINFO);
    kvinf.task.currentTask = t;
}

void createTask(struct gui *ui, struct text *stdio, struct task *t, uint16 maxTicks, uint64 *hooks){
    printq("%qCreating task structure\n", QINFO);
    t->taskUI = ui;
    t->maxTicks = maxTicks;
    t->currentTick = 0;
    t->stdio = stdio;
    t->taskHooks = (struct hook*)hooks;
    printq("%qui: 0x%llx mxt: 0x%x hooks: 0x%llx\n", QINFO, (uint64)ui, maxTicks, (uint64)hooks);
    printq("%qCreated task\n", QINFO);
}