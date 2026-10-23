#ifndef _DLLS_338_H
#define _DLLS_338_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/lfx.h"

typedef struct {
    u8 unk0[0x108];
    LightAction *unk108;
} LFXEmitter_Data;

DLL_INTERFACE(DLL_338_LFXEmitter) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ UnknownDLLFunc func7;
    /*8*/ void (*func8)(Object* self, f32 opacity);
    //TODO: find out how many other exports this DLL has
};

#endif // _DLLS_338_H
