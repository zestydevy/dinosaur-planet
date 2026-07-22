#ifndef _DLLS_338_H
#define _DLLS_338_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/lfx.h"

typedef struct {
    u8 unk0[0x108];
    LightAction *unk108;
} LFXEmitter_Data;

typedef struct {
/*00*/    ObjSetup base;
/*18*/    s16 unk18;
/*1A*/    s16 unk1A;
/*1C*/    s16 unk1C;
/*1E*/    s16 unk1E;
/*20*/    s16 unk20;
/*22*/    s16 unk22;
/*24*/    s8 unk24;
/*25*/    s8 unk25;
/*26*/    s16 unk26;
} LFXEmitter_Setup;

DLL_INTERFACE(DLL_338_LFXEmitter) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ UnknownDLLFunc func7;
    /*8*/ void (*func8)(Object* self, f32 opacity);
    //TODO: find out how many other exports this DLL has
};

#endif // _DLLS_338_H
