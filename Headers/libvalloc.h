#ifndef LIBVALLOC_H
#define LIBVALLOC_H

#include "types.h"

#define CLAYER 0x0 //Cursor layer (reserved for only the cursor)
#define KLAYER 0x1 //Kernel UI
#define RLAYER 0x2 //Reserved for future use
#define RESMAX 10

#define NEW_CHILD 0
#define SELF_MODE 1
#define CHILD_MAX 9

#define GET_BUFFERPOS(x, y, xsz) ((y*xsz)+x)

typedef struct vuiS{
    //Internal Use
    uint32 xsz; //XRes
    uint32 ysz; //YRes
    uint32 xp; //XWinPos
    uint32 yp; //YWinPos
    uint32 lpx; //Char X
    uint32 lpy; //Char Y
    uint32 pxx; //Last pixel written to
    uint32 layer; //Screen Layer
    uint32 buffer; //Virtual Buffer
    uint32 *parent; //Parent VUI
    uint32 *children[CHILD_MAX]; //Children vui
    //GPV
    bool draw; //Should Draw
    bool vsync; //Enable V-Sync
    bool scale; //Text scaling
    bool drawBackColor; //Draw backColor in chars
    uint32 buffMode; //Buffering mode
    //Fancy Functions
    bool autoNL; //Auto Newline
    //Font and Text
    uint32 CColor; //Font Text Color
    uint32 backColor; //Empty space color
    uint32 newlinePadding; //Padding between text lines
    uint32 xOffset; //X Char Offset
    uint32 textPadding; //Padding between chars
    uint32 xScale; //X Font scale
    uint32 yScale; //Y Font scale
    uint32 fontPXXSZ; //Font X Pixel size
    uint32 fontPXYSZ; //Font Y Pixel size
    bool autoTop;
    bool autoNl;
    uint32 *font;
}vui;

#endif