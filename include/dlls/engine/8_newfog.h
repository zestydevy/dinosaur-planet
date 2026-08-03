#ifndef _DLL_8_H
#define _DLL_8_H

#include "PR/gbi.h"
#include "game/objects/object.h"
#include "sys/envfx.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_8_newfog) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(Object* objA, Object* objB, EnvFxAction* action, s32 arg3, u16 actionIndex);
/*1*/ void (*func1)(void);
/*2*/ void (*func2)(void);
/*3*/ void (*func3)(Gfx** gdl);
/*4*/ s32 (*func4)(s32 arg0);
/*5*/ void (*func5)(Gfx** gdl);
/*6*/ void (*func6)(s32* arg0, s32* arg1, s32* arg2);
/*7*/ void (*GetColor)(s32* r, s32* g, s32* b, f32* arg3);
/*8*/ void (*GetPosition)(s32* min, s32* max);
/*9*/ void (*func9)(void);
/*10*/ void (*func10)(Gfx** gdl);
/*11*/ void (*func11)(Gfx** gdl);
/*12*/ s16 (*func12)(void);
};

#endif // _DLL_8_H
