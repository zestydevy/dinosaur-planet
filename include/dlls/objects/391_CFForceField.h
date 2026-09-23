#ifndef _DLLS_391_H
#define _DLLS_391_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 _unk19[0x1C - 0x19];
    f32 unk1C;
} CFForceField_Setup;

#endif
