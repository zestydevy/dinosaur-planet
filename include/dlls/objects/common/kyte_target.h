#ifndef _DLLS_COMMON_KYTE_TARGET_H
#define _DLLS_COMMON_KYTE_TARGET_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "PR/gbi.h"

#define KYTE_TARGET_FUNC10_FLAG_0 0
#define KYTE_TARGET_FUNC10_FLAG_1 1
#define KYTE_TARGET_FUNC10_FLAG_2 2
#define KYTE_TARGET_FUNC10_FLAG_4 4
#define KYTE_TARGET_FUNC10_FLAG_8 8


DLL_INTERFACE(DLL_IKyteTarget) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
	/*7*/ s32 (*Interact)(Object* self, s32 arg1);
	/*8*/ s32 (*func8)(Object *self, s32 arg1, s32 arg2);
	/*9*/ s32 (*Approach)(Object* self, s32 arg1, f32* deltaY);
	/*10*/ s32 (*func10)(Object *self); // returns some flag
};

#endif //_DLLS_COMMON_KYTE_TARGET_H
