#ifndef _DLL_H
#define _DLL_H

#include "dlls/engine/21_gametext.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/30_task.h"
#include "dlls/engine/31_flash.h"

#include "dll_def.h"

// "Unknown" DLL interface to help match code calling DLLs where the exact
// DLL being called has not been determined 
DLL_INTERFACE_BEGIN(Unknown)
    union {
        /* 0x04 */ void (*asVoid)(void);
        /* 0x04 */ s32 (*asVoidS32)(void);
        /* 0x04 */ void (*withOneArg)(s32);
        /* 0x04 */ void (*withTwoArgs)(s32, s32);
        /* 0x04 */ void (*withThreeArgs)(s32, s32, s32);
        /* 0x04 */ void (*withFourArgs)(s32, s32, s32, s32);
        /* 0x04 */ void (*withFiveArgs)(s32, s32, s32, s32, u16);
    } func[1];
DLL_INTERFACE_END()

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
    DLL_GPLAY     = 29,
    DLL_TASK      = 30,
    DLL_FLASH     = 31,
    DLL_MINIMAP   = 59,
    DLL_LINK      = 74,
    DLL_CANNON_CLAW = 223
};


// "Static" DLLs
// These are loaded on game init, are never unloaded, and are referenced by
// their global variable instead of from loading the DLL.

extern DLLInst_21_gametext *gDLL_21_gametext;
extern DLLInst_29_gplay *gDLL_29_gplay;
extern DLLInst_30_task *gDLL_30_task;
extern DLLInst_31_flash *gDLL_31_flash;
extern DLLInst_Unknown
    *gDLL_28,
    *gDLL_Camera,
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
    *gDLL_AMSFX,
    *gDLL_newlfx,
    *gDLL_57,
    *gDLL_58,
    *gDLL_expgfx,
    *gDLL_modgfx,
    *gDLL_projgfx,
    *gDLL_16,
    *gDLL_17,
    *gDLL_18,
    *gDLL_SCREENS,
    *gDLL_subtitles,
    *gDLL_23,
    *gDLL_waterfx,
    *gDLL_25,
    *gDLL_CURVES,
    *gDLL_Link,
    *gDLL_75,
    *gDLL_27,
    *gDLL_56,
    *gDLL_76,
    *gDLL_32,
    *gDLL_33,
    *gDLL_minimap,
    *gDLL_54;

#endif //_DLL_H
