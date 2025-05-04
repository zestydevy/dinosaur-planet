#ifndef _DLLS_31_H
#define _DLLS_31_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "dlls/engine/29_gplay.h"

// size: 0x1800
typedef struct {
    /*0x0*/   GplayStruct3 gplay;
    /*0x17ac*/u8 _unk0x0x17ac[0x44];
    /*0x17f0*/u64 xor;
    /*0x17f8*/u64 sum;
} FlashStruct;

DLL_INTERFACE_BEGIN(31_flash)
    s32 (*load_game)(FlashStruct *param1, u8 param2, s32 param3, s32 param4);
    s16 (*save_game)(FlashStruct *param1, u8 param2, s32 param3, s32 param4);
DLL_INTERFACE_END()

#endif //_DLLS_31_H
