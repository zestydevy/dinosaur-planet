#ifndef _DLLS_325_H
#define _DLLS_325_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/math.h"

DLL_INTERFACE(DLL_TriggerScript) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    // Array length will vary
    void (*subscripts[1])(Object *trigger, Object *activator, s8 dir, s32 activatorDistSquared);
};

typedef struct {
/*00*/ u8 flags; // TriggerFlags enum
/*01*/ u8 _unk1[3];
/*04*/ f32 radiusSquared;
/*08*/ u8 _unk8[4];
/*0C*/ u32 elapsedTicks; // for TriggerTime
/*10*/ Vec3f lookVector; // for TriggerPlane
/*1C*/ f32 lookVectorNegDot;
/*20*/ Vec3f activatorPrevPos;
/*2C*/ Vec3f activatorCurrPos;
/*38*/ Vec3f planeMin;
/*44*/ Vec3f planeMax;
/*50*/ u8 _unk50[8];
/*58*/ s16 bitFlagID;
/*5A*/ s16 conditionBitFlagIDs[4];
/*62*/ u8 _unk62[2];
/*64*/ u32 soundHandles[8];
// Special "script" DLLs where each export is a "subscript".
/*84*/ DLL_TriggerScript *scripts[8];
} Trigger_Data;

typedef enum {
    // Activator entered at least once
    TRG_ACTIVATOR_ENTERED = 0x1,
    // Activator exited at least once
    TRG_ACTIVATOR_EXITED = 0x2,
    TRG_RESTORE_ENTERED_STATE = 0x4,
    TRG_FIRST_TICK = 0x40
} TriggerFlags;

#endif
