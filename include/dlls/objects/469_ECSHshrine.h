#ifndef _DLLS_469_H
#define _DLLS_469_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_469_ECSHshrine) {
/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
/*7*/ void (*func7)(s16 *arg0);
/*8*/ void (*func8)(u8 arg0, f32 *arg1, f32 *arg2);
/*9*/ void (*func9)(s32 *arg0, u8 *arg1);
/*10*/ void (*func10)(u8 arg0, f32 arg1, f32 arg2);
/*11*/ void (*func11)(u8 arg0);
};

#endif // _DLLS_469_H
