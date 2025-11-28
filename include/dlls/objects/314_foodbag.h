#ifndef _DLLS_314_H
#define _DLLS_314_H

#include "PR/ultratypes.h"
#include "dlls/engine/29_gplay.h"
#include "game/objects/object.h"

#define MAX_FOOD_DEFINITIONS 12 //Number of food types defined in "foodbag_items", "dino_foodbag_items"
#define MAX_FOOD_PLACED 20      //Max number of items that can be placed in world from a given bag
#define MAX_FOOD_TIMERS 30      //Same as largest bag capacity

#define BAG_CAPACITY_LARGE 30
#define BAG_CAPACITY_MEDIUM 20
#define BAG_CAPACITY_SMALL 10

#define FOOD_TYPE(position) 1 << (position - 1)
#define FOOD_GONE 0

/** ObjSetup struct for food Objects placed in the world
  * 
  * (e.g. OBJ_foodbagRedApple, OBJ_foodbagNewFish, OBJ_foodbagBlueMush)
  */
typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s8 yaw;
    s16 unk1C;
    s16 unk1E;
} PlacedFood_Setup;

/** Defines various parameters for a type of food that can be stored in a food bag */
typedef struct {
    s16 expiry;         //frames until food spoils
    u8 healthRestored;  //when eaten by player, or for sidekicks: energy restored
    u8 slotsUsed;       //food usually occupies 1 bag slot, but fish seem to occupy 2 slots
    u16 expiryID;       //bitfield storing foodID to transform into once expired (0 doesn't spoil)
    s16 gamebitID;      //inventory item gamebit
    s16 objectID;       //Object to create when placed
} FoodbagItem;

/** Handles food objects placed in the world */
typedef struct {
    u8 nextIndex; //index to use when next placing food (eventually wraps around to overwrite oldest)
    Object* objects[MAX_FOOD_PLACED]; //the actual food objects in the world
    u16 foodType[MAX_FOOD_PLACED]; //foodType of each placed object?
} FoodbagPlaced;

typedef struct {
    u16 small;
    u16 medium;
    u16 large;
} FoodbagGamebits;

typedef struct {
    ObjSetup base;
} Foodbag_Setup;

typedef struct {
    struct DLL_56_Putdown *dllPutdown;  //Handles removing/updating food bag items
    FoodbagGamebits capacityGamebits;   //Bag size gamebits
    u16 capacity;                       //Current capacity
    FoodbagContents* bagSlots;           //Expiry timers for bag items
    s8 *playerHealth;                   //Pointer to current health
    s8 *playerHealthMax;                //Pointer to current max health (for Eat First behaviour)
    u8 eatFirst;                        //Player food bag config (prioritise healing vs. storing)
    FoodbagPlaced placedObjects;        //Food placed in world
    f32 playerEatTimer;                 /*Mostly unused, but set when health recovered via eating 
                                         (Possibly intended as timer: too full to eat for a little while?)
                                        */
} Foodbag_Data;

#define BLANK_FOODBAG_ITEM {0, 0, 0, 0, 0, 0}
#define NO_FOOD_OBJECT_ID -1

DLL_INTERFACE(DLL_314_Foodbag) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ int (*Foodbag_is_obtained)(Object* self);
    /*8*/ Object* (*Foodbag_get_nearest_placed_food_of_type)(Object* self, Object* target, s32 foodType);
    /*9*/ int (*Foodbag_destroy_placed_food)(Object* self, Object* foodObject);
    /*10*/ void (*Foodbag_set_capacity)(Object* self);
    /*11*/ void (*Foodbag_collect_food)(Object* self, s32 foodType);
    /*12*/ void (*Foodbag_delete_food_by_gamebit)(Object* self, s16 gamebit);
    /*13*/ int (*Foodbag_set_eat_config)(Object* self, s32 eatFirst);
    /*14*/ u16 (*Foodbag_get_capacity)(Object* self);
    /*15*/ s32 (*Foodbag_count_slots_occupied)(Object* self);
    /*16*/ s16 (*Foodbag_get_anim_objectID_from_food_gamebit)(s16 foodGamebit);
};

#endif // _DLLS_314_H
