#ifndef _DLLS_338_H
#define _DLLS_338_H

#include "PR/ultratypes.h"
#include "types.h"
#include "game/objects/object.h"

typedef struct {
    u8 unk0[0x108];
    NewLfxStruct *unk108;
} LFXEmitter_Data;

typedef struct {
    ObjSetup base;
    u8 unk18[0x1E-0x18];
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    u8 unk24[0x28-0x24];
} LFXEmitter_Setup;

#endif // _DLLS_338_H
