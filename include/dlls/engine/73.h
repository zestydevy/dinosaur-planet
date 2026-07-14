#ifndef _DLL_73_H
#define _DLL_73_H

#include "dll_def.h"
#include "types.h"

typedef enum {
    DLL73_ACTION_None = -1
} DLL73_Actions;

DLL_INTERFACE(DLL_73) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*init_text_window)(s32 y);
/*1*/ void (*init_text_window_with_margin)(s32 marginX, s32 y);
/*2*/ void (*add_string)(s32 valueEnter, char* text, s32 lineHeight, s32 selectedIndex);
/*3*/ void (*add_string_x)(s32 valueEnter, char* text, s32 x, s32 lineHeight, s32 selectedIndex);
/*4*/ void (*set_exit_value)(s32 value);
/*5*/ s16 (*handle_joystick_and_buttons)(s32* idx);
/*6*/ void (*set_font_and_colour)(s32 dimmed);
/*7*/ void (*enable_joy_buttons)(s32 enabled);
/*8*/ s8 (*get_total_items)(void);
};

#endif // _DLL_73_H
