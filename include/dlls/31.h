#ifndef _DLLS_31_H
#define _DLLS_31_H

#include <PR/ultratypes.h>

typedef struct {
    /*0x0*/   u32 unk0x0[1512];
    /*0x17a0*/u8 unk0x17a0[0x50];
    /*0x17f0*/u64 xor;
    /*0x17f8*/u64 sum;
} SaveStruct;

typedef struct {
    u32 _pad;
    s32 (*flashLoadGame)(SaveStruct *param1, u8 param2, s32 param3, s32 param4);
    s16 (*flashSaveGame)(SaveStruct *param1, u8 param2, s32 param3, s32 param4);
} Dll31Interface;

#endif //_DLLS_31_H
