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
#define GET_PIXELPOS(x, y, cx) (x+(cx*y))
#define FONTBIT_SET(b, o) (b&(0x80>>o))

#define GUI_OBJECT 0x84
#define GUI_TEXT 0x85
#define GUI_BUTTON 0x86

struct gui{
    //Window size
    uint32 Xres;
    uint32 Yres;
    //Window position
    uint32 Xpos;
    uint32 Ypos;
    //GUI flags
    uint16 flags;
    //UI buffer
    uint32 *buffer;
    //Pointer to array of objects
    uint64 *objects;
}__attribute__((packed));

struct text{
    //Text box size
    uint32 Xres;
    uint32 Yres;
    //Relative pos
    uint32 Xpos;
    uint32 Ypos;
    //Scaling
    uint32 Xscale;
    uint32 Yscale;
    //Formatting
    uint32 padding; //Spacing between chars
    uint32 nlPadding; //Spacing between new lines
    //stdio/printf
    uint32 lcpX;
    uint32 lcpY;
    uint32 fcolor;
    uint32 bcolor;
    //Text flags
    uint16 flags;
    //Font
    uint8 *font;
}__attribute__((packed));

struct image{
    //Relative pos
    uint32 Xpos;
    uint32 Ypos;
    //Pointer to image
    uint64 *img;
};

//Font present
#define TXT_FONT 1<<0

extern void plotPixel(uint32 x, uint32 y, uint32 color);
extern void fill(uint32 color);
extern void initGui(struct gui *ui, uint32 xres, uint32 yres, uint32 xpos, uint32 ypos, uint32 *buffer);
extern void plotChar(char ch, uint32 x, uint32 y, uint32 color, struct text *txt);
extern void printf(const char *format, ...);

#endif