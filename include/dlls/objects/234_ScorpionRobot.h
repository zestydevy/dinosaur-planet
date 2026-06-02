#ifndef _DLLS_234_H
#define _DLLS_234_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_234_ScorpionRobot) {
/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
/*07*/ s32 (*get_spin)(Object* self);
};

#endif
