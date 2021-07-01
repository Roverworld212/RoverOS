#ifndef LIBUI_H
#define LIBUI_H

#include "types.h"

struct masterMonitorS{ //Only 1 of these should exist at any time
    uint64 *monitors; //Pointer to array of monitor structures
}masterMonitor;

struct monitorS{ //For multiple monitors
    uint64 id; //Unique ID
    uint32 xs; //X-Res
    uint32 ys; //Y-Res
    uint32 bpp; //Bits per pixel
    bool sleep; //Is monitor sleeping?
    bool vsync; //Is V-Sync enabled?
    uint64 *buffer; //Pointer to VESA buffer
    uint64 *ui; //Pointer to array of gui structures
}monitor;

struct guiS{ //Application GUI
    uint64 monitor; //Parent monitor (pointer if split is true)
    uint64 xsc; //X-Constraint
    uint64 ysc; //Y-Constraint
    uint64 xs; //X-Res
    uint64 ys; //Y-Res
    bool delegate; //Should delagate drawing to parent?
    bool resizable; //Is gui resizable?
    bool split; //Is gui split across multiple monitors?
    bool vsync; //Is V-Sync enabled?
    bool draw; //Should draw gui?
    uint16 bufferMode; //Buffering mode
    uint64 *buffer; //Pointer to allocated buffer
}gui;

#endif