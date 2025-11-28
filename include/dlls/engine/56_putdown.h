#ifndef _DLLS_56_H
#define _DLLS_56_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

#include "dlls/engine/29_gplay.h"
#include "dlls/objects/314_foodbag.h"


DLL_INTERFACE(DLL_56_Putdown) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ int (*putdown_place_food)(Object* arg0, s32 foodType, FoodbagPlaced* placedFood, FoodbagItem* foodDefs);
    /*1*/ Object* (*putdown_get_nearest_placed_food_of_type)(Object* arg0, Object* arg1, s32 typeMask, FoodbagPlaced* placedObjects);
    /*2*/ int (*putdown_destroy_placed_food)(Object* foodObject, FoodbagPlaced* placedObjects);
    /*3*/ u16 (*putdown_func_47C)(GplayStruct14* bagTimers, FoodbagItem* foodDefs);
    /*4*/ int (*putdown_func_5E8)(s32 arg0, s32 arg1, GplayStruct14* bagTimers, FoodbagItem* foodDefs);
    /*5*/ void (*putdown_func_730)(s16 arg0, GplayStruct14* bagTimers, FoodbagItem* foodDefs);
    /*6*/ void (*putdown_func_80C)(u8 arg0, u16 arg1, GplayStruct14* bagTimers, FoodbagItem* foodDefs);
    /*7*/ s32 (*putdown_get_capacity)(FoodbagGamebits* gamebitIDs);
    /*8*/ void (*putdown_update_food_quantity_gamebits)(GplayStruct14* bagTimers, FoodbagItem* foodDefs);
    /*9*/ u32 (*putdown_func_AFC)(s32 arg0); //get foodID from foodType?
};

#endif // _DLLS_56_H
