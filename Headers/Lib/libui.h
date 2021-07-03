#ifndef LIBUI_H
#define LIBUI_H

#include "types.h"

#define VBE_INDEXED 0
#define VBE_RGB 1
#define VBE_EGA 2

//Basic Colors
#define BLACK 0x0
#define WHITE 0xFFFFFF
#define GREY 0x9ba8a7
#define RED 0xff0000
#define GREEN 0x00e32d
#define BLUE 0x0055ff
#define ORANGE 0xff8229
#define VIOLET 0xee82ee
#define PURPLE 0xb02aa3
#define AQUA 0x03fce8
#define YELLOW 0xfcec03
#define DARK_BLUE 0x00008B

#define VCHAR(a,b,f) (a|(((b<<4)|(f&0x0F))<<8))
#define GET_PIXELPOS(x, y, cx) (x+(y*cx))
#define FONTBIT_SET(b, o) (b&(0x80>>o))

#define GUI_OBJECT 0x84
#define GUI_TEXT 0x85
#define GUI_BUTTON 0x86

//Every task with a UI must have a gui to render
struct gui{
    uint64 ownerid; //For when multitasking is avalible
    uint64 *objects; //Pointer to array of pointers for objects
};

//For basic UI elements
struct guiObject{
    uint8 objType;
    uint32 Xres;
    uint32 Yres;
    uint32 Xpos;
    uint32 Ypos;
    uint32 *buffer;
};

//For writing text to the screen
struct guiText{
    struct guiObject objectData;
    uint32 textStart;
    uint32 textEnd;
    uint32 charSpacing;
    uint32 textXPadding;
    uint32 textYPadding;
    uint64 *font;
    uint8 *text;
};

//For drawing buttons on the screen and allowing the user to click them
struct guiButton{
    struct guiObject objectData;

};

extern void plotPixel(struct guiObject *ui, uint32 x, uint32 y, uint32 color);
extern void fill(struct guiObject *ui, uint32 color);

#endif