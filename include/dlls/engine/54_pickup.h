#ifndef _DLL_54_H
#define _DLL_54_H

#include "dll_def.h"
#include "types.h"

#include "game/objects/object.h"
#include "unktypes.h"

enum PickupState {
    PICKUP_NotHeld = 0,
    PICKUP_BeingPickedUp = 1,
    PICKUP_Held = 2
};

enum PickupFlags {
    PICKUPFLAG_PickedUpThisTick = 0x1,
    PICKUPFLAG_NoGravity = 0x2,
    PICKUPFLAG_DropDisabled = 0x4,
    PICKUPFLAG_DontSave = 0x8
};

// size: 0xA
typedef struct {
/*0*/ s16 unk0;
/*2*/ s16 unk2;
/*4*/ u8 _unk4;
/*5*/ s8 state;
/*6*/ s8 unk6;
/*7*/ u8 flags;
/*8*/ u8 unk8;
} Pickup;

DLL_INTERFACE(DLL_54) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*setup)(Object* obj, Pickup* pickup, s16 arg2);
/*1*/ s32 (*control)(Object* obj, Pickup* pickup); // returns pickup state
/*2*/ s32 (*should_print)(Object* obj, s32 visibility);
/*3*/ void (*free)(Object* obj);
/*4*/ s32 (*get_state)(Pickup* pickup);
/*5*/ s32 (*was_just_picked_up)(Pickup* pickup); // was just picked up on this game tick
/*6*/ s32 (*func6)(Pickup* pickup);
/*7*/ void (*set_gravity)(Pickup* pickup, u8 enabled);
/*8*/ void (*set_drop_disabled)(Pickup* pickup, u8 disabled);
/*9*/ s32 (*get_drop_disabled)(Pickup* pickup);
/*10*/ void (*set_dont_save)(Pickup* pickup, u8 dontSave);
/*11*/ void (*drop)(Object* obj, Pickup* pickup);
};

#endif // _DLL_54_H
