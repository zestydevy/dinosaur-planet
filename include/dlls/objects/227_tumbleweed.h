#ifndef _DLLS_227_H
#define _DLLS_227_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_227_Tumbleweed) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*00*/ UnknownDLLFunc func0;
    /*01*/ UnknownDLLFunc func1;
    /*02*/ UnknownDLLFunc func2;
    /*03*/ s32 (*func3)(Object*, Vec3f*);
    /*04*/ s32 (*func4)(Object*);
    /*05*/ UnknownDLLFunc func5;
    /*06*/ UnknownDLLFunc func6;
    /*07*/ UnknownDLLFunc func7;
    /*08*/ UnknownDLLFunc func8;
    /*09*/ UnknownDLLFunc func9;
    /*10*/ UnknownDLLFunc func10;
    /*11*/ UnknownDLLFunc func11;
    /*12*/ UnknownDLLFunc func12;
};

#endif //_DLLS_227_H
