#ifndef _DLLS_338_H
#define _DLLS_338_H

#include "PR/ultratypes.h"
#include "dlls/engine/26_curves.h"
#include "game/objects/object.h"
#include "sys/lfx.h"

enum LFXEmitterFlags {
    LFXEMITTER_FollowCurve = 1,
    LFXEMITTER_HasGravity = 2
};

typedef struct {
/*000*/ UnkCurvesStruct unk0;
/*108*/ LightAction* action;
/*10C*/ f32 curveSpeed;
/*110*/ s16 lifetime;
/*112*/ s16 actionIndex;
/*114*/ s16 unk114;
/*116*/ s16 enableBit;
/*118*/ s16 rollRate;
/*11A*/ s16 pitchRate;
/*11C*/ s16 yawRate;
/*11E*/ u8 hasLifetimeTimer;
/*11F*/ u8 actionApplied;
/*120*/ u8 flags; // LFXEmitterFlags
} LFXEmitter_Data;


DLL_INTERFACE(DLL_338_LFXEmitter) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*GetActionSlot)(Object* self);
    /*8*/ void (*Func_6DC)(Object* self, f32 opacity);
};

#endif // _DLLS_338_H
