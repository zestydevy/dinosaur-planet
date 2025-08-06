#ifndef _DLLS_22_H
#define _DLLS_22_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_22_subtitles) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/* 0 */ u32 (*func_2D0)(u32 arg0);
/* 1 */ void (*func_2F4)(void);
/* 2 */ void (*func_32C)(u8 arg0, u8 arg1);
/* 3 */ void (*func_368)(u16 arg0);
/* 4 */ void (*func_448)(void);
/* 5 */ void (*func_4C0)(void);
/* 6 */ void (*func_578)(Gfx **gdl);
/* 7 */ s16 (*func_214C)(s32 arg0);
/* 8 */ s32 (*func_21C0)(s32 arg0, s16 arg1);
/* 9 */ s32 (*func_2248)(u8 arg0);
};

#endif //_DLLS_22_H
