#ifndef _DLLS_315_H
#define _DLLS_315_H

#include "dlls/engine/56_putdown.h"
#include "dlls/objects/314_foodbag.h"

typedef struct {
    ObjSetup base;
} SideFoodbag_Setup;

typedef struct {
    DLL_56_Putdown *dllPutdown;         //Handles removing/updating food bag items
    FoodbagGamebits capacityGamebits;   //Bag size gamebits
    u16 capacity;                       //Current capacity
    FoodbagContents* bagSlots;          //Expiry timer and food type for each item currently in bag
    FoodbagPlaced placedObjects;        //Food placed in world
} SideFoodbag_Data;

DLL_INTERFACE(DLL_315_SideFoodbag) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ int (*is_obtained)(Object* self);
    /*8*/ Object* (*get_nearest_placed_food_of_type)(Object* self, Object* target, s32 foodType);
    /*9*/ int (*destroy_placed_food)(Object* self, Object* foodObject);
    /*10*/ void (*set_capacity)(Object* self);
    /*11*/ int (*collect_food)(Object* self, s32 foodType);
    /*12*/ void (*delete_food_by_gamebit)(Object* self, s16 foodGamebit);
};

#endif // _DLLS_315_H
