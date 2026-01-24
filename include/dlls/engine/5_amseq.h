#ifndef _DLLS_5_H
#define _DLLS_5_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_5_AMSEQ) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ s32 (*func0)(Object* arg0, u16 arg1, s32 arg2, s32 arg3, s32 arg4);
    /*1*/ void (*func1)(Object *arg0, u16 arg1, s32 arg2, s32 arg3, s32 arg4);
    /*2*/ s32 (*func2)(Object *arg0, u16 arg1);
    /*3*/ void (*func3)(Object *arg0);
    /*4*/ void (*func4)(u8 arg0, u8 arg1);
    /*5*/ void (*func5)(u8 arg0, u8 arg1, s16 arg2, s16 arg3, u16 arg4);
    /*6*/ void (*func6)(u8 arg0);
    /*7*/ u8 (*func7)(u8 arg0);
    /*8*/ u16 (*func8)(void);
    /*9*/ void (*func9)(u32 volume);
    /*10*/ UNK_TYPE_32 (*func10)(void);
    /*11*/ void (*func11)(UNK_TYPE_32 arg0);
    /*12*/ u8 (*func12)(u8 arg0);
    /*13*/ void (*func13)(u8 arg0, u8 arg1);
    /*14*/ UNK_TYPE_32 (*func14)(UNK_TYPE_32 arg0);
    /*15*/ void (*func15)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*16*/ void (*func16)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*17*/ void (*func17)(Object *obj);
    /*18*/ void (*func18)(void);
    /*19*/ void (*func19)(UNK_TYPE_32 arg0);
    /*20*/ void (*func20)(UNK_TYPE_32 arg0);
    /*21*/ void (*func21)(UNK_TYPE_32 arg0);
    /*22*/ void (*func22)(UNK_TYPE_32 arg0);
    /*23*/ void (*func23)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*24*/ void (*func24)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*25*/ void (*func25)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*26*/ UNK_TYPE_32 (*func26)(UNK_TYPE_32 arg0);
    /*27*/ void (*func27)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*28*/ void (*func28)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*29*/ void (*func29)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*30*/ UNK_TYPE_32 (*func30)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*31*/ UNK_TYPE_32 (*func31)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*32*/ void (*func32)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2);
    /*33*/ UNK_TYPE_32 (*func33)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
    /*34*/ void (*func34)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2);
    /*35*/ void (*func35)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2);
};

#endif //_DLLS_5_H
