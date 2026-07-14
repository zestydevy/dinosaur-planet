#ifndef _DLL_75_H
#define _DLL_75_H

#include "dll_def.h"
#include "types.h"

typedef enum {
    FRONTEND_CONTROL_Slider,
    FRONTEND_CONTROL_Checkbox,
    FRONTEND_CONTROL_List
} FrontEnd_Control_Types;

typedef enum {
    FRONTEND_FLAG_0_None = 0x0,
    FRONTEND_FLAG_1_Highlighted = 0x1,      //Control is currently highlighted
    FRONTEND_FLAG_2_Wrap_Value = 0x2,       //Sliders can wrap or clamp when moving past min/max values
    FRONTEND_FLAG_4_Moved_Left = 0x4,       //Lists highlight their left arrow when moving left
    FRONTEND_FLAG_8_Moved_Right = 0x8,      //Lists highlight their right arrow when moving right
    FRONTEND_FLAG_10_Value_Changed = 0x10,  //Can be used to drive logic when the player interacts with the control
    FRONTEND_FLAG_20_Locked = 0x20          //No interaction possible
} FrontEnd_Flags;

/* Generic FrontEnd control */
typedef struct {
/*0*/    s16 x;
/*2*/    s16 y;
/*4*/    u8 flags;
/*5*/    u8 type; 
/*6*/    s8 redrawFrames;
/*8*/    s16 min;
/*A*/    s16 max;
/*C*/    s16 value;
} FrontEndControl;

typedef struct {
/*0*/    s16 x;
/*2*/    s16 y;
/*4*/    u8 flags;
/*5*/    u8 type;
/*6*/    s8 redrawFrames;
/*8*/    s16 min;
/*A*/    s16 max;
/*C*/    s16 value;
/*E*/    s16 width;
} FrontEndSlider;

typedef struct {
/*0*/    s16 x;
/*2*/    s16 y;
/*4*/    u8 flags;
/*5*/    u8 type; 
/*6*/    s8 redrawFrames;
/*8*/    s16 min;
/*A*/    s16 max;
/*C*/    s16 value;
} FrontEndCheckbox;

typedef struct {
/*0*/    s16 x;
/*2*/    s16 y;
/*4*/    u8 flags;
/*5*/    u8 type;
/*6*/    s8 redrawFrames;
/*8*/    s16 min;
/*A*/    s16 max;
/*C*/    s16 value;
/*10*/   char** strings;
/*14*/   u8 halfWidth;
} FrontEndList;

DLL_INTERFACE(DLL_75) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ FrontEndSlider* (*create_slider)(s16 x, s16 y, s16 min, s16 max, s16 initialValue);
/*1*/ FrontEndCheckbox* (*create_checkbox)(s16 x, s16 y, s16 min, s16 max, s16 initialValue);
/*2*/ FrontEndList* (*create_list)(s16 x, s16 y, s16 min, s16 max, s16 initialValue, char** strings, u8 halfWidth);
/*3*/ void (*free)(FrontEndControl* ctrl);
/*4*/ void (*update)(FrontEndControl* ctrl);
/*5*/ void (*draw)(FrontEndControl* ctrl, Gfx** gdl);
/*6*/ s32 (*get_highlight_state)(FrontEndControl* ctrl);
/*7*/ void (*set_highlight_state)(FrontEndControl* slider, s32 enable);
/*8*/ s32 (*get_value)(FrontEndControl* ctrl);
/*9*/ void (*set_value)(FrontEndControl* ctrl, s32 value);
/*10*/ s32 (*did_value_change)(FrontEndControl* ctrl);
/*11*/ void (*set_unlock_state)(FrontEndControl* ctrl, s32 unlock);
};

#endif // _DLL_75_H
