#ifndef _DLLS_516_H
#define _DLLS_516_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s8 yaw;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s8 unk32;
    s32 unk34;
} SCLightFoot_Setup; //0x38

DLL_INTERFACE(DLL_516_SC_lightfoot) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ UnknownDLLFunc func7;
    /*8*/ UnknownDLLFunc func8;
    /*9*/ UnknownDLLFunc func9;
    /*10*/ UnknownDLLFunc func10;
    /*11*/ UnknownDLLFunc func11;
    /*12*/ UnknownDLLFunc func12;
    /*13*/ UnknownDLLFunc func13;
    /*14*/ UnknownDLLFunc func14;
    /*15*/ UnknownDLLFunc func15;
    /*16*/ UnknownDLLFunc func16;
    /*17*/ UnknownDLLFunc func17;
    /*18*/ UnknownDLLFunc func18;
    /*19*/ UnknownDLLFunc func19;
    /*20*/ UnknownDLLFunc func20;
};

#endif // _DLLS_516_H
