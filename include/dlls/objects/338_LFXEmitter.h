#ifndef _DLLS_338_H
#define _DLLS_338_H

#include "PR/ultratypes.h"
#include "types.h"
#include "game/objects/object.h"

typedef struct {
    u8 unk0[0x108];
    LightAction *unk108;
} LFXEmitter_Data;

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s8 unk24;
    s8 unk25;
    s16 unk26;
} LFXEmitter_Setup;

#endif // _DLLS_338_H
