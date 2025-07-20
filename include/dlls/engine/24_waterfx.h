#ifndef _DLLS_24_H
#define _DLLS_24_H

#include "PR/ultratypes.h"
#include "sys/math.h"
#include "dll_def.h"

// size: 0x18
typedef struct Func564Arg0 {
    s16 unk0;
    s16 pad2;
    s32 pad4;
    s32 pad8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} Func564Arg0;

// size: 0x1B4
typedef struct Func564Arg3 {
    u8 pad0[0x1B0];
    f32 unk1B0;
} Func564Arg3;

DLL_INTERFACE_BEGIN(24_waterfx)
// TODO: (bug?) this function signature doesn't match the one in the waterfx source
/* 0 */ void (*func_6E8)(s32);
/* 1 */ void (*func_564)(Func564Arg0 *arg0, u16 arg1, Vec3f *arg2, Func564Arg3 *arg3, f32 arg4);
/* 2 */ void (*func_C7C)(Gfx** gdl, Mtx** arg1);
/* 3 */ void (*func_174C)(f32 arg0, f32 arg1, f32 arg2, f32 arg3);
/* 4 */ void (*func_1CC8)(f32 arg0, f32 arg1, f32 arg2, s16 arg3, f32 arg4, s32 arg5);
/* 5 */ void (*func_1B28)(f32 arg0, f32 arg1, f32 arg2, s16 arg3, f32 arg4);
/* 6 */ void (*func_24C)(void);
/* 7 */ void (*func_1C88)(s32 arg0, f32 arg1);
DLL_INTERFACE_END()

#endif //_DLLS_24_H
