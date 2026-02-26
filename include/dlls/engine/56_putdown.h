#ifndef _DLLS_56_H
#define _DLLS_56_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

#include "dlls/engine/29_gplay.h"

#define MAX_FOOD_PLACED 20      //Max number of items that can be placed in world from a given bag

/** Handles food objects placed in the world */
typedef struct {
    u8 nextIndex; //index to use when next placing food (eventually wraps around to overwrite oldest)
    Object* objects[MAX_FOOD_PLACED]; //array of food objects placed
    u16 foodType[MAX_FOOD_PLACED];    //foodType of each placed object
} FoodbagPlaced;

typedef struct {
    u16 small;
    u16 medium;
    u16 large;
} FoodbagGamebits;

/** Defines various parameters for a type of food that can be stored in a food bag */
typedef struct {
    s16 expiry;         //frames until food spoils
    u8 healthRestored;  //when eaten by player, or for sidekicks: energy restored
    u8 slotsUsed;       //food usually occupies 1 bag slot, but fish seem to occupy 2 slots
    u16 expiryID;       //bitfield storing foodID to transform into once expired (or vanishes on expiry if 0)
    s16 gamebitID;      //inventory item gamebit
    s16 objectID;       //Object to create when placed
} FoodbagItem;

DLL_INTERFACE(DLL_56_Putdown) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ int (*place_food)(Object* foodbag, s32 foodType, FoodbagPlaced* placed, FoodbagItem* foodDefs);
    /*1*/ Object* (*get_nearest_placed_food_of_type)(Object* foodbag, Object* target, s32 foodType, FoodbagPlaced* placedObjects);
    /*2*/ int (*destroy_placed_food)(Object* foodObject, FoodbagPlaced* placedObjects);
    /*3*/ u16 (*tick_food_lifetimes)(FoodbagContents* bagSlots, FoodbagItem* foodDefs);
    /*4*/ int (*add_food)(s32 foodType, s32 capacity, FoodbagContents* bagSlots, FoodbagItem* foodDefs);
    /*5*/ void (*delete_food_by_gamebit)(s16 foodGamebit, FoodbagContents* bagSlots, FoodbagItem* foodDefs);
    /*6*/ void (*delete_food_from_bag)(u8 indexInBag, u16 foodType, FoodbagContents* bagSlots, FoodbagItem* foodDefs);
    /*7*/ s32 (*get_capacity)(FoodbagGamebits* gamebitIDs);
    /*8*/ void (*update_food_quantity_gamebits)(FoodbagContents* bagSlots, FoodbagItem* foodDefs);
    /*9*/ u32 (*get_foodID_from_foodType)(s32 foodTypeBitfield);
};

#endif // _DLLS_56_H
