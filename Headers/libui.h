#ifndef LIBUI_H
#define LIBUI_H

#include "types.h"
#include "keyboard.h"

//Types
#define VBE_INDEXED 0
#define VBE_RGB 1
#define VBE_EGA 2

//Basic Colors
#define BLACK 0x0
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

extern void plotPixel(vui *ui, uint32 x, uint32 y, uint32 col);
extern void printChar(vui *ui, char ch);

#endif