#include "kheaders.h"
#include "libui.h"
#include "libfont.h"

//With this addition 80x25 support is dead (might come back one day)

//Base functions

void plotPixel(vui *ui, uint32 x, uint32 y, uint32 col){
    uint32 pos = GET_PIXELPOS(x, y, kvinf.kui.pfui->xsz);
    uint32 *buff = (uint32*)ui->buffer;
    buff[pos] = col;
    buff[pos+1] = col;
    buff[pos+2] = col;
    ui->pxx = pos+2;
}

void plotLine(uint32 x, uint32 y, uint32 l, uint32 w, uint32 col, vui *ui){
    /*uint32 x2 = x+l;
    uint32 y2 = y+w;
    for(uint32 i = pos; i <= end; ++i){
        plotPixel(x, y, col, ui);
        if(x%kvinf.kui.xsz == 0){++y; x = 0;}
        ++x;
    }*/
}

void printBitmap(uint32 bmp, uint32 col, vui *ui){

}

//text Functions

void printLine(vui *ui){
    ui->lpy = ui->newlinePadding+ui->fontPXXSZ;
}

void printChar(vui *ui, char ch){
    uint32 off = (((uint32)ch)*8);
    uint32 x = ui->lpx;
    uint32 y = ui->lpy;
    bool bc = false;
    uint8 bmp = (uint8)StolenFont[off];
    for(uint32 i = 0; i <= ui->fontPXYSZ; ++i){
        for(uint32 b = 0; b <= ui->yScale; ++b){
            for(uint32 c = 0; c <= ui->fontPXXSZ; ++c){
                if(FONTBIT_SET(bmp, c)){plotPixel(ui, x, y, ui->CColor); bc = false;}else{if(ui->drawBackColor){plotPixel(ui, x, y, ui->backColor);} bc = true;}
                if(ui->scale && (ui->xScale != 0)){
                    for(uint32 d = 0; d <= ui->xScale; ++d){
                        if(bc){
                            if(ui->drawBackColor){plotPixel(ui, x, y, ui->backColor);}
                        }
                        else{
                            plotPixel(ui, x, y, ui->CColor);
                        }
                        ++x;
                    }
                }
                ++x;
            }
            if((ui->yScale != 0) && ui->scale){++y;}
            x = ui->lpx;
        }
        ++off;
        if(!((ui->yScale != 0) && ui->scale)){++y;}
        x = ui->lpx;
        bmp = (uint8)StolenFont[off];
    }
}

void printInt(vui *ui, uint32 i){

}

void printHex(vui *ui, int n){
    int tmp;
    char nZ = 1;
    int i;
    for(i = 28; i > 0; i -= 4){
        tmp = (n>>i)&0xF;
        if(tmp == 0 && nZ != 0){
            continue;
        }
        if(tmp >= 0xA){
            nZ = 0;
            printChar(ui, tmp-0xA+'a');
            ui->lpx += ui->textPadding;
        }
        else{
            nZ = 0;
            printChar(ui, tmp+'0');
            ui->lpx += ui->textPadding;
        }
    }
    tmp = n & 0xF;
    if(tmp >= 0xA){
        printChar(ui, tmp-0xA+'a');
        ui->lpx += ui->textPadding;
    }
    else{
        printChar(ui, tmp+'0');
        ui->lpx += ui->textPadding;
    }
}

void plotSquare(){

}

void clearArea(uint32 x, uint32 y, uint32 x1, uint32 y1){
    uint32 p1 = GET_PIXELPOS(x, y, kvinf.kui.pfui->xsz);
    uint32 p2 = GET_PIXELPOS(x, y, kvinf.kui.pfui->xsz);
    uint32 *buff = (uint32*)kvinf.kui.pfui->buffer;
    for(uint32 i = p1; i <= p2; ++i){
        buff[i] = kvinf.kui.pfui->backColor;
    }
}

/* Syntax Guide
// %f %i %u %d - int types
// %h - Char type
// %x %X - print int type as hex
// %c - Change Text Color (Deprecated)
// %y - Change back to original color (Deprecated)
// %b - Print bool as (true/false)
// %p - Place text at custom pos (Deprecated)
// %o - Go back to original pos (Deprecated)
// %s - Print string
*/

void printf(char *str){
    if(!kvinf.kui.pfui->draw){return;}
    char *c = str;
    uint32 i = 0;
    uint32 b = 0;
    uint32 *p;
    p = &str+1;
    uint32 l;
    while(c[i]){
        if(c[i] == '\0'){return;}
        if(c[i] != '%'){
            if(c[i] == '\n'){
                kvinf.kui.pfui->lpy += (kvinf.kui.pfui->newlinePadding+kvinf.kui.pfui->fontPXXSZ);
                kvinf.kui.pfui->lpx = kvinf.kui.pfui->xOffset;
            }
            else{
                printChar(kvinf.kui.pfui, c[i]);
                kvinf.kui.pfui->lpx += (kvinf.kui.pfui->textPadding);
            }
        }
        else{
            ++i;
            switch(c[i]){
                case 'f':
                case 'i':
                case 'u':
                case 'd':
                //printInt()
                break;
                case 'h':
                break;
                case 'x':
                printf("0x");
                case 'X':
                printHex(kvinf.kui.pfui, p[b]);
                break;
                case 'b':
                break;
                case 's':
                printf(p[b]);
                break;
                case 'c':
                printChar(kvinf.kui.pfui, p[b]);
                kvinf.kui.pfui->lpx += kvinf.kui.pfui->textPadding;
                break;
            }
            ++b;
        }
        ++i;
    }
}

//Fancy functions

uint32 rgb(uint32 r, uint32 g, uint32 b, uint32 al){

}