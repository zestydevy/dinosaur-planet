#ifndef _DLLS_227_H
#define _DLLS_227_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_227_Tumbleweed) {
/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
/*07*/ u8 (*func7)(Object* self);
/*08*/ void (*func8)(Object* self, f32 arg1, f32 arg2);
/*09*/ void (*func9)(Object* self);
/*10*/ void (*func10)(Object* self, Vec3f* arg1);
/*11*/ s32 (*func11)(Object* self);
/*12*/ void (*func12)(Object* self, s32 arg1);
};

#endif //_DLLS_227_H
