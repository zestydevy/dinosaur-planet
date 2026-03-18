#ifndef _DLLS_253_H
#define _DLLS_253_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
} CannonBall_Setup;

typedef struct {
    s16 timer;
} CannonBall_Data;

#endif // _DLLS_253_H
