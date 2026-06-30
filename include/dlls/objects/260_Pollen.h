#ifndef _DLLS_260_H
#define _DLLS_260_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} Pollen_Setup; //0x22

typedef struct {
    s16 unk0;
    s16 pad2;
    s16 unk4;
    s16 unk6;
    f32 unk8;
    f32 unkC;
    s16 unk10;
    s16 unk12;
} Pollen_Data;

#endif //_DLLS_260_H
