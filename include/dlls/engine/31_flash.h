#ifndef _DLLS_31_H
#define _DLLS_31_H

#include "PR/ultratypes.h"
#include "dll_def.h"

// size: 0x1800
typedef struct {
    /*0x0*/   u8 data[0x17F0];
    /*0x17f0*/u64 xor;
    /*0x17f8*/u64 sum;
} FlashStruct;

DLL_INTERFACE(DLL_31_flash) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    s16 (*load_game)(void *dest, u8 slotno, s32 size, s32 validateChecksum);
    s16 (*save_game)(void *src, u8 slotno, s32 size, s32 computeChecksum);
};

#endif //_DLLS_31_H
