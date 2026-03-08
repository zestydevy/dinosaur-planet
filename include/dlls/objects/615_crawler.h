#ifndef _DLLS_615_H
#define _DLLS_615_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    u8 _unk1C[8];
} Scorpion_Setup; //TODO: may be the same as Scarab_Setup?

#endif //_DLLS_615_H
