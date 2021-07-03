#include "libstd.h"
#include "libui.h"
#include "kvinf.h"

void initObject(uintptr *obj, uint8 type){
    switch(type){
        case GUI_OBJECT:
        break;
        case GUI_TEXT:
        break;
        case GUI_BUTTON:
        break;
    }
}

void plotPixel(struct guiObject *ui, uint32 x, uint32 y, uint32 color){
    if(ui == NULL){return;}
    uint32 pos = GET_PIXELPOS(x, y, ui->Xres);
    ui->buffer[pos] = color;
}

void fill(struct guiObject *ui, uint32 color){
    if(ui == NULL){return;}
    uint32 end = GET_PIXELPOS(ui->Xres, ui->Yres, ui->Xres);
    for(uint32 i = 0; i <= end; ++i){
        ui->buffer[i] = color;
    }
}