#ifndef _DLLS_779_H
#define _DLLS_779_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_779_WCLevelControl) {
/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
/*7*/ void (*func7)(SRT *arg0, s16 arg1, s16 arg2, f32 *arg3, f32 *arg4);
/*8*/ void (*func8)(SRT *arg0, f32 arg1, f32 arg2, s16 *arg3, s16 *arg4);
/*9*/ void (*func9)(s16 arg0, s16 x, s16 y);
/*10*/ u8 (*func10)(s16 x, s16 y);
/*11*/ void (*func11)(s16 arg0, s16 *outX, s16 *outY);
/*12*/ void (*func12)(s16 arg0, s16 *outX, s16 *outY);
/*13*/ s32 (*func13)(SRT *arg0, s16 arg1, s16 arg2, f32 *arg3, f32 *arg4, s32 arg5, s32 arg6);
/*14*/ void (*func14)(SRT *arg0, s16 arg1, s16 arg2, f32* arg3, f32 *arg4);
/*15*/ void (*func15)(SRT *arg0, f32 arg1, f32 arg2, s16 *arg3, s16 *arg4);
/*16*/ void (*func16)(s16 arg0, s16 x, s16 y);
/*17*/ u8 (*func17)(s16 x, s16 y);
/*18*/ void (*func18)(s16 arg0, s16 *outX, s16 *outY);
/*19*/ void (*func19)(s16 arg0, s16 *outX, s16 *outY);
/*20*/ s32 (*func20)(SRT *arg0, s16 arg1, s16 arg2, f32 *arg3, f32 *arg4, s32 arg5, s32 arg6);
};

#endif // _DLLS_779_H
