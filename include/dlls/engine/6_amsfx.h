#ifndef _DLLS_6_H
#define _DLLS_6_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_6_AMSFX) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*func0)(void);
    /*1*/ void (*func1)(Object *obj);
    /** 2 - Play sound using source object for panning and distance falloff */
    s32 (*func2)(Object* obj, u16 soundID, u8 volume, u32 *sound, s32, s32, s32);
    /*3*/ UnknownDLLFunc func3;
    /*4*/ void (*func4)(s32, u8);
    /*5*/ void (*func5)(s32, f32);
    /*6*/ void (*func6)(s32 sound); //stop sound?
    /*7*/ UnknownDLLFunc func7;
    /*8*/ s32 (*func8)(s32);
    /*9*/ UnknownDLLFunc func9;
    /*10*/ s32 (*func10)(s32, s32, s32*, s32*);
    /*11*/ UnknownDLLFunc func11;
    /*12*/ UnknownDLLFunc func12;
    /*13*/ UnknownDLLFunc func13;
    /*14*/ void (*func14)(Object* obj, u16 soundID, s32*);
    /*15*/ UnknownDLLFunc func15;
    /*16*/ void (*func16)(Object *obj);
    /*17*/ UnknownDLLFunc func17;
    /*18*/ UnknownDLLFunc func18;
    /*19*/ void (*water_falls_set_flags)(u8);
    /*20*/ UnknownDLLFunc func20;
};

#endif //_DLLS_6_H
