#ifndef _DLLS_223_H
#define _DLLS_223_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 _unk18[0x1a - 0x18];
/*1A*/ s16 gamebit;
} CannonClaw_Setup;

#endif //_DLLS_223_H
