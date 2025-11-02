#ifndef _DLLS_277_H
#define _DLLS_277_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

// size: 0x24
typedef struct {
    ObjSetup base;
    /*18*/ s8 unused18;
    /*19*/ s8 unk19; //collision-related
    /*1A*/ s16 timer;
    /*1C*/ u16 unk1C;
    /*1E*/ u8 pad1E[0x24 - 0x1E];
} Iceblast_Setup;

#endif //_DLLS_277_H
