#include "libstd.h"
#include "libio.h"
#include "libui.h"
#include "libhook.h"
#include "libtask.h"
#include "kvinf.h"
#include "basefont.h"

struct gui ui;
struct text txt;
struct task *kshellTask;
struct hook shellHooks[20];

hookfunc *charhook(struct hookData *data){
    printq("Got 0x%llx\n", data->data);
}

void initKshell(){
    printq("%qLoading kshell\n", QINFO);
    ui.buffer = kvinf.video.physical;
    ui.flags = 0x0;
    ui.objects = NULL;
    ui.Xres = kvinf.video.width;
    ui.Yres = kvinf.video.height;
    ui.Xpos = 0;
    ui.Ypos = 0;
    //Txt gui
    txt.Xpos = 5;
    txt.Ypos = 5;
    txt.flags = TXT_FONT;
    txt.Xres = 200;
    txt.Yres = 100;
    txt.Xscale = 0;
    txt.Yscale = 0;
    txt.fcolor = VIOLET;
    txt.padding = 20;
    txt.font = &baseFont;
    createTask(&ui, &txt, &kshellTask, 10, &shellHooks);
    swapTask(&kshellTask);
    printf("!##!00");
    printf("1122!");
    addHook(KEYBOARD_HOOK,0,0x0,charhook);
}