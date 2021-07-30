#include "libstd.h"
#include "libui.h"
#include "kvinf.h"
#include "basefont.h"
#include "libmath.h"
#include "libtask.h"
#include "libarg.h"

void createGUI(struct gui *ui, uint32 xr, uint32 yr, uint32 xp, uint32 yp, uint16 f, uint64 *objects){
    ui->Xres = xr;
    ui->Yres = yr;
    ui->Xpos = xp;
    ui->Ypos = yp;
    ui->flags = f;
    ui->objects = objects;
    ui->buffer = (uint32*)kvinf.video.physical+GET_PIXELPOS(xp,yp,kvinf.video.width);
}

void createText(struct text *txt, uint32 xr, uint32 yr, uint32 xp, uint32 yp, uint32 xs, uint32 ys, uint32 pd, uint32 nlpd, uint32 lcpX, uint32 lcpY, uint32 fc, uint32 bc, uint16 f, uint8 *font){
    txt->Xres = xr;
    txt->Yres = yr;
    txt->Xpos = xp;
    txt->Ypos = yp;
    txt->Xscale = xs;
    txt->Yscale = ys;
    txt->padding = pd;
    txt->nlPadding = nlpd;
    txt->lcpX = lcpX;
    txt->lcpY = lcpY;
    txt->fcolor = fc;
    txt->bcolor = bc;
    txt->flags = f;
    if(font == NULL){txt->font = &baseFont;}else{txt->font = font;}
}

void createObject(struct object *obj, uint32 xp, uint32 yp, uint32 xr, uint32 yr, uint16 f, uint32 sz, uint8 *draw){
    obj->Xpos = xp;
    obj->Ypos = yp;
    obj->Xres = xr;
    obj->Yres = yr;
    obj->flags = f;
    obj->size = sz;
    obj->draw = draw;
}

void initGui(struct gui *ui, uint32 xres, uint32 yres, uint32 xpos, uint32 ypos, uint32 *buffer){
    ui->Xres = xres;
    ui->Yres = yres;
    ui->Xpos = xpos;
    ui->Ypos = ypos;
    ui->buffer = buffer;
}

void plotPixel(struct gui *ui, uint32 x, uint32 y, uint32 color){
    uint32 pos = GET_PIXELPOS(Clamp(x, ui->Xpos, ui->Xres-1), Clamp(y, ui->Ypos, ui->Yres-1), ui->Xres);
    ui->buffer[pos] = color;
}

void fill(struct gui *ui, uint32 color){
    for(uint32 i = 0; i < (ui->Xres*(kvinf.video.bpp/8))*ui->Yres; ++i){
        ui->buffer[i] = color;
    }
    return;
}

void drawUIObject(struct object *obj){
    //Only bit drawing is supported
    struct task *t = currentTask();
    uint32 xp = obj->Xpos+t->taskUI->Xpos;
    uint32 yp = obj->Ypos+t->taskUI->Ypos;
    uint32 xoff = 0;
    uint32 yoff = 0;
    uint32 fcolor = obj->draw[0];
    uint32 bcolor = obj->draw[1];
    if(!(obj->flags&OBJ_BIT_DRAW)){return;}
    for(uint32 i = 0; i < obj->size; ++i){
        for(uint32 c = 0; c < 8 && obj->draw[i+2] != NULL; ++c){
            if(obj->draw[i+2]&0x80>>i){plotPixel(t->taskUI, xp+xoff,yp+yoff,fcolor);}else{plotPixel(t->taskUI,+xoff,yp+yoff,bcolor);}
            ++xoff;
            if(xoff > obj->Xres){++yoff; xoff = 0;}
        }
    }
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
                        plotPixel(ui, xp+i+xoff,yp+yoff,color);
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
    struct text *txt = (currentTask()->stdio);
    if(txt == NULL){return;}
    va_list args;
    va_start(args, format);
    uint16 cast = CAST_NORMAL;
    bool loop = false;
    bool caps = true;
    uint32 presX = txt->lcpX;
    uint8 u = NULL;
    for(uint64 ch = 0; format[ch] != '\0';){
        if(format[ch] == '%' || loop == true){
            if(!loop){++ch;}
            loop = false;
            switch(format[ch]){
                case 'X':
                caps = true;
                phex_int(cast,caps,va_arg(args,uintptr*),txt);
                break;
                case 'x':
                caps = false;
                phex_int(cast,caps,va_arg(args,uintptr*),txt);
                break;
                case 'l':
                if(format[ch+1] == 'l'){
                    loop = true;
                    cast = CAST_LLONG;
                    ++ch;
                    break;
                }
                cast = CAST_LONG;
                loop = true;
                break;
                case 'c':
                u = (uint8)va_arg(args,int);
                plotChar(u,txt->lcpX,txt->lcpY,txt->fcolor,txt);
                txt->lcpX+=txt->padding;
                break;
                case 's':
                pwrite(va_arg(args, char*),txt);
                break;
                case 'i':
                //qint_placeholder(va_arg(args, char*), COM1);
                break;
                case 'h':
                cast = CAST_CHAR;
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
