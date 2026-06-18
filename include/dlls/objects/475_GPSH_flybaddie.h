#ifndef _DLLS_475_H
#define _DLLS_475_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
/*1C*/ u8 _unk1C[0x24 - 0x1C];
} GPSH_flybaddie_Setup;

#endif
