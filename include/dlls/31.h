#ifndef _DLLS_31_H
#define _DLLS_31_H

#include <PR/ultratypes.h>
#include "dll_def.h"

typedef struct {
    /*0x0*/   u8 unk0x0[0x2f8];
    /*0x2f8*/ u8 unk0x2f8;
    /*0x2f9*/ u8 unk0x2f9[0x14A7];
    /*0x17a0*/u8 unk0x17a0[0x50];
    /*0x17f0*/u64 xor;
    /*0x17f8*/u64 sum;
} SaveStruct;

DLL_INTERFACE_BEGIN(31_Save)
    s32 (*flashLoadGame)(SaveStruct *param1, u8 param2, s32 param3, s32 param4);
    s16 (*flashSaveGame)(SaveStruct *param1, u8 param2, s32 param3, s32 param4);
DLL_INTERFACE_END()

#endif //_DLLS_31_H
