#ifndef _DLLS_276_H
#define _DLLS_276_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

// size: 0x20
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 pad1B[0x20 - 0x1B];    
} InvHit_Setup;

#endif //_DLLS_276_H
