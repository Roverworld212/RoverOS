#ifndef LIBUI_H
#define LIBUI_H

#include "types.h"

//For basic UI elements
struct guiObjectS{
    uint8 objType;
    uint32 Xres;
    uint32 Yres;
    uint32 Xpos;
    uint32 Ypos;
    uint32 length;
    uint32 width;
    uint64 *buffer;
};

struct guiText{
    struct guiObjectS objectData;
    uint32 textStart;
    uint32 textEnd;
    uint32 charSpacing;
    uint32 textXPadding;
    uint32 textYPadding;
    uint64 *font;
};

struct guiButton{
    struct guiObjectS objectData;

};

#endif