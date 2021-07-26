#include "libstd.h"
#include "libui.h"
#include "kvinf.h"
#include "basefont.h"
#include "libmath.h"
#include "libtask.h"
#include "libarg.h"

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

void initGui(struct gui *ui, uint32 xres, uint32 yres, uint32 xpos, uint32 ypos, uint32 *buffer){
    ui->Xres = xres;
    ui->Yres = yres;
    ui->Xpos = xpos;
    ui->Ypos = ypos;
    ui->buffer = buffer;
}

void plotPixel(uint32 x, uint32 y, uint32 color){
    struct task *t = currentTask();
    struct gui *ui = t->taskUI;
    uint32 pos = GET_PIXELPOS(Clamp(x, ui->Xpos, ui->Xres-1), Clamp(y, ui->Ypos, ui->Yres-1), ui->Xres);
    ui->buffer[pos] = color;
}

void fill(uint32 color){
    struct task *t = currentTask();
    struct gui *ui = t->taskUI;
    uint64 end = GET_PIXELPOS(ui->Xres, ui->Yres-1, ui->Xres);
    for(uint64 i = 0; i < end; ++i){
        ui->buffer[i] = color;
    }
    return;
}

void scroll(struct gui *ui, struct text *txt){
    //Try my best to scroll based on information avalible
}

void plotChar(char ch, uint32 x, uint32 y, uint32 color, struct text *txt){
    struct task *t = currentTask();
    struct gui *ui = t->taskUI;
    uint32 *buffer = ui->buffer;
    uint8 *font;
    if(txt->flags&TXT_FONT){font = txt->font;}else{font = &baseFont;}\
    font+=ch*8;
    uint32 xp = txt->Xpos+x;
    uint32 yp = txt->Ypos+y;
    uint16 exit = 0;
    uint32 shift = 0;
    uint32 yoff = 0;
    uint32 xoff = 0;
    uint32 yinc = 0;
    uint32 i = 0;
    uint8 r = font[0];
    while(true){
        if(exit == 8){
            return;
        }
        do{
            while(shift <= 7){
                if(r&0x80>>shift){
                    do{
                        plotPixel(xp+i+xoff,yp+yoff,color);
                        ++i;
                    }while(i < txt->Xscale);
                }
                else{
                    xoff+=txt->Xscale+1;
                }
                xoff+=i;
                i = 0;
                ++shift;
            }
            ++yinc;
            shift = 0;
            i = 0;
            xoff = 0;
            ++yoff;
        }while(yinc < txt->Yscale);
        yinc = 0;
        r = font[++exit];
    }
}

void phex_int(int t, bool caps, const uintptr *v,struct text *txt){
    uint64 n = (uint64)v;
    int comp = GET_SZBYTES(t);
    int p = 0;
    int a;
    bool leading = true;
    bool c = false;
    for(; comp >= 0; --comp){
        c = false;
        a = 0x0;
        p = (n>>(4*comp)&0xF);
        if(p >= 10){
            leading = false;
            c = true;
            a = p-0xA+'a';
        }
        else{
            if(p > 0){leading = false;}
            c = false;
            a = p+'0';
        }
        if(!leading || (!leading && a != '0')){plotChar((char)a,txt->lcpX,txt->lcpY,txt->fcolor,txt); txt->lcpX+=txt->padding;}
    }
    //Nothing was printed
    if(leading){plotChar('0',txt->lcpX,txt->lcpY,txt->fcolor,txt); txt->lcpX+=txt->padding;}
}

void pwrite(const char *s, struct text *txt){
    for(uint64 i = 0; s[i] != '\0'; ++i){
        plotChar(s[i],txt->lcpX,txt->lcpY,txt->fcolor,txt);
    }
}

void printf(const char *format, ...){
    struct task *t = currentTask();
    struct text *txt = t->stdio;
    if(txt == NULL){return;}
    va_list args;
    va_start(args, format);
    uint16 cast_type = CAST_NORMAL;
    bool loop = false;
    bool caps = false;
    uint32 presX = txt->lcpX;
    for(uint64 ch = 0; format[ch] != '\0';){
        if(format[ch] == '%' || loop == true){
            if(!loop){++ch;}
            loop = false;
            switch(format[ch]){
                case 'X':
                caps = true;
                case 'x':
                phex_int(cast_type, caps, va_arg(args, uintptr*),txt);
                caps = false;
                break;
                case 'l':
                if(format[ch+1] == 'l'){
                    loop = true;
                    cast_type = CAST_LLONG;
                    ++ch;
                    break;
                }
                cast_type = CAST_LONG;
                loop = true;
                break;
                case 'c':
                plotChar(va_arg(args, char),txt->lcpX,txt->lcpY,txt->fcolor,txt);
                txt->lcpX+=txt->padding;
                break;
                case 's':
                pwrite(va_arg(args, char*),txt);
                break;
                case 'i':
                //qint_placeholder(va_arg(args, char*), COM1);
                break;
                case 'h':
                cast_type = CAST_CHAR;
                loop = true;
                break;
                default:
                break;
            }
            ++ch;
        }
        else{
            if(format[ch] == '\n'){
                txt->lcpY+=txt->nlPadding;
                txt->lcpX = presX;
            }
            else{
                plotChar(format[ch],txt->lcpX,txt->lcpY,txt->fcolor,txt);
                txt->lcpX+=txt->padding;
            }
            ++ch;
        }
    }
}