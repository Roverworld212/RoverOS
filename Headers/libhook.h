#ifndef LIBHOOK_H
#define LIBHOOK_H

#include "types.h"
#include "libargs.h"

#define AUTO -1

#define GK_HOOK 0x1//For qwertyuiop type keys
#define SPACE_HOOK 0x39
#define LSHIFT_HOOK 0x2a
#define LSHIFTR_HOOK 101
#define RSHIFT_HOOK 0xaa
#define RSHIFTR_HOOK 102
#define LCTRL_HOOK 0x1d
#define RCTRL_HOOK 0x38
#define RCTRLR_HOOK 0xb8
#define TAB_HOOK 0x8f
#define CAPS_HOOK 0xba
#define CAPSR_HOOK 0x3a
#define ESC_HOOK 8
#define BACKSPACE_HOOK 0xe
#define BACKSPACER_HOOK 0x8e
#define FN_HOOK 105
#define WIN_HOOK 104
#define LALT_HOOK 103
#define RALT_HOOK 13
#define ENTER_HOOK 0x1c
#define ENTERR_HOOK 0x9c
#define UPA_HOOK 0x48
#define DOWNA_HOOK 0x50
#define LEFTA_HOOK 0x4B
#define RIGHTA_HOOK 0x4D
//Other system type hooks
#define END_HOOK 0x2 //Will not be called by its own end
#define PIT_HOOK 0x4 //Called every 300 ticks
#define SUDO_HOOK 0x5
#define MOUSE_HOOK 0x8
#define ACCESSGROUP_HOOK 0x9

extern void hook_int(uint32 type, uint8 ftype, int s, hook fc, uint8 data);
#define addHook(ftype, s, fc) hook_int(0, ftype, s, fc, NULL)
#define removeHook(s) hook_int(1, NULL, s, NULL, NULL)
#define runHook(ftype, data) hook_int(2, ftype, NULL, NULL, data)
#define clearHook(ftype) hook_int(3, ftype, NULL, NULL, NULL)

extern void command_int(uint32 type, char *cmd, argData *args, command cmdfc, char *ah, uint32 acgrp, uint32 acright);
#define runCommand(cmd, args) command_int(4, cmd, args, NULL, NULL, NULL, NULL)

#endif