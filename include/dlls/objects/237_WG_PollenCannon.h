#ifndef _DLLS_237_H
#define _DLLS_237_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    s32 unk0;
    s16 unk4;
    u16 unk6_0 : 3;    //total Objects in unk8
    u16 unk6_1 : 13;
    Object* unk8[8];   //array size just a guess
} PollenCannonUnk3F4;

typedef struct {
    s8 unk0[0x3F4];
    PollenCannonUnk3F4* unk3F4;
} PollenCannon_Data;

#endif //_DLLS_237_H
