#ifndef _DLLS_84_H
#define _DLLS_84_H

#include "PR/ultratypes.h"
#include "dlls/engine/2_camcontrol.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_84_camnormal) {
/*:*/ DLL_INTERFACE_BASE(DLL_ICamControlModule);
/*4*/ void (*func4)(CamControl_Data* arg0, Object* arg1);
/*5*/ void (*func5)(CamControl_Data* arg0, f32 arg1, f32 arg2);
/*6*/ void (*func6)(CamControl_Data* arg0, Object* arg1, f32 arg2, f32 arg3);
/*7*/ void (*func7)(f32* arg0, f32* arg1, f32* arg2, f32* arg3, f32* arg4);
/*8*/ void (*func8)(CamControl_Data* arg0, s32 arg1, s32 arg2, f32* arg3, f32* arg4);
};

#endif
