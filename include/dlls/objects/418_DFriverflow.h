#ifndef _DLLS_418_H
#define _DLLS_418_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    u8 yaw;
    s8 range;           //determines push falloff range and push strength (also used as scale, divided by 512)
    u8 flags;           //set during setup, bit1 must be set for player push interaction
    u8 pad1B;
    s16 toggleGamebit;  //optional: gamebitID switches off the riverflow when set
} DFriverflow_Setup;

typedef struct {
    u8 activated;
} DFriverflow_Data;

#endif // _DLLS_418_H
