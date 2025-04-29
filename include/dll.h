#ifndef _DLL_H
#define _DLL_H

#include "dlls/engine/2_camera.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/20_screens.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/28_screen_fade.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/30_task.h"
#include "dlls/engine/31_flash.h"
#include "dlls/engine/74_picmenu.h"
#include "dlls/engine/76.h"

#include "dll_def.h"
#include "types.h"

enum DLL_ID {
    DLL_UI        = 1,
    DLL_CAMERA    = 2,
    DLL_ANIM      = 3,
    DLL_RACE      = 4,
    DLL_AMSEQ     = 5,
    DLL_AMSFX     = 6,
    DLL_SKY       = 7,
    DLL_NEWCLOUDS = 9,
    DLL_NEWSTARS  = 10,
    DLL_NEWLFX    = 11,
    DLL_MINIC     = 12,
    DLL_EXPGFX    = 13,
    DLL_MODGFX    = 14,
    DLL_PROJGFX   = 15,
    DLL_SCREENS   = 20,
    DLL_TEXT      = 21,
    DLL_SUBTITLES = 22,
    DLL_WATERFX   = 24,
    DLL_CURVES    = 26,
    DLL_SCREEN_FADE = 28,
    DLL_GPLAY     = 29,
    DLL_TASK      = 30,
    DLL_FLASH     = 31,
    DLL_MINIMAP   = 59,
    DLL_PICMENU   = 74,
    DLL_CANNON_CLAW = 223
};


// TODO: the below should be moved to main.h

// "Static" DLLs
// These are loaded on game init, are never unloaded, and are referenced by
// their global variable instead of from loading the DLL.

extern DLLInst_2_Camera *gDLL_Camera;
extern DLLInst_6_AMSFX *gDLL_AMSFX;
extern DLLInst_20_screens *gDLL_20_screens;
extern DLLInst_21_gametext *gDLL_21_gametext;
extern DLLInst_28_screen_fade *gDLL_28_screen_fade;
extern DLLInst_29_gplay *gDLL_29_gplay;
extern DLLInst_30_task *gDLL_30_task;
extern DLLInst_31_flash *gDLL_31_flash;
extern DLLInst_74_picmenu *gDLL_picmenu;
extern DLLInst_Unknown
    *gDLL_ANIM,
    *gDLL_Sky,
    *gDLL_8,
    *gDLL_newclouds,
    *gDLL_newstars,
    *gDLL_minic,
    *gDLL_UI,
    *gDLL_Race,
    *gDLL_AMSEQ,
    *gDLL_AMSEQ2,
    *gDLL_newlfx,
    *gDLL_57,
    *gDLL_58,
    *gDLL_expgfx,
    *gDLL_modgfx,
    *gDLL_projgfx,
    *gDLL_16,
    *gDLL_17,
    *gDLL_18,
    *gDLL_subtitles,
    *gDLL_23,
    *gDLL_waterfx,
    *gDLL_25,
    *gDLL_CURVES,
    *gDLL_75,
    *gDLL_27,
    *gDLL_56,
    *gDLL_32,
    *gDLL_33,
    *gDLL_minimap,
    *gDLL_54;

    
// Other common DLLs that aren't always loaded but are accessed globally

extern DLLInst_76 *gDLL_76;

#endif //_DLL_H
