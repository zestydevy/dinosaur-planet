#ifndef _DLLS_56_H
#define _DLLS_56_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

#include "dlls/engine/29_gplay.h"
#include "dlls/objects/314_foodbag.h"

DLL_INTERFACE(DLL_56_Putdown) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ int (*putdown_place_food)(Object* foodbag, s32 foodType, FoodbagPlaced* placed, FoodbagItem* foodDefs);
    /*1*/ Object* (*putdown_get_nearest_placed_food_of_type)(Object* foodbag, Object* target, s32 foodType, FoodbagPlaced* placedObjects);
    /*2*/ int (*putdown_destroy_placed_food)(Object* foodObject, FoodbagPlaced* placedObjects);
    /*3*/ u16 (*putdown_tick_food_lifetimes)(FoodbagContents* bagSlots, FoodbagItem* foodDefs);
    /*4*/ int (*putdown_add_food)(s32 foodType, s32 capacity, FoodbagContents* bagSlots, FoodbagItem* foodDefs);
    /*5*/ void (*putdown_delete_food_by_gamebit)(s16 foodGamebit, FoodbagContents* bagSlots, FoodbagItem* foodDefs);
    /*6*/ void (*putdown_delete_food_from_bag)(u8 indexInBag, u16 foodType, FoodbagContents* bagSlots, FoodbagItem* foodDefs);
    /*7*/ s32 (*putdown_get_capacity)(FoodbagGamebits* gamebitIDs);
    /*8*/ void (*putdown_update_food_quantity_gamebits)(FoodbagContents* bagSlots, FoodbagItem* foodDefs);
    /*9*/ u32 (*putdown_get_foodID_from_foodType)(s32 foodTypeBitfield);
};

#endif // _DLLS_56_H
