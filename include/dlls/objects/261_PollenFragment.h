#ifndef _DLLS_261_H
#define _DLLS_261_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} PollenFragment_Setup;

typedef struct {
    u32 lifetime;
    u32 soundHandle;
} PollenFragment_Data;

#endif //_DLLS_261_H
